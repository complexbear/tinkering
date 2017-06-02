// PatternMatch.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN

#include <string>
#include <iostream>
#include <catch.hpp>

using namespace std;

const char WILDCARD = '*';

vector<string> tokenize(const string& pattern) {
	vector<string> tokens;
	size_t prevpos = 0,
		   pos = pattern.find_first_of(WILDCARD);

	while (pos != string::npos) {
		string token = pattern.substr(prevpos, (pos-prevpos));
		// If * at front of pattern, token will be empty
		if(token.size()) 
			tokens.push_back(token);
		tokens.push_back("*");
		prevpos = pos + 1;
		pos = pattern.find_first_of(WILDCARD, prevpos);
	};
	// Catch any remaining text
	if (prevpos < pattern.size())
		tokens.push_back(pattern.substr(prevpos));
	return tokens;
}

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
bool match(const string& input, const string& pattern) {
	
	// Basic checks first
	if (input == pattern) return true; 
	if (input.empty() && pattern.empty()) return true;
	if (input.empty() && pattern.size()) return false;
	if (input.size() && pattern.empty()) return false;
	// TODO implement special all wildcard match
	if (pattern == string(&WILDCARD, pattern.size())) return false; 

	// Break pattern into tokens separated by wildcard char
	// Then successively match the tokens against the input string
	vector<string> tokens = tokenize(pattern);
	string prevToken;
	int wildcards = 0;
	auto iter = input.begin();

	for (auto token : tokens) {
		if (token.front() == WILDCARD) {
			++wildcards;
		}
		else {

			if (string(iter, iter + token.size()) == token) {
				// Straight forward match
				iter += token.size();
			}
			else if (wildcards) {
				while (wildcards--) {
					// Consume widcard characters until repeating sequence 
					// ends or a match against the token is found
					char activeChar = *iter;
					while (activeChar == *iter) {
						if (string(iter, iter + token.size()) == token) {
							wildcards = 0;
							break;
						}
						++iter;
					}
				}
				// No more wildcards left so check that the token now matches
				if (string(iter, iter + token.size()) != token) {
					return false;
				}
				iter += token.size();
			}
			else {
				// No match and no active wildcards
				return false;
			}
		}
		prevToken = token;
		if (iter >= input.end()) break;
	}

	return true;
}

TEST_CASE("Tokenize", "[PatternMatch]") {
	REQUIRE(tokenize("hello") == vector<string>({ "hello" }));
	REQUIRE(tokenize("h*llo") == vector<string>({ "h", "*", "llo" }));
	REQUIRE(tokenize("h*llo*") == vector<string>({ "h", "*", "llo", "*" }));
	REQUIRE(tokenize("*") == vector<string>({ "*" }));
	REQUIRE(tokenize("*ello") == vector<string>({ "*", "ello" }));
}

TEST_CASE("Match", "[PatternMatch]") {
	REQUIRE(match("hello", "hello"));
	REQUIRE(match("hello", "*hello"));
	REQUIRE(match("hello", "hello*"));
	REQUIRE(match("hello", "*ello"));
	REQUIRE(match("hello", "*he*o"));
	REQUIRE(match("hello", "*h**o"));
	REQUIRE(match("hello", "*he*lo"));
	REQUIRE(match("", ""));

	REQUIRE_FALSE(match("hello", ""));
	REQUIRE_FALSE(match("hello", "**"));
	REQUIRE_FALSE(match("hello", "*h*lo"));
	REQUIRE_FALSE(match("hello", "*hlo"));
}
