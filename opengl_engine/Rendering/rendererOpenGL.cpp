#include "rendererOpenGL.h"
#include <Assets/assetManager.h>
#include <ServiceLocator/locator.h>
#include <algorithm>

#include "Debug/point.h"
#include "Debug/line.h"
#include "Debug/cube.h"


void RendererOpenGL::draw()
{
	//  clear with flat color
	glClearColor(clearColor.r / 255.0f, clearColor.g / 255.0f, clearColor.b / 255.0f, clearColor.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	
	if (!isCurrentCamValid()) return;
	CameraComponent& current_camera = selectCurrentCam();

	//  RENDERING 3D
	// ===================

	Matrix4 view = current_camera.getViewMatrix();
	Matrix4 projection = Matrix4::createPerspectiveFOV(Maths::toRadians(current_camera.getFov()), static_cast<float>(windowSize.x), static_cast<float>(windowSize.y), 0.1f, 1000.0f);

	//  loop through all shaders
	for (auto& materials_by_shaders : materials)
	{
		//  retrieve the shader
		Shader* shader = materials_by_shaders.first;

		if (!shader->isLoaded()) continue;

		//  activate the shader and set the primary uniforms
		shader->use();
		shader->setMatrix4("view", view.getAsFloatPtr());
		shader->setMatrix4("projection", projection.getAsFloatPtr());

		ShaderType shader_type = shader->getShaderType();
		switch (shader_type) //  feels a bit hardcoded, should be cool to find a better way to do this
		{
		case ShaderType::Lit:
			//  use lights
			for (auto light_t : lights)
			{
				LightType light_type = light_t.first;

				int light_type_used = 0;
				for (auto light : light_t.second)
				{
					if (!light->isActive()) continue;

					light->useLight(*shader, light_type_used);

					light_type_used++;
					if (light_type_used >= LIGHTS_LIMITS.at(light_type))
					{
						break;
					}
				}

				switch (light_type)
				{
				case EPointLight:
					shader->setInt("nbPointLights", light_type_used++);
					break;
				case ESpotLight:
					shader->setInt("nbSpotLights", light_type_used++);
					break;
				}
			}

			shader->setVec3("viewPos", current_camera.getCamPosition());

			break;

		case ShaderType::Unlit:
			//  nothing else to do
			break;
		}
		
		//  loop through all materials that use the shader
		for (auto& material : materials_by_shaders.second)
		{
			shader->setBool("beta_prevent_tex_scaling", false); //  should do a better thing for all beta parameters
			shader->setFloat("beta_tex_scaling_factor", 1.0f); //  should do a better thing for all beta parameters

			material->use();

			//  loop through all model renderer components to draw all meshes that uses the material
			for (auto& model_renderer : modelRenderers)
			{
				if (model_renderer->useMaterial(*material)) model_renderer->draw(*material);
			}
		}
	}


	//  draw debug part
	Material& debug_draw_mat = AssetManager::GetMaterial("debug_draws");
	Shader& debug_draw_shader = debug_draw_mat.getShader();
	debug_draw_shader.use();
	debug_draw_shader.setMatrix4("view", view.getAsFloatPtr());
	debug_draw_shader.setMatrix4("projection", projection.getAsFloatPtr());

	debug_draw_mat.use();

	for (auto& debug_draw : debugDraws)
	{
		debug_draw->draw(debug_draw_mat, debug_draw->getColor());
	}

	if (physicsDebugMode)
	{
		Locator::getPhysics().DrawCollisionsDebug(debug_draw_mat);
	}





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
}


void RendererOpenGL::updateDebugDraws(float dt)
{
	std::vector<DebugRenderBase*> expired_debug_draws;
	for (auto& debug_draw : debugDraws)
	{
		if (debug_draw->updateLifetime(dt))
		{
			expired_debug_draws.push_back(debug_draw);
		}
	}

	for (auto& expired_debug_draw : expired_debug_draws)
	{
		auto iter = std::find(debugDraws.begin(), debugDraws.end(), expired_debug_draw);
		debugDraws.erase(iter);
	}

	debugDraws.shrink_to_fit();
	expired_debug_draws.clear();
}



void RendererOpenGL::SetCamera(std::weak_ptr<CameraComponent> camera)
{
	if(activeCamera) activeCamera->setActiveValue(false);

	activeCamera = camera.lock();

	if (!activeCamera)
	{
		activeCamera = defaultCamera;
		return;
	}

	activeCamera->setActiveValue(true);
}

const std::shared_ptr<CameraComponent> RendererOpenGL::GetCamera() const
{
	return activeCamera;
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


void RendererOpenGL::AddLight(LightComponent* light)
{
	lights[light->getLightType()].push_back(light);

	if (lights[light->getLightType()].size() > LIGHTS_LIMITS.at(light->getLightType()))
	{
		Locator::getLog().LogMessage_Category("Renderer: A light has been added but will not be used as it overflow the lit shader array.", LogCategory::Warning);
	}
}

void RendererOpenGL::RemoveLight(LightComponent* light)
{
	auto iter = std::find(lights[light->getLightType()].begin(), lights[light->getLightType()].end(), light);
	if (iter == lights[light->getLightType()].end())
	{
		Locator::getLog().LogMessage_Category("Renderer: Tried to remove a light that doesn't exist.", LogCategory::Error);
		return;
	}

	std::iter_swap(iter, lights[light->getLightType()].end() - 1);
	lights[light->getLightType()].pop_back();
}


void RendererOpenGL::AddModelRenderer(ModelRendererComponent* modelRenderer)
{
	modelRenderers.push_back(modelRenderer);
}

void RendererOpenGL::RemoveModelRenderer(ModelRendererComponent* modelRenderer)
{
	auto iter = std::find(modelRenderers.begin(), modelRenderers.end(), modelRenderer);
	if (iter == modelRenderers.end())
	{
		Locator::getLog().LogMessage_Category("Renderer: Tried to remove a model renderer that doesn't exist.", LogCategory::Error);
		return;
	}

	std::iter_swap(iter, modelRenderers.end() - 1);
	modelRenderers.pop_back();
}


void RendererOpenGL::AddText(TextRendererComponent* text)
{
	texts.push_back(text);
}

void RendererOpenGL::RemoveText(TextRendererComponent* text)
{
	auto iter = std::find(texts.begin(), texts.end(), text);
	if (iter == texts.end())
	{
		Locator::getLog().LogMessage_Category("Renderer: Tried to remove a text that doesn't exist.", LogCategory::Error);
		return;
	}

	std::iter_swap(iter, texts.end() - 1);
	texts.pop_back();
}

void RendererOpenGL::AddSprite(SpriteRendererComponent* sprite)
{
	sprites.push_back(sprite);
}

void RendererOpenGL::RemoveSprite(SpriteRendererComponent* sprite)
{
	auto iter = std::find(sprites.begin(), sprites.end(), sprite);
	if (iter == sprites.end())
	{
		Locator::getLog().LogMessage_Category("Renderer: Tried to remove a sprite that doesn't exist.", LogCategory::Error);
		return;
	}

	std::iter_swap(iter, sprites.end() - 1);
	sprites.pop_back();
}


void RendererOpenGL::DrawDebugPoint(const Vector3& pointPosition, const Color& color, float duration)
{
	Point* debug_point = new Point();
	debug_point->setupDebugDraw(color, duration);
	debug_point->setPointPostition(pointPosition);
	debugDraws.push_back(debug_point);
}

void RendererOpenGL::DrawDebugLine(const Vector3& pointA, const Vector3& pointB, const Color& color, float duration)
{
	Line* debug_line = new Line();
	debug_line->setupDebugDraw(color, duration);
	debug_line->setPoints(pointA, pointB);
	debugDraws.push_back(debug_line);
}

void RendererOpenGL::DrawDebugCube(const Box& boxInfos, const Color& color, float duration)
{
	Cube* debug_cube = new Cube();
	debug_cube->setupDebugDraw(color, duration);
	debug_cube->setBox(boxInfos);
	debugDraws.push_back(debug_cube);
}


CameraComponent& RendererOpenGL::selectCurrentCam()
{
	return debugCamActivated ? *debugCamera : *activeCamera;
}

bool RendererOpenGL::isCurrentCamValid()
{
	return debugCamActivated ? debugCamera.operator bool() : activeCamera.operator bool();
}



void RendererOpenGL::initializeRenderer(Color clearColor_, Vector2Int windowSize_, std::weak_ptr<CameraComponent> defaultCamera_)
{
	clearColor = clearColor_;
	windowSize = windowSize_;
	defaultCamera = defaultCamera_.lock();
	activeCamera = defaultCamera;
}

void RendererOpenGL::setDebugCamera(std::weak_ptr<CameraComponent> debugCamera_)
{
	debugCamera = debugCamera_.lock();
}

void RendererOpenGL::setDebugCamActivated(bool debugCamActivated_)
{
	debugCamActivated = debugCamActivated_;
}

void RendererOpenGL::setWindowSize(Vector2Int windowSize_)
{
	windowSize = windowSize_;
}