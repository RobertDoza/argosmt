#ifndef __PARTITION_TRIPLE_HPP__
#define __PARTITION_TRIPLE_HPP__

#include <unordered_set>
#include <string>

struct PartitionTriple {
    std::unordered_set<unsigned> vertices;
    std::size_t lower;
    std::size_t upper;

    std::string to_string() const;
    bool operator==(const PartitionTriple& other) const;
    void check_correctness() const;
    bool is_empty() const;
    friend std::ostream& operator<<(std::ostream& out, const PartitionTriple& triple);
};

#endif

