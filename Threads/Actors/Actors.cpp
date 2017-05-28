//============================================================================
// Program to demonstrate worker threads embodied in RAII style objects
//
// Threads start on object creation and stop on thread destruction
//============================================================================
#define CATCH_CONFIG_MAIN 

#include "Actor.h"
#include <catch.hpp>
#include <chrono>
#include <thread>

using namespace std;

TEST_CASE("Single actor", "[actor]") 
{
	Actor a;
	auto f1 = a.ping();
	auto f2 = a.ping();
	f1.wait();
	f2.wait();
	REQUIRE(f2.get() == 2);
}


TEST_CASE("Multi actor", "[actor]")
{
	Actor a, b;

	future<int> countA, countB;
	for (size_t i = 0; i < 10; ++i)
	{
		countA = a.ping();
		countB = b.ping();
	}
	countA.wait();
	countB.wait();
	REQUIRE(countA.get() == countB.get());
}

TEST_CASE("User stop", "[actor]")
{
	Actor a;
	for (size_t i = 0; i < 5; ++i)
		a.ping();

	auto stopper = a.stop();
	stopper.wait();
	REQUIRE(! a.isRunning());
}
