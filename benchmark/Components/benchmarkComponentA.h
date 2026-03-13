#pragma once
#include <ECS/component.h>


class BenchmarkComponentA : public Component
{
public:
	void SetPointers(Entity* dummy, int* counter);

protected:
	void update(float deltaTime) override;

private:
	Entity* dummyEntity{ nullptr };
	int* sharedCounter{ nullptr };
};

