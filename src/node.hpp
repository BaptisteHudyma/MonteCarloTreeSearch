#ifndef MCTS_NODE_CLASS_HPP
#define MCTS_NODE_CLASS_HPP

#include "game_state.hpp"

#include <list>
#include <sstream>
#include <vector>
#include <map>

/**
 * \file    node.hpp
 * \author  Baptiste Hudyma
 * \version 1.0
 * \date    11 mars 2021
 *
 * \brief   Define the node class used in the Tree
 * \details Define the basic node classe to use in the tree. Contains the search metrics and heuristics.
 */

namespace MCTS {

/*
 *
 *
 *
 *
 */
class Node {
    public:
        /**
         * \brief   Basic constructor for the tree root (no parent)
         *
         * \param[in] gameState IGame_State object, created by the parent node
         */
        Node (IGame_State* gameState);

        /**
         * \brief  Check if all children of this node are already explored
         *
         * \return  Boolean: True if all children where explored
         */
        bool is_fully_expanded ();

        /**
         * \brief  Return the child with the highest UCB1
         *
         * \return  The child Node object with the highest UCB1
         */
        Node* get_best_child_UCB ();


        /**
         * \brief  Return the child with the highest UCT, discarding those which are fully explored
         *
         * \return  The child Node object with the highest UCT
         */
        Node* get_best_child_UCT (); 

        /**
         * \brief   Play a full game at random from this game state until a game_over
         *
         * \return  The score of the final node
         */
        float rollout ();

        /**
         * \brief   Propagate the results from this node to parents until the root is reached
         * \details Update the \a visitCount, \a rewardValue and eventually \a nodeHasMoves if we exhausted this node children moves. The call is recursive from this node to the root.
         *
         * \param[in] reward    The reward of the leaf, propagated to the root node
         */
        void backpropagate (float reward);


        /**
         * \brief   Create a new children from the game state posibilities
         *
         * \return  A new child object
         */
        Node* expand_children();

        /**
          * \brief  Return the index of this node game state
          *
          * \return The index of the move contained in this node
          */
        unsigned int get_move_index();


        /**
         * \fn ~Node ();
         * \brief   Destructor
         */
        ~Node ();
        

        //return this state move count
        unsigned int get_move_count();

    protected:
        /**
         * \brief   Basic constructor
         *
         * \param[in] parent    Parent node reference
         * \param[in] gameState IGame_State object, created by the parent node
         */
        Node(Node* parent, IGame_State* gameState);

        /**
         * \brief   Get the UCB1 score
         * \details  Here, UCB1 is define as the reward value over the number of visits 
         *
         * \return This node UCB1
         */
        float get_UCB1();

        /**
         * \brief   Get the UCT score
         * \details This score balances score and exploration to parse the tree
         *
         * \return  The UTC score
         */
        float get_UCT();

        /**
         * \brief   Return this node as a stringstream
         */
        std::string to_string();

        /**
         * \brief    Show this tree in cout (Can be very expansive for large trees)
         * 
         * \param[in] maxDepth  The maximum depth at which to parse the tree
         * \param[in] index     The current level of the tree
         */
        void show_node(unsigned int maxDepth, unsigned int indent);

        /**
         * \brief   Show this tree in cout (Can be very expansive for large trees)
         * \details This function only develop the best path (based on UCB1)
         * 
         * \param[in] maxDepth  The maximum depth at which to parse the tree
         * \param[in] index     The current level of the tree
         */
        void show_best_node_to_string(unsigned int maxDepth, unsigned int indent);


        //friend ostream& operator<<(ostream& os, const Node& n);

    private:
        Node* _parent;       //reference to parent
        IGame_State* _state;         //This node associated IGame_State object
        std::list<Node*> _children;    //Children of this node

        unsigned int _visitCount;   //Child visits sum
        float _rewardValue;         //Child reward sum
        unsigned int _moveIndex;    //_state index

        //bool _nodeHasMoves;          //True while this node have unexplored children

        //unsigned int _childCount;
        //unsigned int _unexploredChildrenCount;
        //unsigned int _closedChildrenCount;       //count of the node's children which are closed
        //std::map<int, bool> _unexploredChildren; //map <index: explored flag>


};

} /* MCTS */

#endif
