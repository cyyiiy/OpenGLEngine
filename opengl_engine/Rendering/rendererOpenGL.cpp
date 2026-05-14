#include "rendererOpenGL.h"
#include <Assets/assetManager.h>
#include <ServiceLocator/locator.h>
#include <ECS/ecs.h>
#include <ECS/entity.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/shapeRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Rendering/Lights/spotLightComponent.h>
#include <algorithm>
#include <stdexcept>



void RendererOpenGL::draw()
{
	// Clear with flat color
	glClearColor(clearColor.r / 255.0f, clearColor.g / 255.0f, clearColor.b / 255.0f, clearColor.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	
	if (!isCurrentCamValid()) return;
	CameraComponent& current_camera = selectCurrentCam();

	//  RENDERING 3D
	// ===================

	Matrix4 view = current_camera.getViewMatrix();
	Matrix4 projection = Matrix4::createPerspectiveFOV(Maths::toRadians(current_camera.getFov()), static_cast<float>(windowSize.x), static_cast<float>(windowSize.y), 0.1f, 1000.0f);

	// Get the needed component managers
	auto& model_renderers_manager = ECS::Manager<ModelRendererComponent>();
	auto& directional_lights_manager = ECS::Manager<DirectionalLightComponent>();
	auto& point_lights_manager = ECS::Manager<PointLightComponent>();
	auto& spot_lights_manager = ECS::Manager<SpotLightComponent>();

	// Loop through all shaders
	for (auto& materials_by_shaders : materials)
	{
		// Retrieve the shader
		Shader* shader = materials_by_shaders.first;

		if (!shader->isLoaded()) continue;

		// Activate the shader and set the primary uniforms
		shader->use();
		shader->setMatrix4("view", view.getAsFloatPtr());
		shader->setMatrix4("projection", projection.getAsFloatPtr());

		ShaderType shader_type = shader->getShaderType();
		switch (shader_type) // Feels a bit hardcoded, should be cool to find a better way to do this
		{
		case ShaderType::Lit:
			// Use lights
			lights_count[EDirectionalLight] = 0;
			lights_count[EPointLight] = 0;
			lights_count[ESpotLight] = 0;

			directional_lights_manager.ForEach([this, shader](const DirectionalLightComponent& dir_light_component)
			{
				this->useDirectionalLight(dir_light_component, *shader);
			});

			point_lights_manager.ForEach([this, shader](const PointLightComponent& point_light_component)
			{
				this->usePointLight(point_light_component, *shader);
			});
			shader->setInt("nbPointLights", lights_count[EPointLight]);

			spot_lights_manager.ForEach([this, shader](const SpotLightComponent& spot_light_component)
			{
				this->useSpotLight(spot_light_component, *shader);
			});
			shader->setInt("nbSpotLights", lights_count[ESpotLight]);

			shader->setVec3("viewPos", current_camera.getCamPosition());

			break;

		case ShaderType::Unlit:
			// Nothing else to do
			break;
		}
		
		// Loop through all materials that use the shader
		for (auto& material : materials_by_shaders.second)
		{
			shader->setBool("beta_prevent_tex_scaling", false); // Should do a better thing for all beta parameters
			shader->setFloat("beta_tex_scaling_factor", 1.0f); // Should do a better thing for all beta parameters

			material->use();

			// Loop through all model renderer components to draw all meshes that uses the active material
			model_renderers_manager.ForEach([this, material](const ModelRendererComponent& model_component)
			{
				this->drawModelComponent(model_component, *material);
			});
		}
	}

	//  draw debug part
	Material& debug_draw_mat = AssetManager::GetMaterial("debug_draws");
	Shader& debug_draw_shader = debug_draw_mat.getShader();
	debug_draw_shader.use();
	debug_draw_shader.setMatrix4("view", view.getAsFloatPtr());
	debug_draw_shader.setMatrix4("projection", projection.getAsFloatPtr());
	
	debug_draw_mat.use();
	Shader* debug_shader_ptr = debug_draw_mat.getShaderPtr();

	auto& shape_renderers_manager = ECS::Manager<ShapeRendererComponent>();
	shape_renderers_manager.ForEach([debug_shader_ptr](const ShapeRendererComponent& shape_renderer_component)
	{
		shape_renderer_component.shape->draw(*debug_shader_ptr);
	});



	// TODO (when physics is back online):
	// - Renderer get all collision components (only Box AABB) through ECS:
	//   These components have informations (box, entity) for the renderer to draw them
	// - Then renderer get all "RaycastRendererComponents" and draw their shapes
	//   (raycasts objects only need to exist to render debug after the physic job is done)

	/* if (physicsDebugMode)
	{
		Locator::getPhysics().DrawCollisionsDebug(debug_draw_mat);
	}*/


	/*  WILL BE RE-IMPLEMENTED LATER (HUD)

	//  RENDERING HUD
	// ===================

	glDisable(GL_DEPTH_TEST);

	Matrix4 hud_projection = Matrix4::createSimpleViewProj(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

	//  prepare the shader used in text rendering
	Shader& text_render_shader = AssetManager::GetShader("text_render");
	text_render_shader.use();
	text_render_shader.setMatrix4("projection", hud_projection.getAsFloatPtr());

	//  bind the char (and sprite) vertex array
	AssetManager::GetVertexArray("hud_quad").setActive();

	for (auto& text : texts)
	{
		//  check text enabled
		if (!text->getEnabled()) continue;

		//  set text color
		text_render_shader.setVec3("textColor", text->getTintColor().toVector());

		//  check if computing angle is needed or not
		float text_angle = text->getRotAngle();
		const bool compute_angle = text_angle != 0.0f;
		text_angle = Maths::toRadians(text_angle);

		//  prepare arrays of datas that will be sent to the shader
		int char_map_ids[TEXT_CHARS_LIMIT]{ 0 };
		Matrix4 char_transforms[TEXT_CHARS_LIMIT]{ Matrix4::identity };

		//  retrieve datas of the text
		float x = text->getScreenPos().x;
		float y = text->getScreenPos().y;
		const float begin_x = x;

		const std::string& text_text = text->getText();
		const Vector2 text_scale = text->getScale();

		Vector2 text_pivot = text->getPivot(); //  pivot need a little treatment to be used properly
		text_pivot.x = -text_pivot.x;
		text_pivot.y = 1.0f - text_pivot.y;
		const Vector2 text_size = text->getSize();

		//  get font and bind font texture array
		const Font& text_font = text->getTextFont();
		text_font.use();
		const int font_size = text_font.getFontSize();

		//  allow the text pivot to be applied correctly  
		y -= (float)(font_size) * text_scale.y;

		//  iterate through all characters
		std::string::const_iterator c;
		int index = 0;
		for (c = text_text.begin(); c != text_text.end(); c++)
		{
			if (index >= TEXT_CHARS_LIMIT)
			{
				break;
			}

			FontCharacter ch = text_font.getCharacter(*c);

			if (*c == '\n')
			{
				y -= ((ch.Size.y)) * 1.6f * text_scale.y;
				x = begin_x;
			}
			else if (*c == ' ')
			{
				x += (ch.Advance >> 6) * text_scale.x; // bitshift by 6 (2^6 = 64) to advance the space character size
			}
			else
			{
				//  compute pos and scale of the char
				const Vector2 ch_pos = Vector2{ x + ch.Bearing.x * text_scale.x, y - (float(font_size) - ch.Bearing.y) * text_scale.y };
				const Vector2 ch_scale = Vector2{ float(font_size) * text_scale.x, float(font_size) * text_scale.y };

				if (compute_angle)
				{
					char_transforms[index] =
						Matrix4::createScale(Vector3(ch_scale, 1.0f)) *
						Matrix4::createTranslation(ch_pos - text->getScreenPos() + (text_size * text_pivot)) *
						Matrix4::createRotationZ(text_angle) *
						Matrix4::createTranslation(text->getScreenPos());
				}
				else
				{
					char_transforms[index] =
						Matrix4::createScale(Vector3(ch_scale, 1.0f)) *
						Matrix4::createTranslation(ch_pos + (text_size * text_pivot));
				}
				char_map_ids[index] = ch.TextureID;

				x += (ch.Advance >> 6) * text_scale.x; // bitshift by 6 (2^6 = 64) to advance the character size

				index++;
				if (index >= TEXT_CHARS_LIMIT)
				{
					//  draw array of max TEXT_CHARS_LIMIT chars
					text_render_shader.setMatrix4Array("textTransforms", char_transforms[0].getAsFloatPtr(), index);
					text_render_shader.setIntArray("letterMap", &char_map_ids[0], index);
					glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, index);

					index = 0;
				}
			}
		}

		//  draw array of remaining chars
		if (index > 0)
		{
			text_render_shader.setMatrix4Array("textTransforms", char_transforms[0].getAsFloatPtr(), index);
			text_render_shader.setIntArray("letterMap", &char_map_ids[0], index);
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, index);
		}

		//  unbind font texture array
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	//  do not unbind char vertex array as it will also serve as sprite vertex array


	//  prepare the shader used in sprite rendering
	Shader& sprite_render_shader = AssetManager::GetShader("sprite_render");
	sprite_render_shader.use();
	sprite_render_shader.setMatrix4("projection", hud_projection.getAsFloatPtr());

	for (auto& sprite : sprites)
	{
		//  check sprite enabled
		if (!sprite->canDraw()) continue;

		//  use sprite texture
		glActiveTexture(GL_TEXTURE0);
		sprite->getSpriteTexture().use();

		//  set sprite color
		sprite_render_shader.setVec3("spriteColor", sprite->getTintColor().toVector());

		//  set sprite transform
		sprite_render_shader.setMatrix4("spriteTransform", sprite->getHudTransform().getAsFloatPtr());

		//  draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//  unbind sprite texture
		glActiveTexture(GL_TEXTURE0);
	}

	glBindVertexArray(0);

	*/
}

void RendererOpenGL::drawModelComponent(const ModelRendererComponent& modelComponent, Material& materialInUsage)
{
	// 1. Check if the model component is valid and uses the currently processed material
	Model* model = modelComponent.model;
	if (model == nullptr) return;
	if (!model->useMaterial(materialInUsage)) return;

	// 2. Compute the model matrices
	Matrix4 model_matrix = modelComponent.offset.getModelMatrix();
	Vector3 model_scale = modelComponent.offset.getScale();

	if (!modelComponent.ignoreOwnerTransform)
	{
		Transform& model_owner = *modelComponent.getOwner();
		model_matrix *= model_owner.getModelMatrix();
		model_scale *= model_owner.getScale();
	}

	Matrix4 normal_matrix = model_matrix;
	normal_matrix.invert();
	normal_matrix.transpose();

	// 3. Set the matrices in the shader
	Shader& shader_used = materialInUsage.getShader();

	shader_used.setMatrix4("model", model_matrix.getAsFloatPtr());
	shader_used.setMatrix4("normalMatrix", normal_matrix.getAsFloatPtr());
	shader_used.setVec3("scale", model_scale);

	// 4. Draw the model
	model->draw(materialInUsage);
}

void RendererOpenGL::useDirectionalLight(const DirectionalLightComponent& dirLightComponent, Shader& shaderInUsage)
{
	// 1. Check if the directional lights limit has been reached
	const int limit = LIGHTS_LIMITS.at(EDirectionalLight);
	if (lights_count[EDirectionalLight] >= limit) return;

	if (!dirLightComponent.active) return;

	// 2. Set the light informations in the shader
	shaderInUsage.setVec3("dirLight.direction", dirLightComponent.direction);

	shaderInUsage.setVec3("dirLight.ambient", dirLightComponent.ambientStrength * dirLightComponent.lightColor.toVector());
	shaderInUsage.setVec3("dirLight.diffuse", dirLightComponent.diffuseStrength * dirLightComponent.lightColor.toVector());
	shaderInUsage.setVec3("dirLight.specular", Color::white.toVector());

	// 3. Increase the directional lights limit
	lights_count[EDirectionalLight]++;
	if (lights_count[EDirectionalLight] >= limit)
	{
		Locator::getLog().LogMessage_Category("Renderer: There are more than " + std::to_string(limit) + " active directional lights.", LogCategory::Warning);
	}
}

void RendererOpenGL::usePointLight(const PointLightComponent& pointLightComponent, Shader& shaderInUsage)
{
	// 1. Check if the point lights limit has been reached
	const int limit = LIGHTS_LIMITS.at(EPointLight);
	if (lights_count[EPointLight] >= limit) return;

	if (!pointLightComponent.active) return;

	// 2. Compute the point light world position
	const Matrix4 light_pos_matrix = 
		Matrix4::createTranslation(pointLightComponent.offset) * 
		pointLightComponent.getOwner()->getModelMatrix();
	const Vector3 light_position = light_pos_matrix.getTranslation();

	// 3. Set the light informations in the shader
	std::string light_index = std::to_string(lights_count[EPointLight]);

	shaderInUsage.setVec3("pointLights[" + light_index + "].position", light_position);

	shaderInUsage.setVec3("pointLights[" + light_index + "].ambient", pointLightComponent.ambientStrength * pointLightComponent.lightColor.toVector());
	shaderInUsage.setVec3("pointLights[" + light_index + "].diffuse", pointLightComponent.diffuseStrength * pointLightComponent.lightColor.toVector());
	const Color specular_color = pointLightComponent.useColorToSpecular ? pointLightComponent.lightColor : Color::white;
	shaderInUsage.setVec3("pointLights[" + light_index + "].specular", specular_color.toVector());

	shaderInUsage.setFloat("pointLights[" + light_index + "].constant", pointLightComponent.attenuation.constant);
	shaderInUsage.setFloat("pointLights[" + light_index + "].linear", pointLightComponent.attenuation.linear);
	shaderInUsage.setFloat("pointLights[" + light_index + "].quadratic", pointLightComponent.attenuation.quadratic);

	// 4. Increase the point lights limit
	lights_count[EPointLight]++;
	if (lights_count[EPointLight] >= limit)
	{
		Locator::getLog().LogMessage_Category("Renderer: There are more than " + std::to_string(limit) + " active point lights.", LogCategory::Warning);
	}
}

void RendererOpenGL::useSpotLight(const SpotLightComponent& spotLightComponent, Shader& shaderInUsage)
{
	// 1. Check if the spot lights limit has been reached
	const int limit = LIGHTS_LIMITS.at(ESpotLight);
	if (lights_count[ESpotLight] >= limit) return;

	if (!spotLightComponent.active) return;

	// 2. Compute the spot light world position
	const Matrix4 light_pos_matrix =
		Matrix4::createTranslation(spotLightComponent.offset) *
		spotLightComponent.getOwner()->getModelMatrix();
	const Vector3 light_position = light_pos_matrix.getTranslation();

	// 3. Set the light informations in the shader
	std::string light_index = std::to_string(lights_count[ESpotLight]);

	shaderInUsage.setVec3("spotLights[" + light_index + "].position", light_position);
	shaderInUsage.setVec3("spotLights[" + light_index + "].direction", spotLightComponent.direction);

	shaderInUsage.setVec3("spotLights[" + light_index + "].ambient", spotLightComponent.ambientStrength * spotLightComponent.lightColor.toVector());
	shaderInUsage.setVec3("spotLights[" + light_index + "].diffuse", spotLightComponent.diffuseStrength * spotLightComponent.lightColor.toVector());
	const Color specular_color = spotLightComponent.useColorToSpecular ? spotLightComponent.lightColor : Color::white;
	shaderInUsage.setVec3("spotLights[" + light_index + "].specular", specular_color.toVector());

	shaderInUsage.setFloat("spotLights[" + light_index + "].cutOff", spotLightComponent.cutoffs.innerCutoff);
	shaderInUsage.setFloat("spotLights[" + light_index + "].outerCutOff", spotLightComponent.cutoffs.outerCutoff);

	shaderInUsage.setFloat("spotLights[" + light_index + "].constant", spotLightComponent.attenuation.constant);
	shaderInUsage.setFloat("spotLights[" + light_index + "].linear", spotLightComponent.attenuation.linear);
	shaderInUsage.setFloat("spotLights[" + light_index + "].quadratic", spotLightComponent.attenuation.quadratic);

	// 4. Increase the spot lights limit
	lights_count[ESpotLight]++;
	if (lights_count[ESpotLight] >= limit)
	{
		Locator::getLog().LogMessage_Category("Renderer: There are more than " + std::to_string(limit) + " active spot lights.", LogCategory::Warning);
	}
}



void RendererOpenGL::SetCamera(ComponentHandle<CameraComponent> camera)
{
	if (!ECS::IsComponentHandleValid(camera))
	{
		Locator::getLog().LogMessage_Category("Renderer: Tried to set a camera that doesn't exist.", LogCategory::Error);
	}

	activeCamera = camera;
}

bool RendererOpenGL::IsActiveCamera(ComponentHandle<CameraComponent> camera)
{
	return activeCamera == camera;
}

void RendererOpenGL::RemoveActiveCamera()
{
	activeCamera = defaultCamera;
}

const CameraComponent& RendererOpenGL::GetCamera() const
{
	if (ECS::IsComponentHandleValid(activeCamera))
	{
		return ECS::GetComponent(activeCamera);
	}
	else
	{
		throw std::runtime_error("The renderer doesn't have an active camera.");
	}
}


void RendererOpenGL::SetClearColor(Color clearColor_)
{
	clearColor = clearColor_;
}

const Color RendererOpenGL::GetClearColor() const
{
	return clearColor;
}


void RendererOpenGL::AddMaterial(Material* material)
{
	materials[material->getShaderPtr()].push_back(material);
}

void RendererOpenGL::RemoveMaterial(Material* material)
{
	auto iter = std::find(materials[material->getShaderPtr()].begin(), materials[material->getShaderPtr()].end(), material);
	if (iter == materials[material->getShaderPtr()].end())
	{
		Locator::getLog().LogMessage_Category("Renderer: Tried to remove a material that doesn't exist.", LogCategory::Error);
		return;
	}

	std::iter_swap(iter, materials[material->getShaderPtr()].end() - 1);
	materials[material->getShaderPtr()].pop_back();
}


CameraComponent& RendererOpenGL::selectCurrentCam()
{
	return debugCamActivated ? ECS::GetComponent(debugCamera) : ECS::GetComponent(activeCamera);
}

bool RendererOpenGL::isCurrentCamValid()
{
	return debugCamActivated ? ECS::IsComponentHandleValid(debugCamera) : ECS::IsComponentHandleValid(activeCamera);
}



void RendererOpenGL::initializeRenderer(Color clearColor_, Vector2Int windowSize_, ComponentHandle<CameraComponent> defaultCamera_)
{
	clearColor = clearColor_;
	windowSize = windowSize_;
	defaultCamera = defaultCamera_;
	activeCamera = defaultCamera;
}

void RendererOpenGL::setDebugCamera(ComponentHandle<CameraComponent> debugCamera_)
{
	debugCamera = debugCamera_;
}

void RendererOpenGL::setDebugCamActivated(bool debugCamActivated_)
{
	debugCamActivated = debugCamActivated_;
}

void RendererOpenGL::setWindowSize(Vector2Int windowSize_)
{
	windowSize = windowSize_;
}