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
            /**
             *  Interface overload
             *
             */
            float get_score();
            
            
            bool is_game_over();
            
            
            unsigned int get_move_count();


            Game_State* do_move(unsigned int index);
            /**
             * End of interface overloading
             *
             */

            Game_State();

            Game_State(const Game_State* gs);

            ~Game_State(); 


        protected:
            void fill_moves();


            //get/set array
            void set_board_at(unsigned int x, unsigned int y);


            int get_board_at(unsigned int x, unsigned int y) const;


            int get_line_sum(unsigned int lineIndex) const;


            int get_column_sum(unsigned int columnIndex) const;

            int get_diagonal_sum(unsigned int index) const;
            //3  for X
            //-3 for O
            //0  no winner
            int get_winner() const; 

        private:
            void show(std::ostream& os) const; 

            //members
            std::array<int, 9> _board;
            std::vector<Index> _nextMoves;
            int _turn;   //1 if i play, 0 if he plays

    };

}


#endif
