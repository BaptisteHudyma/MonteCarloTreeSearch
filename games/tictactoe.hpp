#ifndef MCTS_GAME_TICTAC_TOE_CLASS_HPP

#include "game_state.hpp"

#include <iostream>
#include <array>
#include <vector>

namespace MCTS {

    struct Index {
        unsigned int x;
        unsigned int y;

        Index(unsigned int _x, unsigned int _y) {
            x = _x;
            y = _y;
        }
    };


    class Game_State : public IGame_State 
    {
        public:
            //0: no player action, 1 player 1, 2 player 2
            static const char _playerMarkers[3];

            /**
             * \brief Implementation of the get_score function of the IGame_State interface
             */
            virtual float get_score();
            
            /**
             * \brief Implementation of the is_game_over function of the IGame_State interface
             */
            virtual bool is_game_over();
            
            /**
             * \brief Implementation of the get_move_count function of the IGame_State interface
             */
            virtual unsigned int get_move_count();

            /**
             * \brief Implementation of the do_move function of the IGame_State interface
             */
            virtual Game_State* do_move(unsigned int index);

            /**
              * \brief Default constructor. Create an empty 3x3 grid, 0 starts
              *
              */
            Game_State();

            /**
              * \brief Copy constructor, setting the variable for next player (0 -> X, X -> O)
              */
            Game_State(const Game_State* gs);

            ~Game_State(); 

            void set_turn(unsigned int turn);

            /**
              * \brief Sets the value of the game board at a coordinate set to the current player value (1 for X, -1 for O)
              *
              * \param[in] x
              * \param[in] y
              */
            void set_board_at(unsigned int x, unsigned int y);

        protected:
            void fill_moves();


            bool is_full(); //board is full

            /**
              * \brief Return the value of the game board at a coordinate set
              *
              * \param[in] x
              * \param[in] y
              */
            int get_board_at(unsigned int x, unsigned int y) const;

            /**
              * \brief Returns the sum of the line at an index
              *
              * \param[in] lineIndex Index of the line to parse, between 0 et 2
              *
              * \return the sum of the values on the line [-3, 3]
              */
            int get_line_sum(unsigned int lineIndex) const;

            /**
              * \brief Returns the sum of the column at an index
              *
              * \param[in] columnIndex Index of the column to parse, between 0 et 2
              *
              * \return the sum of the values on the column [-3, 3]
              */
            int get_column_sum(unsigned int columnIndex) const;

            /**
              * \brief Returns the sum of a diagonal at an index
              *
              * \param[in] index Index of the column to parse, between [0, 1]
              *
              * \return the sum of the values on the diagonal [-3, 3]
              */
            int get_diagonal_sum(unsigned int index) const;
            
            /**
              * \brief Calculates the winner of this game
              *
              * \return -3 if O won, 3 if X won, 0 if nobody won yet
              */
            int get_winner() const; 

        private:
            /**
             * \brief Implementation of the do_move function of the IGame_State interface
             */
            virtual void show(std::ostream& os) const; 

            //members
            std::array<int, 9> _board;
            std::vector<Index> _nextMoves;
            int _turn;   //1 if i play, 0 if he plays

    };



}


#endif
