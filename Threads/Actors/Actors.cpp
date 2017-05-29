//============================================================================
// Program to demonstrate worker threads embodied in RAII style objects
//
// Threads start on object creation and stop on thread destruction
//============================================================================
#define CATCH_CONFIG_MAIN 

#include "Actor.h"
#include <catch.hpp>
#include <string>


class Counter : public Actor<int>
{
public:
	Actor::FutureResult count()
	{
		return execute([=]() {
			return ++counter_;
		});
	}
private:
	int counter_ = 0;
};

class Printer : public Actor<std::string>
{
public:
	Actor::FutureResult print(int i)
	{
		return execute([=]() {
			return std::to_string(i);
		});
	}
};


TEST_CASE("Single actor", "[actor]") 
{

	Counter a;
	auto f1 = a.count();
	auto f2 = a.count();
	f1.wait();
	f2.wait();
	REQUIRE(f2.get() == 2);
}


TEST_CASE("Multi actor", "[actor]")
{
	Counter a, b;

	std::future<int> countA, countB;
	for (size_t i = 0; i < 10; ++i)
	{
		countA = a.count();
		countB = b.count();
	}
	countA.wait();
	countB.wait();
	REQUIRE(countA.get() == countB.get());
}

TEST_CASE("User stop", "[actor]")
{
	Printer a;
	std::vector<Printer::FutureResult> results;
	for (int i = 0; i < 5; ++i)
		results.push_back(a.print(i));

	for (auto& r : results)
	{
		r.wait();
		std::cout << r.get() << std::endl;
	}

	auto stopper = a.stop();
	stopper.wait();
	REQUIRE(! a.isRunning());
}
