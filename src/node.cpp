#include "node.hpp"

#include <cmath>
#include <iostream>
#include <memory>

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
    }

    /**
     * \fn ~Node ();
     * \brief   Destructor
     */
    Node::~Node () {
        //call children destructor
        while( not _children.empty() ) {
            delete _children.front();
            _children.pop_front();
        }
        //call state destructor
        delete _state;
    }

    /**
     * \brief   Basic constructor
     *
     * \param[in] parent    Parent node reference
     * \param[in] gameState IGame_State object, created by the parent node
     */
    Node::Node(Node* parent, IGame_State* gameState) {

        if(parent == nullptr)
            std::cerr << "Node do not have a parent" << std::endl;
        if(gameState == nullptr)
            std::cerr << "Node cannot have empty game state" << std::endl;

        _parent = parent;

        _state = gameState;
        _moveIndex = 0;

        _isClosed = this->is_game_over(); 
        _closedChildrenCount = 0;

        _visitCount = 0;
        _rewardValue = 0.0;

        //reserve space for children and unexplored node tracking
        _unexploredChildren.reserve(_state->get_move_count());
        for(unsigned int i = 0; i < _state->get_move_count(); ++i)
            _unexploredChildren.push_back(i);
    }

    /**
     * \fn bool is_fully_expanded ()
     * \brief  Check if all children of this node are already explored
     *
     * \return  Boolean: True if all children are explored
     */
    bool Node::is_fully_expanded () const {
        return _unexploredChildren.size() <= 0; 
    }

    /**
     * \brief Check if all this child's children were explored
     * 
     * \return True if all children have been fully explored
     */
    bool Node::is_closed() const {
        return _isClosed;
    }

    /**
     * \fn Node* get_best_child ();
     * \brief  Return the child with the highest UCB1
     *
     * \return  The child Node object with the highest UCB1
     */
    Node* Node::get_best_child () const {
        if(_state->is_game_over()) {
            return nullptr;
        }

        Node* bestChild = nullptr;
        float bestUCB1 = -10000;
        for(Node* child : _children) 
        {
            const float ucb = child->get_UCB1();
            if(ucb > bestUCB1) {
                bestChild = child;
                bestUCB1 = ucb;
            }
        }
        return bestChild;
    }

    /**
     * \fn Node* get_best_child ();
     * \brief  Return the child with the highest UCB1
     *
     * \return  The child Node object with the highest UCB1
     */
    Node* Node::get_best_child_UCB () const {
        if(_state->is_game_over()) {
            return nullptr;
        }

        Node* bestChild = nullptr;
        float bestUCB1 = -10000;
        for(Node* child : _children) 
        {
            if(child->is_closed())
                continue;   //do not select already explored child for exploration

            const float ucb = child->get_UCB1();
            if(ucb > bestUCB1) {
                bestChild = child;
                bestUCB1 = ucb;
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
    Node* Node::get_best_child_UCT () const {
        if(_state->is_game_over()) {
            return nullptr;
        }

        Node* bestChild = nullptr;
        float bestUCBT = -10000;
        for (Node* child : _children)
        {
            if(child->is_closed())
                continue;   //do not select already explored child for exploration

            float uct = child->get_UCT();
            if (uct > bestUCBT) {
                bestChild = child;
                bestUCBT = uct;
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
        if(_state->is_game_over()) {
            return _state->get_score();
        }

        unsigned int indexToExecute = rand() % _state->get_move_count();
        std::unique_ptr<IGame_State> currentRolloutState(_state->do_move(indexToExecute));

        //while the game is not over
        while(not currentRolloutState->is_game_over()) {
            //get an available action from this game state
            unsigned int indexToExecute = rand() % currentRolloutState->get_move_count();

            //make a move, swap values and delete current state
            currentRolloutState = std::unique_ptr<IGame_State>(currentRolloutState->do_move(indexToExecute));
        }

        return currentRolloutState->get_score();
    }

    void Node::rollout_expand () {
        Node* currentNode = this;

        //while the game is not over
        while(not currentNode->is_game_over()) {
            //make a move
            currentNode = currentNode->expand_children();
        }

        float endScore = currentNode->_state->get_score();
        currentNode->backpropagate(endScore);
    }




    /**
     * \fn void backpropagate (float reward);
     * \brief   Propagate the results from this node to parents until the root is reached
     * \details Update the \a _visitCount, \a _rewardValue and eventually \a _isClosed if we exhausted this node children moves. The call is recursive from this node to the root.
     *
     * \param[in] reward    The reward of the leaf, propagated to the root node
     */
    void Node::backpropagate (float reward) {
        _visitCount += 1;
        _rewardValue += reward;

        //this node is maybe a leaf, check if all children are leafs and propagate to parent
        if(_isClosed) {
            _closedChildrenCount += 1; //a calling children was closed
            if((this->is_fully_expanded() and _closedChildrenCount >= _children.size()) or is_game_over()) {
                //no more children to explore and closed children >= max children count
                if(_parent != nullptr) { //force parent to check if it should close
                    _parent->_isClosed = true;
                    _parent->backpropagate(reward);
                }
                return;
            }
            else
                _isClosed = false;  //still some moves to test
        }


        if(_parent != nullptr) {   //propagate to parent
            _parent->backpropagate(reward);
        }
    }


    /**
     * \brief   Get the UCB1 score
     * \details  Here, UCB1 is define as the reward value over the number of visits 
     *
     * \return This node UCB1
     */
    float Node::get_UCB1() const {
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
    float Node::get_UCT() const {
        if(_parent == nullptr or _visitCount <= 0) {
            //parent is null, should be first node
            return this->get_UCB1();
        }
        //else if(_parent->_parent == nullptr) {
        //parent's parent is null, first be first layer of the tree
        return this->get_UCB1() + EXPLORATION_SCORE * sqrt( log(_parent->_visitCount) ) / sqrt(_visitCount);
        /*}
          else {
        //balance exploration and score
        return 
        this->get_UCB1() + 
        EXPLORATION_SCORE * sqrt( log(_parent->_visitCount) ) / sqrt(_visitCount) +
        sqrt(_parent->_parent->_visitCount) / sqrt(_visitCount);
        }*/
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
        unsigned int randomInt = rand() % _unexploredChildren.size();

        //create next game state
        unsigned int indexToChoose = _unexploredChildren[randomInt];
        IGame_State* newGS = _state->do_move(indexToChoose);

        //remove selected element
        //std::iter_swap(_unexploredChildren.end() - 1, _unexploredChildren.begin() + randomInt);
        //_unexploredChildren.resize(_unexploredChildren.size() - 1);

        _unexploredChildren.erase(_unexploredChildren.begin() + randomInt);

        //create child node
        Node* child = new Node(this, newGS);
        child->_moveIndex = indexToChoose;

        _children.push_back(child);

        return child;
    }

    std::ostream& operator<<(std::ostream& os, const Node* node) {
        os << "[ UCB " << node->get_UCB1()
            << " R/V: " << node->_rewardValue << "/" << node->_visitCount
            << " U " << node->_unexploredChildren.size()
            << "  " << node->_moveIndex;

        //mark this path closed
        if(node->is_closed())
            os << " | X ";  

        os << " ]";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Node& node) {
        return operator<<(os, &node);
    }


    void Node::show_node(unsigned int maxDepth, unsigned int indent) const {
        for(unsigned int i = 0; i < indent; ++i) 
            std::cout << "|    ";
        std::cout << this << std::endl;

        if(indent + 1 >= maxDepth)
            return;

        for(Node* child : _children) {
            child->show_node(maxDepth, indent + 1);
        }
    }


    /**
     * \brief Display the best path based on UCB (display heavy)
     *
     * \param[in] maxDepth Max depth of the search
     * \param[in] indent Actual depth of the current node
     */
    void Node::show_best_node(unsigned int maxDepth, unsigned int indent) const {
        std::string indentStr = "";
        for(unsigned int i = 0; i < indent; ++i) 
            indentStr += "|\t";
        std::cout << indentStr << this << std::endl;

        if(indent + 1 >= maxDepth)
            return;

        Node* bestChild = this->get_best_child_UCB();

        indentStr += "|\t";
        for(Node* child : _children) {
            if(child == bestChild) {    //display best child's children
                child->show_best_node(maxDepth, indent + 1);
            }
            else  {   //display child but not its children
                std::cout << indentStr << *child << std::endl;;
            }
        }
    }

    void Node::show_best_moves(unsigned int maxDepth, unsigned int level) const {
        std::cout << _state << std::endl;

        if(level + 1 >= maxDepth)
            return;

        Node* bestChild = this->get_best_child_UCB();
        for(Node* child : _children) {
            if(child == bestChild) {    //display best child's children
                child->show_best_moves(maxDepth, level + 1);
                return;
            }
        }
    }

    //return this state move count
    unsigned int Node::get_move_count() const {
        return _state->get_move_count();
    }


    unsigned int Node::get_move_index() const {
        return _moveIndex;
    }

    bool Node::is_game_over() const {
        return _state->is_game_over();
    }

    unsigned int Node::get_visit_count() const {
        return _visitCount;
    }
    float Node::get_score() const {
        return _rewardValue;
    }








}   /* MCTS*/





