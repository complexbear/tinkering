#define CATCH_CONFIG_MAIN
/*
 * A common task in DNA analysis is to align two fragments of DNA sequences against each other.
 * This lends itself to a Dynamic Programming style of solution where we are interested in
 * identifying the longest common subsequences of DNA bases.
 *
 * Inspired by IBM's DP articles here: http://ibm.com/developerworks/library/j-seqalign
 */
#include <catch.hpp>
#include "Grid.h"

static const SimilarityWeights defaultWeights;

Grid::StrandPair longestCommonSequence(const Grid::StrandPair& s)
{
	/* Form an MxN matrix where M = length of s1, N = length of s2
	   This will hold the score as we try to align the sequences (s1, s2)

	        s1 ----> M
	 s2     ---G-----A-----T--------
	  |  A  |  0  |  2  |  2 
	  |  |  |------------------------
	  |  T  |  0  |  0  |  3
	  N     |
	*/
    Grid grid(s);

	Grid::CellEditFunction zeros = [](size_t, size_t, Grid::Cell* cell) { cell->score = 0; };
		
	// Score the alignment of the two sequences, working from top left
	// to bottom right accumulating the scores from previous cells
    Grid::CellEditFunction scoreFunc = [&](const size_t row, const size_t col, Grid::Cell* target)
	{
		// If the character in s1 and s2 at these indices are the same then +1 to the previous score (top left cell from this position)
		// Otherwise take the max score from the prev calcs
        if (s.first[row] == s.second[col])
		{
			auto topleft = grid.idx(row - 1, col - 1);
			target->prev = topleft;
		}
		else
		{
			auto left = grid.idx(row - 1, col);
			auto top = grid.idx(row, col - 1);
			auto prev = (left->score > top->score) ? left : top;
			target->prev = prev;
		}
        int score = defaultWeights(s.first[row], s.second[col]);
        target->score = target->prev->score + score;
	};

    grid.initialize_function = zeros;
    grid.scoring_function = scoreFunc;
    return grid.align();
}


/*
 * Needleman - Wunsch algorithm
 * For global optimal string alignment, with scoring that penalises gaps in the sequences.
 * 
 * Scoring:
 * Match = +1, Mismatch = -1, Gap = -2
 */
Grid::StrandPair globalOptimalSequence(const Grid::StrandPair& s)
{    
    Grid grid(s);

	// Initial scores should be -2 increments in row 0 and col 0
	Grid::CellEditFunction spaces = [](size_t r, size_t c, Grid::Cell* cell) {
		if (r == 0 && c != 0)
            cell->score = c * defaultWeights.indel;
        else if (r != 0 && c == 0)
            cell->score = r * defaultWeights.indel;
        else
            cell->score = 0;
	};

    Grid::CellEditFunction scoreFunc = [&](const size_t row, const size_t col, Grid::Cell* target)
	{
		if (s.first[row] == s.second[col])
		{
			target->prev = grid.idx(row - 1, col - 1); // top left from current cell
		}
		else			
		{			
			auto left = grid.idx(row - 1, col);
			auto top = grid.idx(row, col - 1);
			target->prev = (left->score > top->score) ? left : top;								
		}
        int score = defaultWeights(s.first[row], s.second[col]);
        target->score = target->prev->score + score;		
	};

    grid.initialize_function = spaces;
    grid.scoring_function = scoreFunc;

    return grid.align();
}


TEST_CASE("LongestCommonSequence", "")
{
    Grid::StrandPair s{ "GAGGTCAG", "GATGCTAG" };

	s = longestCommonSequence(s);
	REQUIRE(s.first == "GA_G__AG");
    REQUIRE(s.second == "GA_G__AG");
}

TEST_CASE("GlobalOptimal", "")
{
    Grid::StrandPair s{ "GCATGCU", "GATTACA" };

	s = globalOptimalSequence(s);
	REQUIRE(s.first == "GCATG_CU");
    REQUIRE(s.second == "G_ATTACA");
}

