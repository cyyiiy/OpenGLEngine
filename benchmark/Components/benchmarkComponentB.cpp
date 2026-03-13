#include "benchmarkComponentB.h"
#include <random>


void BenchmarkComponentB::incrementCounter(int* counter)
{
	*counter = *counter + randomNumber;
}

void BenchmarkComponentB::init()
{
	static std::mt19937 gen(std::random_device{}());
	static std::uniform_int_distribution<> dist(1, 1000);
	randomNumber = dist(gen);
}
