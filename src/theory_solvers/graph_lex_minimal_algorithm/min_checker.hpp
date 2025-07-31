#ifndef __MIN_CHECKER_HPP__
#define __MIN_CHECKER_HPP__

#include <optional>

#include "permutation.hpp"
#include "adjacency_matrix.hpp"
#include "general_ordered_partition.hpp"
#include "edge_literal.hpp"

struct MinCheckReturnValue {
    Permutation p;
    std::pair<unsigned, unsigned> indicator_pair;
};

class MinChecker {
    public:
        static std::optional<MinCheckReturnValue> check_minimality(const AdjacencyMatrix& graph_matrix);
    public:
        MinChecker(const AdjacencyMatrix& g);
    private:
        std::optional<MinCheckReturnValue> check_minimality();
        std::optional<MinCheckReturnValue> min_check(const GeneralOrderedPartition& p, unsigned r);
        std::optional<MinCheckReturnValue> try_vertex(unsigned v, const GeneralOrderedPartition& p, unsigned r);
    private:
        const AdjacencyMatrix _g;
        const std::size_t _n;
        GeneralOrderedPartition _p;
};

bool is_critical(const std::pair<unsigned, unsigned>& vertex_pair, const AdjacencyMatrix& G, const Permutation& p);
bool is_indicator_pair(const std::pair<unsigned, unsigned>& vertex_pair, const AdjacencyMatrix& G, const Permutation& p);
std::vector<EdgeLiteral> create_clause(const AdjacencyMatrix& graph, const Permutation& permutation, const std::pair<unsigned, unsigned>& indicator_pair);

#endif

