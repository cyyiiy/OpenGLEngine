#pragma once
#include <ECS/entityContainer.h>
#include <string>

class Scene : public EntityContainer
{
public:
	virtual ~Scene() {} 

	void load();
	void unload(bool exitGame);
	void update(float dt);

	void lateUpdate();


	//  Overridable functions for user-created scenes
	// -----------------------------------------------
	virtual std::string getSceneName() { return ""; };
	virtual void loadSceneAssets() {}

protected:
	virtual void loadScene() = 0;
	virtual void unloadScene() = 0;
	virtual void updateScene(float dt) {}

private:
	bool firstFrame{ true };
};