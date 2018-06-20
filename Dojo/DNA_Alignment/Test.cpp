#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "DNA_Alignment.h"

TEST_CASE("LongestCommonSequence", "")
{
    StrandPair s{
        "GCCCTAGCG",
        "GCGCAATG"            
    };

    s = longestCommonSequence(s); // GCCAG  
    REQUIRE(s.first  == "GC_C_A__G"); 
    REQUIRE(s.second == "GC_CA__G");
}

TEST_CASE("GlobalOptimal", "")
{
    StrandPair s{
        "GCCCTAGCG",
        "GCGCAATG"
    };

    s = globalOptimalSequence(s);
    REQUIRE(s.first == "GC_C_A__G");
    REQUIRE(s.second == "GC_CA__G");
}

