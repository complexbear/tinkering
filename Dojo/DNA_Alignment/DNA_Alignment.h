#pragma once
/*
* A common task in DNA analysis is to align two fragments of DNA sequences against each other.
* This lends itself to a Dynamic Programming style of solution where we are interested in
* identifying the longest common subsequences of DNA bases.
*
* Inspired by IBM's DP articles here: http://ibm.com/developerworks/library/j-seqalign
*/
#include "Grid.h"

Grid::StrandPair longestCommonSequence(const Grid::StrandPair& s);

/*
* Needleman - Wunsch algorithm
* For global optimal string alignment, with scoring that penalises gaps in the sequences.
*
* Scoring:
* Match = +1, Mismatch = -1, Gap = -2
*/
Grid::StrandPair globalOptimalSequence(const Grid::StrandPair& s);

