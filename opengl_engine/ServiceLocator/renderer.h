#pragma once
#include <ECS/ecsTypes.h>
#include <memory>

class CameraComponent;
struct Color;
class Material;
class LightComponent;
class ModelRendererComponent;
class TextRendererComponent;
class SpriteRendererComponent;
struct Vector3;
struct Vector2Int;
class Box;


/**
* The Renderer Service class (the virtual class provided by the Locator).
*/
class Renderer
{
public:
	virtual ~Renderer() {}


	/**
	* Set the new camera that will be used.
	* @param	camera		The new camera to use.
	*/
	virtual void SetCamera(ComponentHandle<CameraComponent> camera) = 0;

	/**
	* Check if this camera is the one currently used by the renderer.
	* @return			True if the camera is used by the renderer.
	*/
	virtual bool IsActiveCamera(ComponentHandle<CameraComponent> camera) = 0;

	/**
	* Removes the active camera used by the renderer.
	* The renderer will switch to the default camera if possible.
	*/
	virtual void RemoveActiveCamera() = 0;

	/**
	* Retrieve the currently used camera.
	* @return			The currently used camera.
	* @exception		Throws a runtime error if there is no active camera on the renderer.
	*/
	virtual const CameraComponent& GetCamera() const = 0;


	/**
	* Set the new clear color that will be used.
	* @param	clearColor		The new clear color to use.
	*/
	virtual void SetClearColor(Color clearColor) = 0;

	/**
	* Retrieve the currently used clear color.
	* @return			The currently used clear color.
	*/
	virtual const Color GetClearColor() const = 0;


	/**
	* Register a material to the renderer.
	* @param	material	The material to register.
	*/
	virtual void AddMaterial(const Material* material) = 0;

	/**
	* Unregister a material from the renderer.
	* @param	material	The material to unregister.
	*/
	virtual void RemoveMaterial(const Material* material) = 0;


	/**
	* Initialize the renderer.
	* Automatically called by the engine when creating the renderer. Prefer to not call this function.
	* @param	clearColor_		The color use to clear the screen each frame. Can be changed at runtime.
	* @param	windowSize_		The size of the window to render on. Can be changed at runtime.
	* @param	defaultCamera_	The camera used by the renderer if no other camera is valid.
	*/
	virtual void InitializeRenderer(Color clearColor_, Vector2Int windowSize_, ComponentHandle<CameraComponent> defaultCamera_) = 0;

	/**
	* Execute the rendering process.
	* Automatically called by the engine each frame. Prefer to not call this function.
	*/
	virtual void Draw() = 0;


	/**
	* Set the debug camera used by the renderer.
	* Automatically called by the engine. Prefer to not call this function.
	*/
	virtual void SetDebugCamera(ComponentHandle<CameraComponent> debugCamera_) = 0;

	/**
	* Set the debug camera on/off.
	* Automatically called by the engine when turning on and off the debug camera mode. Prefer to not call this function.
	*/
	virtual void SetDebugCamActivated(bool debugCamActivated_) = 0;

	/**
	* Set the debug view mode on/off.
	* Automatically called by the engine when turning on and off the debug view mode. Prefer to not call this function.
	*/
	virtual void SetDebugViewMode(bool debugViewMode_) = 0;

	/**
	* Set the size of the window to render on.
	* Automatically called by the engine when resizing the game window. Prefer to not call this function.
	*/
	virtual void SetWindowSize(Vector2Int windowSize_) = 0;
};