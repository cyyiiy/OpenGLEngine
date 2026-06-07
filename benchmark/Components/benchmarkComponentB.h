#pragma once
#include <ECS/behaviorComponent.h>


class BenchmarkComponentB : public BehaviorComponent
{
public:
	void incrementCounter(int* counter);

	void init() override;
	
private:
	int randomNumber{ 0 };
};

