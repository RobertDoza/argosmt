#ifndef __ADJACENCY_MATRIX_ENTRY_HPP__
#define __ADJACENCY_MATRIX_ENTRY_HPP__

enum class AdjacencyMatrixEntry {
    Zero,
    One,
    Star
};

char value_char(AdjacencyMatrixEntry e);

#endif

