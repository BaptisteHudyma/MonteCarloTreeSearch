#include <iostream>

#include "MCTS.hpp"

#include "tictactoe/tictactoe.hpp"


int main(int argc, char** argv) {

    MCTS::Game_State* test = new MCTS::Game_State();

    MCTS::MCTS monteCarloTreeSearch(test);

    unsigned int bestIndex = monteCarloTreeSearch.search_best_move(1000);

    std::cout << "Best index is " << bestIndex << std::endl;

    monteCarloTreeSearch.show_tree(10);
    std::cout << std::endl;

    monteCarloTreeSearch.show_best_path(10);
    std::cout << std::endl;

    monteCarloTreeSearch.show_best_moves(10);
    return 0;
}
