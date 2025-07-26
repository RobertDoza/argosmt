#include <stdexcept>
#include <set>
#include <iostream>

#include "min_checker.hpp"

namespace {
    template <typename T>
    std::unordered_set<T> create_union(const std::unordered_set<T>& set_1, const std::unordered_set<T>& set_2) {
        std::unordered_set<T> result = set_1;

        for (const auto& elem : set_2) {
            result.insert(elem);
        }

        return result;
    }

    std::string unordered_set_to_string(const std::unordered_set<unsigned>& set) {
        std::string s;
        bool first = true;
        s += "{";
        for (unsigned elem : set) {
            if (!first) {
                s += ", ";
            }
            s += std::to_string(elem);
            first = false;
        }
        s += "}";
        return s;
    }

    std::string set_to_string(const std::set<unsigned>& set) {
        std::string s;
        bool first = true;
        s += "{";
        for (unsigned elem : set) {
            if (!first) {
                s += ", ";
            }
            s += std::to_string(elem);
            first = false;
        }
        s += "}";
        return s;
    }
}

std::optional<MinCheckReturnValue> MinChecker::check_minimality(const AdjacencyMatrix& graph_matrix) {
    MinChecker min_checker(graph_matrix);
    return min_checker.check_minimality();
}

MinChecker::MinChecker(const AdjacencyMatrix& g)
    :_g(g), _n(g.get_dimension()), _p(_n)
{}

std::optional<MinCheckReturnValue> MinChecker::check_minimality() {
    return min_check(_p, 0);
}

std::optional<MinCheckReturnValue> MinChecker::min_check(const GeneralOrderedPartition& p, unsigned r) {
    #ifdef LOG
    log("MinCheck call: P = " + p.to_string() + ", r = " + std::to_string(r) + "\n", r);
    #endif

    if (r == _n - 1) {
        #ifdef LOG
        log("MinCheck over (indicator pair not found)\n", r);
        #endif
        return {};
    }

    std::unordered_set<unsigned> v_r = p.get_triple(r).vertices;
    for (unsigned v : v_r) {
        #ifdef LOG
        log("try v = " + std::to_string(v) + " - begin\n", r + 1);
        #endif
        std::optional<MinCheckReturnValue> ret = try_vertex(v, p, r);
        #ifdef LOG
        log("try v = " + std::to_string(v) + " - end\n", r + 1);
        #endif
        if (ret.has_value()) {
            #ifdef LOG
            log("MinCheck over (indicator pair found)\n", r);
            #endif
            return ret.value();
        }
    }

    #ifdef LOG
    log("MinCheck over (indicator pair not found)\n", r);
    #endif
    return {};
}

std::optional<MinCheckReturnValue> MinChecker::try_vertex(unsigned v, const GeneralOrderedPartition& p, unsigned r) {
    auto p_v = p;

    auto l_r = p.get_triple(r).lower;
    auto u_r = p.get_triple(r).upper;
    auto V_r = p.get_triple(r).vertices;

    auto V_r_without_v = V_r;
    V_r_without_v.erase(v);

    // split V_r
    #ifdef LOG
    log("Splitting V_r into " + PartitionTriple({{v}, l_r, l_r}).to_string() + " & "
    + PartitionTriple{V_r_without_v, l_r + 1, u_r}.to_string() + "\n", r + 1);
    #endif
    p_v.split_triple(r, {{v}, l_r, l_r}, {V_r_without_v, l_r + 1, u_r});
    #ifdef LOG
    log("P_v = " + p_v.to_string() + "\n", r + 1);
    #endif

    // TODO: don't copy entire vector, just the part we need
    std::vector<PartitionTriple> triples_to_split = p_v.get_triples();

    for (std::size_t i = r + 1; i < triples_to_split.size(); i++) {
        auto triple = triples_to_split[i];
        unsigned l_i = triple.lower;
        unsigned u_i = triple.upper;

        #ifdef LOG
        log("current triple: " + triple.to_string() + "\n", r + 1);
        #endif

        std::unordered_set<unsigned> v_i_zero;
        std::unordered_set<unsigned> v_i_one;
        std::unordered_set<unsigned> v_i_star;

        for (unsigned u : triple.vertices) {
            switch (_g.get_entry(v, u)) {
                case AdjacencyMatrixEntry::Zero:
                    v_i_zero.insert(u);
                    break;
                case AdjacencyMatrixEntry::One:
                    v_i_one.insert(u);
                    break;
                case AdjacencyMatrixEntry::Star:
                    v_i_star.insert(u);
                    break;
                default:
                    throw std::runtime_error("Unknown matrix entry");
            }
        }

        #ifdef LOG
        log("ViZero: " + unordered_set_to_string(v_i_zero) + "\n", r + 1);
        log("ViOne: " + unordered_set_to_string(v_i_one) + "\n", r + 1);
        log("ViStar: " + unordered_set_to_string(v_i_star) + "\n", r + 1);
        #endif

        // step 1
        #ifdef LOG
        log("step 1\n", r + 1);
        #endif
        PartitionTriple new_triple_1 = {v_i_zero, l_i, l_i + (unsigned)v_i_zero.size() - 1};
        PartitionTriple new_triple_2 = {create_union<unsigned>(v_i_star, v_i_one), l_i + (unsigned)v_i_zero.size(), u_i};
        #ifdef LOG
        log("Splitting triple " + triple.to_string() + " into " + new_triple_1.to_string() + " & " + new_triple_2.to_string() + "\n", r + 1);
        #endif
        p_v.split_triple(triple, new_triple_1, new_triple_2);
        #ifdef LOG
        log("P_v after step 1 (splitting): " + p_v.to_string() + "\n", r + 1);
        #endif

        // step 2
        #ifdef LOG
        log("step 2\n", r + 1);
        #endif
        std::set<unsigned> J;
        for (unsigned u = l_i; u < l_i + v_i_zero.size(); u++) {
            if (_g.get_entry(r, u) != AdjacencyMatrixEntry::Zero) {
                J.insert(u);
            }
        }
        #ifdef LOG
        log("J = " + set_to_string(J) + "\n", r + 1);
        #endif

        if (!J.empty()) {
            unsigned j = *J.begin();
            #ifdef LOG
            log("j = " + std::to_string(j) + "\n", r + 1);
            #endif
            return {MinCheckReturnValue{p_v.current_permutation(), {r, j}}};
        }

        // step 3
        #ifdef LOG
        log("step 3\n", r + 1);
        #endif
        for (std::size_t p = l_i + v_i_zero.size(); p <= u_i; p++) {
            #ifdef LOG
            log("p = " + std::to_string(p) + "\n", r + 1);
            #endif
            switch (_g.get_entry(r, p)) {
                // case a
                case AdjacencyMatrixEntry::Star: {
                    #ifdef LOG
                    log("A_G[r][p] = *\n", r + 1);
                    #endif
                    if (v == r && v_i_star.find(p) != v_i_star.end()) {
                        auto new_set = v_i_star;
                        new_set.erase(p);
                        new_set = create_union<unsigned>(new_set, v_i_one);
                        auto old_triple = PartitionTriple{create_union<unsigned>(v_i_star, v_i_one), p, u_i};
                        auto new_triple_1 = PartitionTriple{{(unsigned)p}, p, p};
                        auto new_triple_2 = PartitionTriple{new_set, p + 1, u_i};
                        #ifdef LOG
                        log("splitting " + old_triple.to_string() + " into " + new_triple_1.to_string() + " & " + new_triple_2.to_string() + "\n", r + 1);
                        #endif
                        p_v.split_triple(old_triple, new_triple_1, new_triple_2);
                        #ifdef LOG
                        log("P_v after splitting: " + p_v.to_string() + "\n", r + 1);
                        #endif
                        v_i_star.erase(p);
                    } else if (v == p && v_i_star.find(r) != v_i_star.end()) {
                        auto new_set = v_i_star;
                        new_set.erase(r);
                        new_set = create_union<unsigned>(new_set, v_i_one);
                        auto old_triple = PartitionTriple{create_union<unsigned>(v_i_star, v_i_one), p, u_i};
                        auto new_triple_1 = PartitionTriple{{r}, p, p};
                        auto new_triple_2 = PartitionTriple{new_set, p + 1, u_i};
                        #ifdef LOG
                        log("splitting " + old_triple.to_string() + " into " + new_triple_1.to_string() + " & " + new_triple_2.to_string() + "\n", r + 1);
                        #endif
                        p_v.split_triple(old_triple, new_triple_1, new_triple_2);
                        #ifdef LOG
                        log("P_v after splitting: " + p_v.to_string() + "\n", r + 1);
                        #endif
                        v_i_star.erase(r);
                    } else {
                        return {};
                    }
                    break;
                }
                // case b
                case AdjacencyMatrixEntry::Zero:
                    #ifdef LOG
                    log("A_G[r][p] = 0\n", r + 1);
                    log("returning nil.\n", r + 1);
                    #endif
                    return {};
                // case c
                case AdjacencyMatrixEntry::One:
                    #ifdef LOG
                    log("A_G[r][p] = 1\n", r + 1);
                    #endif
                    if (!v_i_star.empty()) {
                        #ifdef LOG
                        log("ViStar is not empty.\n", r + 1);
                        log("splitting triple: " + PartitionTriple{create_union<unsigned>(v_i_star, v_i_one), p, u_i}.to_string() + "\n", r + 1);
                        #endif
                        p_v.split_triple(
                            PartitionTriple{create_union<unsigned>(v_i_star, v_i_one), p, u_i},
                            PartitionTriple{v_i_star, p, p + v_i_star.size() - 1},
                            PartitionTriple{v_i_one, p + v_i_star.size(), u_i}
                        );
                        #ifdef LOG
                        log("P_v after splitting: " + p_v.to_string() + "\n", r + 1);
                        log("returning indicator pair: (" + std::to_string(r) + ", " + std::to_string(p) + ")\n", r + 1);
                        #endif
                        return {MinCheckReturnValue{p_v.current_permutation(), {r, p}}};
                    } else {
                        #ifdef LOG
                        log("ViStar is empty.\n", r + 1);
                        #endif
                        for (auto p_prime = p; p_prime <= u_i; p_prime++) {
                            #ifdef LOG
                            log("p_prime = " + std::to_string(p_prime) + "\n", r + 1);
                            #endif
                            if (_g.get_entry(r, p_prime) != AdjacencyMatrixEntry::One) {
                                #ifdef LOG
                                log("returning nil.\n", r + 1);
                                #endif
                                return {};
                            }
                        }
                    }
                    break;
                default:
                    throw std::runtime_error("Unknown matrix value");
            }
        }
    }

    // TODO: remove me at some point
    p_v.check_correctness();

    return min_check(p_v, r + 1);
}

void MinChecker::log(const std::string& message, unsigned indentation_level) const {
    for (unsigned i = 0; i < indentation_level; i++) {
        std::cout << "|" << "\t";
    }
    std::cout << message << std::flush;
}

bool is_critical(const std::pair<unsigned, unsigned>& vertex_pair, const AdjacencyMatrix& G, const Permutation& p) {
    AdjacencyMatrix p_G = G.permute(p);

    auto i = vertex_pair.first;
    auto j = vertex_pair.second;

    auto G_i_j = G.get_entry(i, j);
    auto p_G_i_j = p_G.get_entry(i, j);

    if (G_i_j == AdjacencyMatrixEntry::One && p_G_i_j == AdjacencyMatrixEntry::Zero) {
        return true;
    }

    if (G_i_j == AdjacencyMatrixEntry::One && p_G_i_j == AdjacencyMatrixEntry::Star) {
        return true;
    }

    if (G_i_j == AdjacencyMatrixEntry::Star && p_G_i_j == AdjacencyMatrixEntry::Zero) {
        return true;
    }

    return false;
}

bool is_indicator_pair(const std::pair<unsigned, unsigned>& vertex_pair, const AdjacencyMatrix& G, const Permutation& p) {
    if (!is_critical(vertex_pair, G, p)) {
        return false;
    }

    AdjacencyMatrix p_G = G.permute(p);

    unsigned i = vertex_pair.first;
    unsigned j = vertex_pair.second;

    for (unsigned i_prime = 0; i_prime < i; i_prime++) {
        for (unsigned j_prime = 0; j_prime < j; j_prime++) {
            if (i_prime == p(i_prime) && j_prime == p(j_prime)) {
                continue;
            }

            if (i_prime == p(j_prime) && j_prime == p(i_prime)) {
                continue;
            }

            if (G.get_entry(i_prime, j_prime) == AdjacencyMatrixEntry::One) {
                continue;
            }

            if (p_G.get_entry(i_prime, j_prime) == AdjacencyMatrixEntry::Zero) {
                continue;
            }

            return false;
        }
    }

    return true;
}

