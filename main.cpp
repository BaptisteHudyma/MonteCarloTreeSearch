#include <iostream>

#include "MCTS.hpp"

#include "tictactoe.hpp"
#include "puissance4.hpp"



void play_tictactoe(bool shouldStart = false) {
    MCTS::Game_State* currentState = new MCTS::Game_State();

    if(shouldStart)
        currentState->set_board_at(0, 1);

    while(1) {
        MCTS::MCTS monteCarloTreeSearch(currentState);

        unsigned int bestIndex = monteCarloTreeSearch.search_best_move(300);
        std::cout << "nodes visited : " << monteCarloTreeSearch.get_visits() << " with a score of " << monteCarloTreeSearch.get_score() << std::endl;

        currentState = currentState->do_move(bestIndex); 

        //monteCarloTreeSearch.show_best_moves(10);
        //monteCarloTreeSearch.show_best_path(10);

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
}

void play_connect4(bool shouldStart = false) {
    MCTS::Puissance4* currentState = new MCTS::Puissance4();
    if(shouldStart)
        currentState->set_board_at(3, 0);

    while(1) {
        MCTS::MCTS monteCarloTreeSearch(currentState);

        unsigned int bestIndex = monteCarloTreeSearch.search_best_move(1000000);
        std::cout << "nodes visited : " << monteCarloTreeSearch.get_visits() << " with a score of " << monteCarloTreeSearch.get_score() << std::endl;
        monteCarloTreeSearch.show_best_path(10);
        monteCarloTreeSearch.show_best_moves(10);

        currentState = currentState->do_move(bestIndex); 

        std::cout << currentState << std::endl;
        if(currentState->is_game_over())
            break;

        int nextMoveX = -1;
        std::cin >> nextMoveX;

        currentState = currentState->do_move(nextMoveX);
        std::cout << currentState << std::endl;

        if(currentState->is_game_over()) {
            std::cout << currentState << std::endl;
            break;
        }
    }
}



int main(int argc, char** argv) {

    play_connect4();
    //play_tictactoe();

    //monteCarloTreeSearch.show_best_moves(10);
    //monteCarloTreeSearch.show_best_path(10);


    return 0;
}
