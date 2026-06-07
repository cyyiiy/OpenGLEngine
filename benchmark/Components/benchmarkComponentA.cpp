#include "benchmarkComponentA.h"
#include "benchmarkComponentB.h"
#include <ECS/entity.h>
#include <ServiceLocator/locator.h>


void BenchmarkComponentA::setPointers(Entity* dummy, int* counter)
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
	ComponentHandle<BenchmarkComponentB> comp = dummyEntity->getComponentOfClass<BenchmarkComponentB>();
	ECS::GetComponent(comp).incrementCounter(sharedCounter);
	dummyEntity->removeComponent(comp);
}
