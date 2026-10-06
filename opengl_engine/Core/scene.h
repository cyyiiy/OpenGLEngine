#pragma once
#include <ECS/entityContainer.h>
#include <string>

class Scene : public EntityContainer
{
public:
	virtual ~Scene() {} 

	void loadAssets();
	void load();
	void unload(bool exitGame);
	void update(float dt);

	void lateUpdate();


	//  Overridable functions for user-created scenes
	// -----------------------------------------------
	virtual std::string getSceneName() { return ""; };

protected:
	virtual void loadSceneAssets() {}
	virtual void loadScene() = 0;
	virtual void unloadScene() = 0;
	virtual void updateScene(float dt) {}

private:
	bool firstFrame{ true };
};