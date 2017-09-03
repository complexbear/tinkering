#define CATCH_CONFIG_MAIN

/* 
	A Y Combinator will take a pure function and recursively call it.

	The pure function is one that may only refer to the variables passed as
	arguments and holds no state.

	Taken from:
	https://rosettacode.org/wiki/Y_combinator#C.2B.2B

	Note this is sooo much easier in Python:
	Y = lambda f: lambda *args: f(Y(f))(*args)
 */

#include <functional>
#include <catch.hpp>


 // Used in our test case functions
 // Simple function type for fibonacci and factorial methods
typedef std::function<int(int)> Func;
// Function that takes as argument and returns a function as defined above
typedef std::function<Func(Func)> FuncFunc;

// Contains a function that will return a type F when called.
// The argument of the member function is another instance of this structure
// i.e. it's recusive
template<typename F>
struct RecursiveFunc {
	std::function<F(RecursiveFunc)> o;
};

// Argument to Y is a function that takes another function of the same argument & return type
// i.e. a recusive function like FuncFunc
// We repeatedly wrap our target function with calls to f.
template <typename ARG, typename RET>
std::function<RET(ARG)> Y(std::function<
								std::function<RET(ARG)>(std::function<RET(ARG)>) // Recusive function type
									   > f)	// Recusive function argument
{				

	// Define the target function signature for convenience
	using TargetFunc = std::function<RET(ARG)>;

	// Create recursive function structure
	RecursiveFunc<TargetFunc> r = {
		// Func here is the F in the RecursiveFunc structure and is our target function
		std::function<TargetFunc(RecursiveFunc<TargetFunc>)>(
			// Inline definition of the function
			[f](RecursiveFunc<TargetFunc> w) {
				// The recursion happens here by calling f with another
				// constructed lambda function that references the recursive
				// function structure (w)
				return f(
					TargetFunc([w](ARG x) {
						return w.o(w)(x);
					})
				);
			}
		)
	};
	return r.o(r);
}


TEST_CASE("Fibonacci", "") {

	// Lambda func that will take a function and use it
	// to sum prev two numbers in the sequence. It returns
	// a new function that combines the chain of these two
	// function calls. 
	FuncFunc fibb = [](Func f) {
		return Func([f](int n) {
			if (n <= 2)
				return 1;
			else
				return f(n - 1) + f(n - 2);
		});
	};

	auto f = Y(fibb);
}

