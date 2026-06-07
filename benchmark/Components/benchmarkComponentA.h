#pragma once
#include <ECS/behaviorComponent.h>


class BenchmarkComponentA : public BehaviorComponent
{
public:
	void setPointers(Entity* dummy, int* counter);

	void update(float deltaTime) override;

private:
	Entity* dummyEntity{ nullptr };
	int* sharedCounter{ nullptr };
};

