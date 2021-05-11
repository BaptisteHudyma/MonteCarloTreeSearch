#ifndef MCTS_GAME_STATE_CLASS_HPP
#define MCTS_GAME_STATE_CLASS_HPP

#include <list>
#include <sstream>

/**
 * \file    State.hpp
 * \author  Baptiste Hudyma
 * \version 1.0
 * \date    11 mars 2021
 *
 * \brief   Define the classes used by the Tree
 * \details Define the basic classes to use as a base for the Monte Carlo Tree Search.
 Each classes shows the basic methods to overload
 *
 */

namespace MCTS {


/**
 *
 * \brief   Interface representing a game state
 * \details This class handles the possible action generation and the associated score.
 */
class IGame_State {
    public:
        /**
         * \brief   Return the score associated with this game state 
         *
         * \return  A score describing the current game state. The higher the score, the better the action.
         */
        virtual float get_score() = 0;

        /**
         *
         * \return  A boolean True if there is no more actions possible from this game state
         */
        virtual bool is_game_over() = 0;

        /**
         * \brief   Compute the possible actions after this game state 
         *
         * \return  A count of possible actions after this node, that will be use as indexes
         */
        virtual unsigned int get_move_count() = 0;

        /**
         * \brief       Make an action. The index must be in [0, get_move_count()[
         *
         * \param[in]   index The index of the action to execute
         * \return      An instantiation of a new IGame_State object after the action
         */
        virtual IGame_State* do_move(unsigned int index) = 0;

        /**
         * \brief Overload of operator << used to display this game state
         * do not need to be overloaded
         */
        friend std::ostream& operator<<(std::ostream& os, const IGame_State& gs) {
            gs.show(os);
            return os;
        }
        /**
         * \brief Overload of operator << used to display this game state
         * do not need to be overloaded
         */
        friend std::ostream& operator<<(std::ostream& os, const IGame_State* gs) {
            gs->show(os);
            return os;
        }

    private:
        //no copy operator
        IGame_State& operator=(const IGame_State& ) = delete;

        virtual void show(std::ostream& os) const = 0;

};


} /* MCTS */

#endif
