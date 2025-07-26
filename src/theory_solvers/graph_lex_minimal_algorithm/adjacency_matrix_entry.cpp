#include "adjacency_matrix_entry.hpp"

char value_char(AdjacencyMatrixEntry e) {
    switch (e) {
        case AdjacencyMatrixEntry::Zero:
            return '0';
        case AdjacencyMatrixEntry::One:
            return '1';
        case AdjacencyMatrixEntry::Star:
            return '*';
        default:
            return '-';
    }
}

