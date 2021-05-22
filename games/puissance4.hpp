#ifndef MCTS_GAME_PUISSANCE_QUATRE_CLASS_HPP

#include "game_state.hpp"

#include <iostream>
#include <array>
#include <vector>

namespace MCTS {


    class Puissance4 :
        public IGame_State 
    {
        public: 
            /**
             * \brief Implementation of the get_score function of the IGame_State interface
             */
            virtual float get_score() const;

            /**
             * \brief Implementation of the is_game_over function of the IGame_State interface
             */
            virtual bool is_game_over() const;

            /**
             * \brief Implementation of the get_move_count function of the IGame_State interface
             */
            virtual unsigned int get_move_count() const;

            /**
             * \brief Implementation of the do_move function of the IGame_State interface
             */
            virtual Puissance4* do_move(unsigned int index);

            /*
             *    End of virtual function overload
             */

            Puissance4();

            Puissance4(const Puissance4* gs);

            virtual ~Puissance4();

            /**
             * \brief Sets the value of the game board at a coordinate set to the current player value (1 for X, -1 for O)
             *
             * \param[in] x
             * \param[in] y
             */
            void set_board_at(unsigned int x, unsigned int y);
            void set_turn(int turn);

        protected:

            bool is_full(); //board is full

            /**
             * \brief Calculates the winner of this game from this position
             *
             * \param[in] x x position of the last played move
             * \param[in] y y position of the last played move
             *
             * \return 1 if O won, -1 if X won, 0 if nobody won yet
             */
            int get_winner(int x, int y) const; 
            /**
             * \brief Return the value of the game board at a coordinate set
             *
             * \param[in] x
             * \param[in] y
             *
             * \return The value at [x, y] in the board
             */
            int get_board_at(unsigned int x, unsigned int y) const;

            void fill_moves();


        private:

            struct Index {
                unsigned int x;
                unsigned int y;

                Index(unsigned int _x, unsigned int _y) {
                    x = _x;
                    y = _y;
                }
            };

            /**
             * \brief Implementation of the do_move function of the IGame_State interface
             */
            virtual void show(std::ostream& os) const; 

            static const unsigned int _boardHeight = 6;
            static const unsigned int _boardWidth = 7;


            //members
            std::array<int, Puissance4::_boardWidth * Puissance4::_boardHeight> _board;
            std::vector<Index> _nextMoves;
            int _turn;
            int _winner;


    };


};

#endif
