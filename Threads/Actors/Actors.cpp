//============================================================================
// Program to demonstrate worker threads embodied in RAII style objects
//
// Threads start on object creation and stop on thread destruction
//============================================================================

#include "Actor.h"

int main() {

	std::cout << "Main thread " << std::this_thread::get_id() << std::endl;
	{
		Actor a;
		a.ping();
	}
	return 0;
}
