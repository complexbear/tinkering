// PatternMatch.cpp : Defines the entry point for the console application.
//
#include <string>
#include <catch.hpp>

using namespace std;
#define CATCH_CONFIG_MAIN

/*
 * Match input string against pattern
 * Pattern may consist of special character * which 
 * can match 0 or more repetitions of any other character
 *
 * A single * wildcard can only match repetitions of a single
 * character.
 * e.g.  haat == h*t
 *       hart != h*t
 */
string match(const string& input, const string& pattern) {
	

}

TEST_CASE("PatternMatch") {
	REQUIRE(match("hello", "hello"), true);
	REQUIRE(match("hello", "*hello"), true);
	REQUIRE(match("hello", "hello*"), true);
	REQUIRE(match("hello", "*ello"), true);
	REQUIRE(match("hello", "*he*o"), true);
	REQUIRE(match("hello", "*h*lo"), false);
	REQUIRE(match("hello", "*h**o"), true);
}
