#include <iostream>

#include "MCTS.hpp"


struct Index {
    unsigned int x;
    unsigned int y;

    Index(unsigned int _x, unsigned int _y) {
        x = _x;
        y = _y;
    }
};

class GameState 
: public MCTS::IGame_State 
{
    private:
        int* _board;
        std::vector<Index> _nextMoves;
        int _turn;   //1 if i play, 0 if he plays

    public:

        GameState() {
            _board = new int[9];    //all 0

            for(unsigned int i = 0; i < 9; ++i)
                _board[i] = 0;
            _turn = 1;
        }
        GameState(unsigned int moveX, unsigned int moveY) 
            : GameState() 
        {
            if(moveX < 0 or moveX >= 3 or moveY < 0 or moveY >= 3) {
                std::cout << "Cannot use coordinates [" << moveX << ", " << moveY << "]" << std::endl;
                return;
            }
            _turn = 1;
            this->set_board_at(moveX, moveY, _turn);    //start as cross
        }

        //get/set array
        void set_board_at(unsigned int x, unsigned int y, int value) {
            _board[x * 2 + y] = value * 2 - 1;  //-1 to 1
        }
        int get_board_at(unsigned int x, unsigned int y) {
            return _board[x * 2 + y];
        }

        int get_line_sum(unsigned int lineIndex) {
            int sum = 0;
            for(unsigned int i = 0; i < 3; ++i) {
                sum += this->get_board_at(lineIndex, i);
            }
            return sum;
        }
        int get_column_sum(unsigned int columnIndex) {
            int sum = 0;
            for(unsigned int i = 0; i < 3; ++i) {
                sum += this->get_board_at(i, columnIndex);
            }
            return sum;
        }
        int get_diagonal_sum(unsigned int index) {
            int sum = 0;
            if(index == 0) {
                for(unsigned int i = 0; i < 3; ++i) {
                    sum += this->get_board_at(i, i);
                }
            }
            else {
                for(unsigned int i = 0; i < 3; ++i) {
                    sum += this->get_board_at(2 - i, i);
                }
            }
            return sum;
        }

        //3  for X
        //-3 for O
        //0  no winner
        int get_winner() {
            for(unsigned int i = 0; i < 3; ++i) {
                int res = get_line_sum(i);
                if(abs(res) >= 3)
                    return res;
                res = get_column_sum(i);
                if(abs(res) >= 3)
                    return res;
            }
            for(unsigned int i = 0; i < 2; ++i) {
                int res = get_diagonal_sum(i);
                if(abs(res) >= 3)
                    return res;    //one of the player won
            }
            return 0;
        }

        float get_score() {
            this->show();
            int winner = this->get_winner();
            if(winner < 0)
                return 0;
            else if (winner == 0)
                return 0.5;
            else
                return 1;

        }

        bool is_game_over() {
            if(this->get_move_count() <= 0)
                return true;

            int winner = this->get_winner();
            if(winner != 0)
                return true;

            for(unsigned int i = 0; i < 9; ++i) {
                if(_board[i] == 0)
                    return false;   //at least a move to play
            }
            return true;
        }

        GameState* do_move(unsigned int index) {
            if(index >= _nextMoves.size())
                std::cout << "Error: index to execute is > to max index: " << index << " " << _nextMoves.size() << std::endl;
            GameState* newGS = new GameState();
            newGS->copy_grid(_board, 9);
            newGS->_turn = 1 - _turn;
            newGS->set_board_at(_nextMoves[index].x, _nextMoves[index].y, newGS->_turn);


            return newGS;
        }

        void copy_grid(int* grid, unsigned int size) {
            for(unsigned int i = 0; i < size; ++i)
                _board[i] = grid[i];

        }

        unsigned int get_move_count() {
            if(_nextMoves.size() == 0) {
                for(unsigned int x = 0; x < 3; ++x) {
                    for(unsigned int y = 0; y < 3; ++y) {
                        if(this->get_board_at(x, y) == 0) {   //no move on this cell
                            _nextMoves.push_back(Index(x, y));
                        }
                    }
                }
            }
            return _nextMoves.size();
        }

        void show() {
            for(unsigned int x = 0; x < 3; ++x) {
                for(unsigned int y = 0; y < 3; ++y) {
                    int boardVal = get_board_at(x, y);
                    switch(boardVal) {
                        case -1:
                            std::cout << " O | ";
                            break;
                        case 1:
                            std::cout << " X | ";
                            break;
                        case 0:
                        default:
                            std::cout << "   | ";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << "--------" << std::endl;
        }

};



int main(int argc, char** argv) {

    GameState* test = new GameState();

    MCTS::MCTS monteCarloTreeSearch(test);

    unsigned int bestIndex = monteCarloTreeSearch.search_best_move(10);

    std::cout << "Best index is " << bestIndex << std::endl;

    return 0;
}
