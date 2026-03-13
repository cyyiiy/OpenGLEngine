#pragma once
#include <ECS/component.h>


class BenchmarkComponentB : public Component
{
public:
	void incrementCounter(int* counter);

protected:
	void init() override;
	
private:
	int randomNumber{ 0 };
};

