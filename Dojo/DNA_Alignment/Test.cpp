#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "DNA_Alignment.h"

TEST_CASE("LongestCommonSequence", "")
{
    Grid::StrandPair s{
        "GAGGTCAG",
        "GATGCTAG"
    };

    s = longestCommonSequence(s);
    REQUIRE(s.first == "GA_G_TAG");
    REQUIRE(s.second == "GA_G_TAG");
}

TEST_CASE("GlobalOptimal", "")
{
    Grid::StrandPair s{
        "GCATGCU",
        "GATTACA"
    };

    s = globalOptimalSequence(s);
    REQUIRE(s.first == "GCATG_CU");
    REQUIRE(s.second == "G_ATTACA");
}

