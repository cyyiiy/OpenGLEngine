#include "rendererOpenGL.h"
#include <Assets/assetManager.h>
#include <ServiceLocator/locator.h>
#include <ECS/ecs.h>
#include <ECS/entity.h>
#include <Rendering/modelRendererComponent.h>
#include <Rendering/billboardRendererComponent.h>
#include <Rendering/shapeRendererComponent.h>
#include <Rendering/Lights/directionalLightComponent.h>
#include <Rendering/Lights/pointLightComponent.h>
#include <Rendering/Lights/spotLightComponent.h>
#include <Rendering/Text/textComponent.h>
#include <Rendering/Hud/spriteComponent.h>
#include <PhysicsAABB/boxCollisionComponent.h>
#include <PhysicsAABB/raycastRendererComponent.h>
#include <algorithm>
#include <stdexcept>



void RendererOpenGL::Draw()
{
	// Clear with flat color
	glClearColor(clearColor.r / 255.0f, clearColor.g / 255.0f, clearColor.b / 255.0f, clearColor.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	
	if (!isCurrentCamValid()) return;
	CameraComponent& current_camera = selectCurrentCam();

	//  RENDERING 3D
	// ========================

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

	// Draw debug part
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


	if (debugViewMode)
	{
		// Draw collisions components
		auto& box_col_renderers_manager = ECS::Manager<BoxCollisionComponent>();
		box_col_renderers_manager.ForEach([this, debug_shader_ptr](const BoxCollisionComponent& box_col_component)
		{
			this->drawBoxCollision(box_col_component, *debug_shader_ptr);
		});

		// Draw raycasts
		auto& raycast_renderers_manager = ECS::Manager<RaycastRendererComponent>();
		raycast_renderers_manager.ForEach([debug_shader_ptr](const RaycastRendererComponent& raycast_renderer_component)
		{
			for (auto& shape : raycast_renderer_component.shapes)
			{
				shape->draw(*debug_shader_ptr);
			}
		});
	}


	//  RENDERING BILLBOARDS
	// ========================

	// Bind the billboard vertex array
	AssetManager::GetVertexArray("billboard").setActive();

	// Compute the matrix and vectors used to render billboards
	const Matrix4 view_proj = view * projection;
	const Vector3 cam_up = current_camera.getCamUp();
	const Vector3 cam_right = current_camera.getCamRight();

	// Activate the billboard shader and set global uniforms
	Shader* billboard_shader = &AssetManager::GetShader("billboard_render");
	billboard_shader->use();
	billboard_shader->setMatrix4("geomViewProj", view_proj.getAsFloatPtr());
	billboard_shader->setVec3("geomCameraUp", cam_up);
	billboard_shader->setVec3("geomCameraRight", cam_right);

	// Render the billboard component
	auto& billboard_renderers_manager = ECS::Manager<BillboardRendererComponent>();
	billboard_renderers_manager.ForEach([this, billboard_shader](const BillboardRendererComponent& billboard_renderer_component)
	{
		this->drawBillboardComponent(billboard_renderer_component, *billboard_shader);
	});

	if (debugViewMode)
	{
		// Draw point lights debug
		point_lights_manager.ForEach([this, billboard_shader](const PointLightComponent& point_light_component)
		{
			this->drawPointLightDebug(point_light_component, *billboard_shader);
		});

		// Draw spot lights debug
		spot_lights_manager.ForEach([this, billboard_shader](const SpotLightComponent& spot_light_component)
		{
			this->drawSpotLightDebug(spot_light_component, *billboard_shader);
		});
	}

	// Unbind the billboard vertex array
	glBindVertexArray(0);


	//  RENDERING HUD
	// ========================

	glDisable(GL_DEPTH_TEST);

	Matrix4 hud_projection = Matrix4::createSimpleViewProj(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

	// Bind the hud (char and sprite) vertex array
	AssetManager::GetVertexArray("hud_quad").setActive();

	// Prepare the shader used in text rendering
	Shader& text_render_shader = AssetManager::GetShader("text_render");
	text_render_shader.use();
	text_render_shader.setMatrix4("projection", hud_projection.getAsFloatPtr());

	auto& hud_texts_manager = ECS::Manager<TextComponent>();
	Shader* text_shader_ptr = &text_render_shader;
	hud_texts_manager.ForEach([this, text_shader_ptr](const TextComponent& text_component)
	{
		this->drawTextComponent(text_component, *text_shader_ptr);
	});

	// Prepare the shader used in sprite rendering
	Shader& sprite_render_shader = AssetManager::GetShader("sprite_render");
	sprite_render_shader.use();
	sprite_render_shader.setMatrix4("projection", hud_projection.getAsFloatPtr());

	auto& hud_sprites_manager = ECS::Manager<SpriteComponent>();
	Shader* sprite_shader_ptr = &sprite_render_shader;
	hud_sprites_manager.ForEach([this, sprite_shader_ptr](const SpriteComponent& sprite_component)
	{
		this->drawSpriteComponent(sprite_component, *sprite_shader_ptr);
	});

	// Unbind the hud vertex array
	glBindVertexArray(0);
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

	// 3. Increase the directional lights count
	lights_count[EDirectionalLight]++;
	if (lights_count[EDirectionalLight] > limit)
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

	// 4. Increase the point lights count
	lights_count[EPointLight]++;
	if (lights_count[EPointLight] > limit)
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

	// 4. Increase the spot lights count
	lights_count[ESpotLight]++;
	if (lights_count[ESpotLight] > limit)
	{
		Locator::getLog().LogMessage_Category("Renderer: There are more than " + std::to_string(limit) + " active spot lights.", LogCategory::Warning);
	}
}

void RendererOpenGL::drawBillboardComponent(const BillboardRendererComponent& billboardComponent, Shader& shaderInUsage)
{
	// 1. Check if the billboard texture is valid
	Texture* billboard_tex = billboardComponent.billboardTexture;
	if (billboard_tex == nullptr) return;

	// 2. Compute the billboard transform
	Matrix4 billboard_tranform = Matrix4::createTranslation(billboardComponent.positionOffset);
	if (!billboardComponent.ignoreOwnerTransform)
	{
		Transform& billboard_owner = *billboardComponent.getOwner();
		billboard_tranform *= billboard_owner.getModelMatrix();
	}

	// 3. Bind the billboard texture
	glActiveTexture(GL_TEXTURE0);
	billboard_tex->use();

	// 4. Set the informations in the shader
	shaderInUsage.setMatrix4("billboardTransform", billboard_tranform.getAsFloatPtr());
	shaderInUsage.setVec2("geomScale", billboardComponent.billboardScale);
	shaderInUsage.setVec3("tintColor", billboardComponent.tintColor.toVector());

	// 5. Draw the billboard
	glDrawArrays(GL_POINTS, 0, 1);

	// 6. Unbind the billboard texture
	glActiveTexture(GL_TEXTURE0);
}

void RendererOpenGL::drawBoxCollision(const BoxCollisionComponent& boxColComponent, Shader& shaderInUsage)
{
	// 1. Compute the model matrix
	const Box collision_box = boxColComponent.getTransformedBox();
	const Matrix4 model_matrix =
		Matrix4::createScale(collision_box.getHalfExtents() * 2.0f) *
		Matrix4::createTranslation(collision_box.getCenterPoint());

	// 2. Choose the debug color
	const Color debug_color = boxColComponent.debugIntersectedLastFrame ? Color::red : Color::green;

	// 3. Set the informations in the shader
	shaderInUsage.setMatrix4("model", model_matrix.getAsFloatPtr());
	shaderInUsage.setVec3("color", debug_color.toVector());

	// 4. Draw the debug cube mesh
	Mesh& cube_mesh = AssetManager::GetSingleMesh("debug_cube");
	cube_mesh.draw(true);
}

void RendererOpenGL::drawPointLightDebug(const PointLightComponent& pointLightComponent, Shader& shaderInUsage)
{
	// 1. Check if the light component is active
	if (!pointLightComponent.active) return;

	// 2. Compute the point light transform
	const Matrix4 point_light_transform =
		Matrix4::createTranslation(pointLightComponent.offset) *
		pointLightComponent.getOwner()->getModelMatrix();

	// 3. Bind the debug point light texture
	glActiveTexture(GL_TEXTURE0);
	AssetManager::GetTexture("debug_icon_point_light").use();

	// 4. Set the informations in the shader
	shaderInUsage.setMatrix4("billboardTransform", point_light_transform.getAsFloatPtr());
	shaderInUsage.setVec2("geomScale", 0.35f, 0.35f);
	shaderInUsage.setVec3("tintColor", pointLightComponent.lightColor);

	// 5. Draw the billboard
	glDrawArrays(GL_POINTS, 0, 1);

	// 6. Unbind the billboard texture
	glActiveTexture(GL_TEXTURE0);
}

void RendererOpenGL::drawSpotLightDebug(const SpotLightComponent& spotLightComponenent, Shader& shaderInUsage)
{
	// 1. Check if the light component is active
	if (!spotLightComponenent.active) return;

	// 2. Compute the spot light transform
	const Matrix4 spot_light_transform =
		Matrix4::createTranslation(spotLightComponenent.offset) *
		spotLightComponenent.getOwner()->getModelMatrix();

	// 3. Bind the debug spot light texture
	glActiveTexture(GL_TEXTURE0);
	AssetManager::GetTexture("debug_icon_spot_light").use();

	// 4. Set the informations in the shader
	shaderInUsage.setMatrix4("billboardTransform", spot_light_transform.getAsFloatPtr());
	shaderInUsage.setVec2("geomScale", 0.35f, 0.35f);
	shaderInUsage.setVec3("tintColor", spotLightComponenent.lightColor);

	// 5. Draw the billboard
	glDrawArrays(GL_POINTS, 0, 1);

	// 6. Unbind the billboard texture
	glActiveTexture(GL_TEXTURE0);
}

void RendererOpenGL::drawTextComponent(const TextComponent& textComponent, Shader& shaderInUsage)
{
	// 1. Check if the text renderer component is valid
	if (!textComponent.active) return;

	const std::string text = textComponent.getText();
	if (text.empty()) return;

	const Font* text_font = textComponent.getFont();
	if (text_font == nullptr) return;

	// 2. Prepare char rotation angle values
	const bool compute_angle = textComponent.rotAngle != 0.0f;
	const float text_angle_rad = Maths::toRadians(textComponent.rotAngle);

	// 3. Precompute text screen pos and pivot point
	const Vector2 screen_pos = (windowSize * (textComponent.position.screenAnchor - Vector2{ 0.5f })) + textComponent.position.offset;
	Vector2 text_pivot = textComponent.position.pivot;
	text_pivot.x = -text_pivot.x;
	text_pivot.y = 1.0f - text_pivot.y;

	// 4. Create local const for easy access to size and scale values
	const Vector2 text_scale = textComponent.scale;
	const Vector2 text_size = textComponent.getTextSize();
	const int font_size = text_font->getFontSize();

	// 5. Prepare data for char iteration (const text chars limit is the max number of chars the shader can treat as one)
	int char_map_ids[TEXT_CHARS_LIMIT]{ 0 };
	Matrix4 char_transforms[TEXT_CHARS_LIMIT]{ Matrix4::identity };
	float x = screen_pos.x;
	const float begin_x = x;
	float y = screen_pos.y;
	y -= (float)(font_size)*text_scale.y; // Allow the text pivot to be applied correctly

	// 6. Set text tint color in the shader and bind font texture array
	shaderInUsage.setVec3("textColor", textComponent.tintColor.toVector());
	text_font->use();

	// 7. Iterate through every character of the text
	std::string::const_iterator c;
	int index = 0;
	for (c = text.begin(); c != text.end(); c++)
	{
		// a. Check if the shader char limit has been reached (security, should never happen)
		if (index >= TEXT_CHARS_LIMIT)
		{
			break;
		}

		// b. Get the font character
		FontCharacter ch = text_font->getCharacter(*c);

		// c. Process line breaks and spaces separatly
		if (*c == '\n')
		{
			y -= ((ch.size.y)) * 1.6f * text_scale.y;
			x = begin_x;
		}
		else if (*c == ' ')
		{
			x += (ch.advance >> 6) * text_scale.x; // Bitshift by 6 (2^6 = 64) to advance the space character size
		}
		else
		{
			// d. Compute the position and scale of the char
			const Vector2 ch_pos = Vector2{ x + ch.bearing.x * text_scale.x, y - (float(font_size) - ch.bearing.y) * text_scale.y };
			const Vector2 ch_scale = Vector2{ float(font_size) * text_scale.x, float(font_size) * text_scale.y };

			// e. Compute the char transform matrix and set it in the array
			if (compute_angle)
			{
				char_transforms[index] =
					Matrix4::createScale(Vector3(ch_scale, 1.0f)) * // Scale to the size of the char
					Matrix4::createTranslation(ch_pos - screen_pos + (text_size * text_pivot)) * // Translate to use the pivot (for rotation)
					Matrix4::createRotationZ(text_angle_rad) * // Rotate the char around the pivot
					Matrix4::createTranslation(screen_pos); // Translate back to the real position of the char
			}
			else
			{
				char_transforms[index] =
					Matrix4::createScale(Vector3(ch_scale, 1.0f)) * // Scale to the size of the char
					Matrix4::createTranslation(ch_pos + (text_size * text_pivot)); // Translate to the position of the char
			}
			
			// f. Set the char texture index in the array
			char_map_ids[index] = ch.textureId;

			// g. Advance the x position to prepare the next character
			x += (ch.advance >> 6) * text_scale.x; // Bitshift by 6 (2^6 = 64) to advance the space character size

			// h. Increment the index and draw the batch of chars if the shader char limit has been reached
			index++;
			if (index >= TEXT_CHARS_LIMIT)
			{
				shaderInUsage.setMatrix4Array("textTransforms", char_transforms[0].getAsFloatPtr(), index);
				shaderInUsage.setIntArray("letterMap", &char_map_ids[0], index);
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, index);

				// i. Reset the index to prepare a new batch of chars
				index = 0;
			}
		}
	}

	// 8. Draw the remaining batch of chars
	if (index > 0)
	{
		shaderInUsage.setMatrix4Array("textTransforms", char_transforms[0].getAsFloatPtr(), index);
		shaderInUsage.setIntArray("letterMap", &char_map_ids[0], index);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, index);
	}

	// 9. Unbind the font texture array
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void RendererOpenGL::drawSpriteComponent(const SpriteComponent& spriteComponent, Shader& shaderInUsage)
{
	// 1. Check if the sprite renderer component is valid
	if (!spriteComponent.active) return;

	Texture* sprite_texture = spriteComponent.texture;
	if (sprite_texture == nullptr) return;

	// 2. Compute the hud matrix
	const Vector2 sprite_size = sprite_texture->getTextureSize() * spriteComponent.scale;
	const Vector2 pivot_inv_y = Vector2{ spriteComponent.position.pivot.x, 1.0f - spriteComponent.position.pivot.y };
	const Vector2 screen_pos = (windowSize * (spriteComponent.position.screenAnchor - Vector2{ 0.5f })) + spriteComponent.position.offset;
	const Matrix4 sprite_transform =
		Matrix4::createScale(Vector3{ sprite_size, 1.0f }) * // Scale to the size of the sprite
		Matrix4::createTranslation(sprite_size * -pivot_inv_y) * // Translate to use the pivot (for rotation)
		Matrix4::createRotationZ(Maths::toRadians(spriteComponent.rotAngle)) * // Rotate the sprite around the pivot
		Matrix4::createTranslation(screen_pos); // Translate back to the real position of the sprite

	// 3. Bind the sprite texture
	glActiveTexture(GL_TEXTURE0);
	sprite_texture->use();

	// 4. Set the values in the shader
	shaderInUsage.setVec3("spriteColor", spriteComponent.tintColor.toVector());
	shaderInUsage.setMatrix4("spriteTransform", sprite_transform.getAsFloatPtr());

	// 5. Draw the texture
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// 6. Unbind the sprite texture
	glActiveTexture(GL_TEXTURE0);
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
		Locator::getLog().LogMessage_Category("The renderer doesn't have an active camera.", LogCategory::Crash);
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



void RendererOpenGL::InitializeRenderer(Color clearColor_, Vector2Int windowSize_, ComponentHandle<CameraComponent> defaultCamera_)
{
	clearColor = clearColor_;
	windowSize = windowSize_;
	defaultCamera = defaultCamera_;
	activeCamera = defaultCamera;
}

void RendererOpenGL::SetDebugCamera(ComponentHandle<CameraComponent> debugCamera_)
{
	debugCamera = debugCamera_;
}

void RendererOpenGL::SetDebugCamActivated(bool debugCamActivated_)
{
	debugCamActivated = debugCamActivated_;
}

void RendererOpenGL::SetDebugViewMode(bool debugViewMode_)
{
	debugViewMode = debugViewMode_;
}

void RendererOpenGL::SetWindowSize(Vector2Int windowSize_)
{
	windowSize = windowSize_;
}