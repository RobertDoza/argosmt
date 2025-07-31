#ifndef _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H
#define _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H

#include "backjump_stack.hpp"
#include "constraint_handler.hpp"
#include "csp_explanation_minimizer.hpp"
#include "domain_handler.hpp"
#include "hash_vector.hpp"
#include "history_saver.hpp"
#include "adjacency_matrix.hpp"

class LiteralToVertexPairMap {
    public:
        LiteralToVertexPairMap(const expression_vector& expressions)
            :_map(parse_expressions(expressions))
        {}
        std::pair<std::size_t, std::size_t> get_vertices(const expression& expr) const {
            auto it = _map.find(expr);
            if (it == _map.end()) {
                throw std::runtime_error("Expression not found in map");
            }

            return it->second;
        }
        std::string to_string() const {
            std::stringstream sstr;
            for (auto &[expr, pair] : _map) {
                sstr << expr->to_string() << " -> " << pair.first << " " << pair.second << std::endl;
            }
            return sstr.str();
        }
    private:
        static std::map<expression, std::pair<std::size_t, std::size_t>> parse_expressions(const expression_vector& expressions) {
            if (expressions == EMPTY_EXPRESSION_VECTOR) {
                throw std::runtime_error("Empty expression vector");
            }

            std::map<expression, std::pair<std::size_t, std::size_t>> result;
            for (expression expr : expressions) {
                result[expr] = parse_edge_symbol_string(expr->to_string());
            }
            return result;
        }
        static std::pair<std::size_t, std::size_t> parse_edge_symbol_string(const std::string& edge_symbol_string) {
            const std::string prefix = "edge_";
            std::size_t prefix_len = prefix.length();

            if (edge_symbol_string.compare(0, prefix_len, prefix) != 0) {
                throw std::invalid_argument("Invalid format: expected prefix 'edge_'");
            }

            std::size_t sep_pos = edge_symbol_string.find('_', prefix_len);
            if (sep_pos == std::string::npos) {
                throw std::invalid_argument("Invalid format: expected two indices separated by '_'");
            }

            std::string i_str = edge_symbol_string.substr(prefix_len, sep_pos - prefix_len);
            std::string j_str = edge_symbol_string.substr(sep_pos + 1);

            try {
                std::size_t i = std::stoull(i_str);
                std::size_t j = std::stoull(j_str);
                return {i, j};
            } catch (...) {
                throw std::invalid_argument("Invalid format: indices must be non-negative integers");
            }
        }
    private:
        std::map<expression, std::pair<std::size_t, std::size_t>> _map;
};

class graph_lex_minimal_constraint_handler : public constraint_handler {
public:
    graph_lex_minimal_constraint_handler(csp_theory_solver* th, const expression& l_pos, const expression& l_neg);
    virtual void new_level();
    virtual void backjump(unsigned level);
    virtual void assert_literal(const expression& l);
    virtual void check_and_propagate(unsigned layer);
    virtual void explain_literal(const expression& l);
    virtual int get_variable_value_hint(const expression& var);
    virtual bool is_weaker_from_pos(const expression& c);
    virtual bool is_weaker_from_neg(const expression& c);
    virtual void check_implied();
private:
    void handle_edge_literal(const expression& l);
private:
    graph_lex_minimal_common_data* _common_data;
    sparse_backjump_stack<expression> _trail;
    unsigned _next_to_assert;
    AdjacencyMatrix _adjacency_matrix;
    const LiteralToVertexPairMap _literal_to_vertex_pair_map;
};

unsigned parse_graph_lex_minimal_symbol_string(const std::string& symbol_string);

#endif // _GRAPH_LEX_MINIMAL_CONSTRAINT_HANDLER_H

