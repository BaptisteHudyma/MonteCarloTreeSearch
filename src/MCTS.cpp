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
    unsigned int MCTS::search_best_move(unsigned int iterations) {
        //at least one iteration
        do {
            Node* currentNode = this->get_UCT_leaf();
            if(currentNode != nullptr) {
                float endScore = currentNode->rollout();
                currentNode->backpropagate(endScore);
                //currentNode->rollout_expand();
            }
            else {
                //reached a closed node
                //break;
            }
            iterations -= 1;
        } while (iterations != 0 and not _root->is_closed());
        //while first node is not closed

        if(iterations <= 0)
            std::cout << "Parsed until the end" << std::endl;
        
        //return index with best UCB
       Node* bestChild = this->get_best_move();
        if(bestChild == nullptr) {
            std::cerr << "Best child of root is null" << std::endl;
            return 0;
        }
        return bestChild->get_move_index();
    }


    Node* MCTS::get_UCT_leaf() {
        Node* currentNode = _root;
        while(not currentNode->is_game_over()) {
            //while node not an end game leaf
            
            if(not currentNode->is_fully_expanded()) { 
                //at least a move can be made here, do it
                return currentNode->expand_children();
            }
            currentNode = currentNode->get_best_child_UCT();
        }
        //never reached an end node, should never happen 
        return nullptr;
    }


    Node* MCTS::get_best_move() {
        return _root->get_best_child();
    }

    void MCTS::show_tree(unsigned int maxDepth) {
        _root->show_node(maxDepth, 0);
    }
    void MCTS::show_best_path(unsigned int maxDepth) {
        _root->show_best_node(maxDepth, 0);
    }
    void MCTS::show_best_moves(unsigned int maxDepth) {
        _root->show_best_moves(maxDepth, 0);
    }

    MCTS::~MCTS() {
        delete _root;
    }

};  //MCTS



