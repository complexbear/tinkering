#include "DNA_Alignment.h"
#include "GridOperations.h"

const std::string SimilarityWeights::baseIdx = { "AGCTU" };
const char SimilarityWeights::space = '_';


StrandPair longestCommonSequence(const StrandPair& s)
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
    const SimilarityWeights weights;

	CellEditFunction zeros = [](Grid&, Cell* cell) { cell->score = 0; };
		
	// Score the alignment of the two sequences, working from top left
	// to bottom right accumulating the scores from previous cells
    CellEditFunction scoreFunc = [&weights](Grid& grid, Cell* target)
	{
        const auto row = target->row;
        const auto col = target->col;

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

    return align(grid, zeros, scoreFunc);
}


/*
 * Needleman - Wunsch algorithm
 * For global optimal string alignment, with scoring that penalises gaps in the sequences.
 * 
 * Scoring:
 * Match = +1, Mismatch = -1, Gap = -2
 */
StrandPair globalOptimalSequence(const StrandPair& s)
{    
    Grid grid(s);
    const SimilarityWeights weights(1, -1, -2);

	// Initial scores should be -2 increments in row 0 and col 0
    // Also initialise the prev cell pointers
	CellEditFunction spaces = [&weights](Grid& grid, Cell* cell) 
    {
        const auto r = cell->row;
        const auto c = cell->col;
        if (r == 0 && c != 0) 
        {
            cell->score = c * weights.indel;
            cell->prev = grid.idx(r, c - 1);
        }
        else if (r != 0 && c == 0)
        {
            cell->score = r * weights.indel;
            cell->prev = grid.idx(r - 1, c);
        }
	};

    CellEditFunction scoreFunc = [&weights](Grid& grid, Cell* target)
	{
        const auto row = target->row;
        const auto col = target->col;

        auto top = grid.idx(row, col - 1),
             left = grid.idx(row - 1, col),
             topleft = grid.idx(row - 1, col - 1);

        auto topScore = top->score + weights.indel,
             leftScore = left->score + weights.indel,
             matchOrMissScore = topleft->score + weights(target->val1, target->val2);
        
        if (matchOrMissScore >= leftScore && matchOrMissScore >= topScore)
        {
            target->prev = topleft;
            target->score = matchOrMissScore;
        }
        else
        {
            if (leftScore >= topScore)
            {
                target->prev = left;
                target->score = leftScore;
            }
            else
            {
                target->prev = top;
                target->score = topScore;
            }
        }
	};
    
    return align(grid, spaces, scoreFunc);
}


