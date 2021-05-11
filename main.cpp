#include <iostream>

#include "MCTS.hpp"

#include "tictactoe.hpp"


int main(int argc, char** argv) {

    MCTS::Game_State* currentState = new MCTS::Game_State();
    currentState->set_board_at(1, 1);

    while(1) {
        MCTS::MCTS monteCarloTreeSearch(currentState);

        unsigned int bestIndex = monteCarloTreeSearch.search_best_move(1000);
        std::cout << "Best index is " << bestIndex << std::endl;

        currentState = currentState->do_move(bestIndex); 

        //monteCarloTreeSearch.show_best_moves(10);
        monteCarloTreeSearch.show_best_path(10);

        std::cout << currentState << std::endl;
        if(currentState->is_game_over())
            break;

        int nextMoveX = -1;
        std::cin >> nextMoveX;
        int nextMoveY = -1;
        std::cin >> nextMoveY;

        currentState->set_turn(0);
        currentState->set_board_at(nextMoveX, nextMoveY);

        if(currentState->is_game_over()) {
            std::cout << currentState << std::endl;
            break;
        }
    }

    /*
       std::cout << "Best index is " << bestIndex << std::endl;

       monteCarloTreeSearch.show_best_moves(10);

       monteCarloTreeSearch.show_best_path(10);
       std::cout << std::endl;

     */


    return 0;
}
