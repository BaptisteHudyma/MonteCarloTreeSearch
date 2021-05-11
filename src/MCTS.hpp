#ifndef MCTS_MCTS_TREE_CLASS_HPP
#define MCTS_MCTS_TREE_CLASS_HPP

#include "game_state.hpp"
#include "node.hpp"

/**
 * \file   MCTS.hpp
 * \author Baptiste HUDYMA
 * \version 1.0
 * \date 11 mars 2021
 * 
 * \brief Describe the Monte Carlo Tree Search class
 */


namespace MCTS {

    /**
     *
     *
     *
     *
     */
    class MCTS {
        public:

            /**
             *
             */
            MCTS(IGame_State* initialGameState); 

            /**
             * \brief Search for the action that maximises the tree score
             *
             * \param[in] timeLimit Maximum search time (in seconds)
             *
             * \return Index of the best action, corresponding to an index in initialGameState
             */
            unsigned int search_best_move(unsigned int iterations);
            
            /**
              *
              *
              *
              */
            void show_tree(unsigned int maxDepth);
            void show_best_path(unsigned int maxDepth);
            void show_best_moves(unsigned int maxDepth);


            ~MCTS();


        protected:

            /**
              * \brief go down the tree by picking up the best path based on UCT until a leaf is reached
              *
              * \return A leaf Node
              */
            Node* get_UCT_leaf();

        private:
            Node* _root;

    };



} /* MCTS */

#endif
