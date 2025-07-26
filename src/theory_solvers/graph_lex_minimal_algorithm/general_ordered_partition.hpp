#ifndef __GENERAL_ORDERED_PARTITION_HPP__
#define __GENERAL_ORDERED_PARTITION_HPP__

#include <vector>
#include <string>

#include "permutation.hpp"
#include "partition_triple.hpp"

class GeneralOrderedPartition {
    public:
        GeneralOrderedPartition(std::size_t n);
        GeneralOrderedPartition(const std::vector<PartitionTriple>& t);
        std::size_t size() const;
        std::string to_string() const;
        Permutation current_permutation() const;
        PartitionTriple get_triple(std::size_t index) const;
        std::vector<PartitionTriple> get_triples() const;
        void split_triple(std::size_t index, const PartitionTriple& new_triple_1, const PartitionTriple& new_triple_2);
        void split_triple(const PartitionTriple& target_triple, const PartitionTriple& new_triple_1, const PartitionTriple& new_triple_2);
        void check_correctness() const;
    private:
        std::size_t _num_vertices;
        std::vector<PartitionTriple> _triples;
};

#endif