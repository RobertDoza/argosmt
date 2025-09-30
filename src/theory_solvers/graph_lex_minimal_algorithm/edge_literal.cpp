#include "edge_literal.hpp"

std::string EdgeLiteral::to_string() const {
    std::string str = "(" + std::to_string(vertex_pair.first) + ", " + std::to_string(vertex_pair.second) + ")";
    if (sign == Sign::Negative) {
        str = "~" + str;
    }
    return str;
}

std::ostream& operator<<(std::ostream& out, const EdgeLiteral& edge_literal) {
    out << edge_literal.to_string();
    return out;
}

bool operator==(const EdgeLiteral& lhs, const EdgeLiteral& rhs) {
    return lhs.sign == rhs.sign && lhs.vertex_pair == rhs.vertex_pair;
}

bool operator!=(const EdgeLiteral& lhs, const EdgeLiteral& rhs) {
    return !(lhs == rhs);
}
