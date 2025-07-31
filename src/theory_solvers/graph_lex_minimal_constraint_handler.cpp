#include "graph_lex_minimal_constraint_handler.hpp"

#define GRAPH_LEX_MIN_LOG

namespace {
#ifdef GRAPH_LEX_MIN_LOG
    void log_message(const std::string& msg) {
        std::cout << msg << std::endl;
    }
#endif // GRAPH_LEX_MIN_LOG
} // namespace

graph_lex_minimal_constraint_handler::graph_lex_minimal_constraint_handler(csp_theory_solver* th, const expression& l_pos, const expression& l_neg)
    : constraint_handler(th, l_pos, l_neg), _common_data(&th->_graph_lex_minimal_common_data), _next_to_assert(0),
      _adjacency_matrix(parse_graph_lex_minimal_symbol_string(l_pos->get_symbol().to_string())),
      _literal_to_vertex_pair_map(l_pos->get_operands()) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler constructor called");
    log_message(_adjacency_matrix.to_string());
#endif // GRAPH_LEX_MIN_LOG
    _common_data->_instance_count++;
    const expression_vector& ops = _constraint->get_operands();
    csp_theory_solver::csp_theory_solver_data* cons_data = _theory_solver->get_theory_solver_data(_constraint);
    for (unsigned i = 0; i < ops.size(); i++) {
        csp_theory_solver::csp_theory_solver_data* var_data = _theory_solver->add_variable(ops[i]);
        var_data->add_variable_constraint(_constraint);
        cons_data->add_constraint_variable(ops[i]);
    }

    for (unsigned i = 0; i < th->_current_level; i++)
        _trail.new_level();

#ifdef GRAPH_LEX_MIN_LOG
    log_message(_literal_to_vertex_pair_map.to_string());
#endif // GRAPH_LEX_MIN_LOG
}

void graph_lex_minimal_constraint_handler::new_level() {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler new_level() called");
#endif // GRAPH_LEX_MIN_LOG
    _trail.new_level();
    // TODO
}

void graph_lex_minimal_constraint_handler::backjump(unsigned level) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler backjump() called");
#endif // GRAPH_LEX_MIN_LOG

#ifdef GRAPH_LEX_MIN_LOG
    std::stringstream message_before_backjump;
    message_before_backjump << "Before backjump: " << std::endl;
    _trail.out(message_before_backjump);
    message_before_backjump << std::endl;
    log_message(message_before_backjump.str());
#endif // GRAPH_LEX_MIN_LOG

    assert(level < _trail.current_level());
    _trail.backjump(level);
    _next_to_assert = _trail.size();
    // TODO

#ifdef GRAPH_LEX_MIN_LOG
    std::stringstream message_after_backjump;
    message_after_backjump << "After backjump: " << std::endl;
    _trail.out(message_after_backjump);
    message_after_backjump << std::endl;
    log_message(message_after_backjump.str());
#endif // GRAPH_LEX_MIN_LOG
}

void graph_lex_minimal_constraint_handler::assert_literal(const expression& l) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler assert_literal() called");
#endif // GRAPH_LEX_MIN_LOG
    if (_theory_solver->get_solver().is_conflict())
        return;

    if (_theory_solver->get_solver().get_trail().get_source_theory_solver(l) != _theory_solver ||
        _theory_solver->get_theory_solver_data(l)->get_explanation_handler() != this) {
        _trail.push(l);
    }
}

void graph_lex_minimal_constraint_handler::check_and_propagate(unsigned layer) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler check_and_propagate() called");
#endif // GRAPH_LEX_MIN_LOG
    if (_theory_solver->get_solver().is_conflict()) {
        return;
    }

    extended_boolean eb = _theory_solver->get_solver().get_trail().get_value(_constraint);
    if (eb == EB_UNDEFINED)
        return; // never happens

    if (_just_activated) {
        if (eb == EB_TRUE)
            propagate_weaker_from_pos_constraints();
        else
            propagate_weaker_from_neg_constraints();
        _just_activated = false;
    }

    if (eb == EB_FALSE) {
        return;
    }

    // TODO
}

void graph_lex_minimal_constraint_handler::explain_literal(const expression& l) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler explain_literal() called");
#endif // GRAPH_LEX_MIN_LOG
       // TODO
}

int graph_lex_minimal_constraint_handler::get_variable_value_hint(const expression& var) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler get_variable_value_hint() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
    return 0;
}

bool graph_lex_minimal_constraint_handler::is_weaker_from_pos(const expression& c) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler is_weaker_from_pos() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
    return false;
}

bool graph_lex_minimal_constraint_handler::is_weaker_from_neg(const expression& c) {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler is_weaker_from_neg() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
    return false;
}

void graph_lex_minimal_constraint_handler::check_implied() {
#ifdef GRAPH_LEX_MIN_LOG
    log_message("LexMin constraint handler check_implied() called");
#endif // GRAPH_LEX_MIN_LOG
    // DO NOTHING
}

void graph_lex_minimal_constraint_handler::handle_edge_literal(const expression& l) {
    auto symbol = l->get_symbol();
    auto operands = l->get_operands();
    auto edge_i_j = operands[0];
    auto [i, j] = _literal_to_vertex_pair_map.get_vertices(edge_i_j);
    i--;
    j--;
    auto value = operands[1]->get_special_constant().get_u_value();

    if (symbol == function_symbol::EQ) {
        if (value == 0) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::Zero);
        } else if (value == 1) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::One);
        } else {
            // TODO: ERROR
        }
        return;
    }

    if (symbol == function_symbol::DISTINCT) {
        if (value == 0) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::One);
        } else if (value == 1) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::Zero);
        } else {
            // TODO: ERROR
        }
        return;
    }

    if (symbol == function_symbol::GE) {
        if (value == 1) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::One);
        } else if (value == 0) {
            // DO NOTHING
        }
        return;
    }

    if (symbol == function_symbol::LE) {
        if (value == 0) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::Zero);
        } else if (value == 1) {
            // DO NOTHING
        }
        return;
    }

    if (symbol == function_symbol::GT) {
        if (value == 0) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::One);
        } else if (value == 1) {
            // TODO: ERROR
        } else {
            // TODO: ERROR
        }
        return;
    }

    if (symbol == function_symbol::LT) {
        if (value == 0) {
            // TODO: ERROR
        } else if (value == 1) {
            _adjacency_matrix.set_entry(i, j, AdjacencyMatrixEntry::Zero);
        } else {
            // TODO: ERROR
        }
        return;
    }
}

unsigned parse_graph_lex_minimal_symbol_string(const std::string& symbol_string) {
    constexpr const char* error_msg = "Invalid format: expected '(_ graph_lex_minimal n)'";

    if (symbol_string.empty() || symbol_string.front() != '(' || symbol_string.back() != ')') {
        throw std::invalid_argument(error_msg);
    }

    std::string inner = symbol_string.substr(1, symbol_string.size() - 2);

    std::istringstream iss(inner);
    std::string underscore, keyword, number_str;

    if (!(iss >> underscore >> keyword >> number_str)) {
        throw std::invalid_argument(error_msg);
    }

    if (underscore != "_" || keyword != "graph_lex_minimal") {
        throw std::invalid_argument(error_msg);
    }

    try {
        return static_cast<unsigned>(std::stoul(number_str));
    } catch (...) {
        throw std::invalid_argument(error_msg);
    }
}

