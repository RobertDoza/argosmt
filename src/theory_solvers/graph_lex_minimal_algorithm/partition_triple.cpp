#include <stdexcept>

#include "partition_triple.hpp"

std::string PartitionTriple::to_string() const {
    std::string s;
    s += "({";
    bool first = true;
    for (int v : vertices) {
        if (!first) {
            s += ", ";
        }
        s += std::to_string(v);
        first = false;
    }
    s += "}, " + std::to_string(lower) + ", " + std::to_string(upper) + ")";
    return s;
}

bool PartitionTriple::operator==(const PartitionTriple& other) const {
    return vertices == other.vertices && lower == other.lower && upper == other.upper;
}

void PartitionTriple::check_correctness() const {
    if (vertices.size() != upper - lower + 1) {
        throw std::runtime_error("Incorrect partition triple format");
    }
}

bool PartitionTriple::is_empty() const {
    return vertices.empty();
}

