#include "node.hpp"

#include <cmath>
#include <iostream>

namespace MCTS {

/**
 * \file    Tree_Node.cpp
 * \author  Baptiste Hudyma
 * \version 1.0
 * \date    11 mars 2021
 *
 * \brief   Define the Node class functions used in the Tree
 */


// Basic constructor for the tree root (no parent)
Node::Node (IGame_State* gameState) : Node(nullptr, gameState) {
    _moveIndex = 0;
}

/**
 * \fn ~Node ();
 * \brief   Destructor
 */
Node::~Node () {
    //call children destructor

    //call unexplored children destructor
    //this.unexploredChildren;
}

/**
 * \brief   Basic constructor
 *
 * \param[in] parent    Parent node reference
 * \param[in] gameState IGame_State object, created by the parent node
 */
Node::Node(Node* parent, IGame_State* gameState) {

    if(parent == nullptr)
        std::cout << "Node do not have a parent" << std::endl;
    if(gameState == nullptr)
        std::cout << "Node cannot have empty game state" << std::endl;

    _parent = parent;

    _state = gameState;
    _moveIndex = 0;

    //_nodeHasMoves = (_state->get_move_count() <= 0);  //Node if a leaf 
    //_closedChildrenCount = 0;  

    //reserve space for children and unexplored node tracking
    //_children.reserve(this->childCount);
    //_unexploredChildrenCount = _childCount;
    //for(int i = 0; i < _childCount; i++)
    //    _unexploredChildren.insert (std::pair<int, bool>(i, false) );

    _visitCount = 0;
    _rewardValue = 0.0;
}

/**
 * \fn bool is_fully_expanded ()
 * \brief  Check if all children of this node are already explored
 *
 * \return  Boolean: True if all children are explored
 */
bool Node::is_fully_expanded () {
    return _children.size() >= _state->get_move_count();
}

/**
 * \fn Node* get_best_child ();
 * \brief  Return the child with the highest UCB1
 *
 * \return  The child Node object with the highest UCB1
 */
Node* Node::get_best_child_UCB () {
    if(_state->get_move_count() <= 0) {
        return nullptr;
    }

    Node* bestChild = nullptr;
    float bestUCB1 = -100;
    for(Node* child : _children) 
    {
        if(child->get_UCB1() > bestUCB1) {
            bestChild = child;
            bestUCB1 = child->get_UCB1();
        }
    }
    return bestChild;
}

/**
 * \fn Node* get_best_child_UCT ();
 * \brief  Return the child with the highest UCT, discarding those which are fully explored
 *
 * \return  The child Node object with the highest UCT
 */
Node* Node::get_best_child_UCT () {
    if(_state->get_move_count() <= 0) {
        return nullptr;
    }

    Node* bestChild = nullptr;
    float bestUCBT = -100;
    for (Node* child : _children)
    {
        //if (child->nodeHasMoves and child->get_UCT() > bestUCBT) {
        if (child->get_UCT() > bestUCBT) {
            bestChild = child;
            bestUCBT = child->get_UCT();
        }
    }
    return bestChild;
}


/**
 * \brief   Play a full game at random from this game state until a game_over
 *
 * \return  The score of the final node
 */
float Node::rollout () {

    IGame_State* currentRolloutState = _state;
    
    //while the game is not over
    while(not currentRolloutState->is_game_over()) {

        //get an available action from this game state
        unsigned int indexToExecute = rand() % currentRolloutState->get_move_count();    //TODO: true choice

        //make a move
        currentRolloutState = currentRolloutState->do_move(indexToExecute);
    }
    float endScore = currentRolloutState->get_score();;
    std::cout << "score: " << endScore << std::endl;
    return endScore;
}

/**
 * \fn void backpropagate (float reward);
 * \brief   Propagate the results from this node to parents until the root is reached
 * \details Update the \a visitCount, \a rewardValue and eventually \a nodeHasMoves if we exhausted this node children moves. The call is recursive from this node to the root.
 *
 * \param[in] reward    The reward of the leaf, propagated to the root node
 */
void Node::backpropagate (float reward) {
    _visitCount += 1;
    _rewardValue += reward;

    //this node is maybe a leaf, check if all children are leafs and propagate to parent
    /*if(not _nodeHasMoves) {
        _closedChildrenCount += 1; //a calling children was closed
        if(this->is_fully_expanded() and this->closedChildrenCount >= this->childCount) {
            //no more children to explore and closed children >= max children count
            if(this->parent) //force parent to check if it should close
                this->parent->nodeHasMoves = false;
        }
        else
            this->nodeHasMoves = true;  //still some moves to test
    }*/


    if(dynamic_cast<Node*>(_parent) != nullptr) {   //propagate to parent
        _parent->backpropagate(reward);
    }

}


/**
 * \brief   Get the UCB1 score
 * \details  Here, UCB1 is define as the reward value over the number of visits 
 *
 * \return This node UCB1
 */
float Node::get_UCB1() {
    if(_visitCount == 0)
        return 100000;  //infinity
    return _rewardValue / static_cast<float>(_visitCount);
}

/**
 * \brief   Get the UCT score
 * \details This score balances score and exploration to parse the tree
 *
 * \return  The UTC score
 */
float Node::get_UCT() {
    if(static_cast<Node*>(_parent) == nullptr) {
        //parent is null, should be first node
        return this->get_UCB1();
    }
    else if(dynamic_cast<Node*>(_parent->_parent) == nullptr) {
        //parent's parent is null, first be first layer of the tree
        return this->get_UCB1() + 1.4 * sqrt( log(_parent->_visitCount) ) / sqrt(_visitCount);
    }
    else {
        //balance exploration and score
        return 
            this->get_UCB1() + 
            1.4 * sqrt( log(_parent->_visitCount) ) / sqrt(_visitCount) +
            sqrt(_parent->_visitCount) / sqrt(_visitCount);
    }
}

/**
 * \brief   Create a new children from the game state posibilities
 *
 * \return  A new child object
 */
Node* Node::expand_children() {
    if( this->is_fully_expanded() )
        //no more children to add
        return nullptr;

    //choose index in [0, _state->get_move_count()[
    unsigned int randomInt = rand() % this->get_move_count();
    /*
    std::map<int, bool>::iterator itr;
    for(itr = _unexploredChildren.begin(); itr != _unexploredChildren.end(); ++itr) {
        if(not itr->second) {
            //unexplored child
            randomInt = itr->first;
            itr->second = true; //mark as explored
            break;
        }
    }
    std::list<Game_Action*>::iterator it = std::next(_state->get_actions().begin(), randomInt);
    action = *it;
    */
    //create next game state
    IGame_State* newGS = _state->do_move(randomInt);

    //create child node
    Node* child = new Node(this, newGS);
    child->_moveIndex = randomInt;

    _children.push_back(child);

    return child;
}

//return a string stream representing this node
std::string Node::to_string() {
    std::ostringstream thisNode;
    thisNode << "[ UCB " << this->get_UCB1();

    //mark this path closed
    /*if(not _nodeHasMoves)
        thisNode << " | X ";  
*/
    thisNode << " ]";
    return thisNode.str();
}

void Node::show_node(unsigned int maxDepth, unsigned int indent) {
    for(int i = 0; i < indent; i++) 
        std::cout << "|    ";
    std::cout << this->to_string() << std::endl;

    if(indent + 1 >= maxDepth)
        return;

    for(Node* child : _children) {
        child->show_node(maxDepth, indent + 1);
        std::cout << std::endl;
    }
}


/**
  * \brief Display the best path based on UCB (display heavy)
  *
  * \param[in] maxDepth Max depth of the search
  * \param[in] indent Actual depth of the current node
  */
void Node::show_best_node_to_string(unsigned int maxDepth, unsigned int indent) {
    std::cout << this->to_string() << std::endl;

    if(indent + 1 >= maxDepth)
        return;

    Node* bestChild = this->get_best_child_UCB();

    for(Node* child : _children) {
        if(child == bestChild) {    //display best child's children
            child->show_best_node_to_string(maxDepth, indent + 1);
            std::cout << std::endl;
        }
        else  {   //display child but not its children
            for(int i = 0; i < indent; i++) 
                std::cout << "|    ";
            std::cout << child->to_string() << std::endl;;
        }
    }
}

//return this state move count
unsigned int Node::get_move_count() {
    return _state->get_move_count();
}


unsigned int Node::get_move_index() {
    return _moveIndex;
}




}   /* MCTS*/





