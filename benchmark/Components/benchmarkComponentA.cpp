#include "benchmarkComponentA.h"
#include "benchmarkComponentB.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>


void BenchmarkComponentA::SetPointers(Entity* dummy, int* counter)
{
	dummyEntity = dummy;
	sharedCounter = counter;
}

void BenchmarkComponentA::update(float deltaTime)
{
	if (!dummyEntity)
	{
		Locator::getLog().LogMessage_Category("Benchmark Component A: Dummy entity is null!", LogCategory::Error);
		return;
	}

	if (!sharedCounter)
	{
		Locator::getLog().LogMessage_Category("Benchmark Component A: Shared counter is null!", LogCategory::Error);
		return;
	}

	dummyEntity->addComponentByClass<BenchmarkComponentB>();
	std::shared_ptr<BenchmarkComponentB> comp = dummyEntity->getComponentByClass<BenchmarkComponentB>();
	comp->incrementCounter(sharedCounter);
	dummyEntity->removeComponent(comp);
}
