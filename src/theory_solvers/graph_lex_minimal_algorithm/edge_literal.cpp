#include "edge_literal.hpp"

std::string EdgeLiteral::to_string() const {
    std::string str = "(" + std::to_string(vertex_pair.first) + ", " + std::to_string(vertex_pair.second) + ")";
    if (sign == Sign::Negative) {
        str = "~" + str;
    }
    return str;
}

