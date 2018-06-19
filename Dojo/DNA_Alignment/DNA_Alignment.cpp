#include "DNA_Alignment.h"

namespace
{
    const SimilarityWeights& defaultWeights()
    {
        static const SimilarityWeights w;
        return w;
    }
}

const std::string SimilarityWeights::baseIdx = { "AGCTU" };
const char SimilarityWeights::space = '_';


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
    const SimilarityWeights& weights = defaultWeights();

	Grid::CellEditFunction zeros = [](size_t, size_t, Grid::Cell* cell) { cell->score = 0; };
		
	// Score the alignment of the two sequences, working from top left
	// to bottom right accumulating the scores from previous cells
    Grid::CellEditFunction scoreFunc = [&](const size_t row, const size_t col, Grid::Cell* target)
	{
		// Take the max score of
        // * left cell
        // * top cell
        // * topleft cell, and in addition +1 to that score if the topleft cell values are equal
        auto left = grid.idx(row, col - 1),
             top = grid.idx(row - 1, col),
             topleft = grid.idx(row - 1, col - 1);

        auto bonus = weights(target->val1, target->val2);

        if (topleft->score + bonus >= left->score && topleft->score + bonus >= top->score)
        {
            target->prev = topleft;
            target->score = topleft->score + bonus;
        }
        else
        {
            target->prev = (left->score >= top->score) ? left : top;
            target->score = target->prev->score;
        }
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
    const SimilarityWeights& weights = defaultWeights();

	// Initial scores should be -2 increments in row 0 and col 0
	Grid::CellEditFunction spaces = [&](size_t r, size_t c, Grid::Cell* cell) {
		if (r == 0 && c != 0)
            cell->score = c * weights.indel;
        else if (r != 0 && c == 0)
            cell->score = r * weights.indel;
        else
            cell->score = 0;
	};

    Grid::CellEditFunction scoreFunc = [&](const size_t row, const size_t col, Grid::Cell* target)
	{
		if (target->val1 == target->val2)
		{
			target->prev = grid.idx(row - 1, col - 1); // top left from current cell
		}
		else			
		{			
			auto left = grid.idx(row - 1, col);
			auto top = grid.idx(row, col - 1);
			target->prev = (left->score > top->score) ? left : top;								
		}
        int score = weights(target->val1, target->val2);
        target->score = target->prev->score + score;		
	};

    grid.initialize_function = spaces;
    grid.scoring_function = scoreFunc;

    return grid.align();
}


