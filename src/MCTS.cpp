#include "MCTS.hpp"

#include <iostream>

namespace MCTS {

    /**
     *
     *
     *
     */
    MCTS::MCTS(IGame_State* initialGameState) {
        _root = new Node(initialGameState);    
    }


    /**
     *
     *
     *
     *
     */
    unsigned int MCTS::search_best_move(double timeLimit) {
        //at least one iteration
        do {
            Node* currentNode = this->get_UCT_leaf();
            if(currentNode == nullptr) {
                std::cout << "get leaf returned null" << std::endl;
                break;
            }

            std::cout << "iteration " << timeLimit  << std::endl;
            float endScore = currentNode->rollout();
            currentNode->backpropagate(endScore);

            timeLimit -= 1.0;
        } while (timeLimit > 0);
        
        //return index with best UCB
        return _root->get_best_child_UCB()->get_move_index();
    }


    Node* MCTS::get_UCT_leaf() {
        Node* currentNode = _root;
        while(currentNode->get_move_count() > 0) {
            //while node not an end game leaf
            
            if(not currentNode->is_fully_expanded()) { 
                //at least a move can be made here, do it
                return currentNode->expand_children();
            }
            currentNode = currentNode->get_best_child_UCB();
        }
        //never reached an end node, should never happen 
        return nullptr;
    }

};


