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
	virtual void AddMaterial(Material* material) = 0;

	/**
	* Unregister a material from the renderer.
	* @param	material	The material to unregister.
	*/
	virtual void RemoveMaterial(Material* material) = 0;
};