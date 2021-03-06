#include "tictactoe.hpp"


namespace MCTS {
    //set player tokens
    const char Game_State::_playerMarkers[3] = {' ', 'X', 'O'};

    float Game_State::get_score() const {
        int winner = this->get_winner();
        if(winner < 0)
            return 0;
        else if (winner == 0)
            return 0.5;
        return 1;

    }

    bool Game_State::is_game_over() const {
        //if there is a winner, game over
        if(this->get_winner() != 0)
            return true;

        //grid full
        return this->is_full();
    }

    unsigned int Game_State::get_move_count() const {
        return _nextMoves.size();
    }

    Game_State* Game_State::do_move(unsigned int index) {
        if(index >= this->get_move_count())
            std::cerr << "Error: index to execute is > to max index: " << index << " " << _nextMoves.size() << std::endl;
        Game_State* newGS = new Game_State(this);
        Index move = _nextMoves[index];
        newGS->set_board_at(move.x, move.y);

        return newGS;
    }

    /**
     * End of interface overloading
     *
     */



    Game_State::Game_State() {
        _board.fill(0);
        _turn = 0;
        this->fill_moves();
    }
    Game_State::Game_State(const Game_State* gs) {
        for(unsigned int i = 0; i < 9; ++i)
            _board[i] = gs->_board[i];
        _turn = 1 - gs->_turn;
        this->fill_moves();
    }
    Game_State::~Game_State() {
        _nextMoves.clear();
    }


    void Game_State::set_turn(unsigned int turn) {
        _turn = turn;
    }

    void Game_State::fill_moves() {
        _nextMoves.clear();

        if(not this->is_game_over()) {
            //no winner/loser yer

            for(unsigned int x = 0; x < 3; ++x) {
                for(unsigned int y = 0; y < 3; ++y) {
                    if(this->get_board_at(x, y) == 0) {   //no move on this cell
                        _nextMoves.push_back(Index(x, y));
                    }
                }
            }

        }
    }

    bool Game_State::is_full() const {
        for(int i = 0; i < 9; ++i) {
            if(_board[i] == 0)
                return false;
        }
        return true;
    }

    //get/set array
    void Game_State::set_board_at(unsigned int x, unsigned int y) {
        if(x > 2 or y > 2) {
            std::cout << x << " " << y << " must be between 0 and 2" << std::endl;
            return;
        }
        _board[x * 3 + y] = _turn * 2 - 1;  //-1 to 1
        this->fill_moves();
    }
    int Game_State::get_board_at(unsigned int x, unsigned int y) const {
        return _board[x * 3 + y];
    }

    int Game_State::get_line_sum(unsigned int lineIndex) const {
        int sum = 0;
        for(unsigned int i = 0; i < 3; ++i) {
            sum += this->get_board_at(lineIndex, i);
        }
        return sum;
    }
    int Game_State::get_column_sum(unsigned int columnIndex) const {
        int sum = 0;
        for(unsigned int i = 0; i < 3; ++i) {
            sum += this->get_board_at(i, columnIndex);
        }
        return sum;
    }
    int Game_State::get_diagonal_sum(unsigned int index) const {
        int sum = 0;
        if(index == 0) {
            sum += this->get_board_at(0, 0);
            sum += this->get_board_at(1, 1);
            sum += this->get_board_at(2, 2);
        }
        else {
            sum += this->get_board_at(0, 2);
            sum += this->get_board_at(1, 1);
            sum += this->get_board_at(2, 0);
        }
        return sum;
    }

    //3  for X
    //-3 for O
    //0  no winner
    int Game_State::get_winner() const {
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


    void Game_State::show(std::ostream& os) const {
        for(unsigned int x = 0; x < 3; ++x) {
            for(unsigned int y = 0; y < 3; ++y) {
                int boardVal = this->get_board_at(x, y);
                switch(boardVal) {
                    case -1:
                        os << " " << Game_State::_playerMarkers[1] << " | ";
                        break;
                    case 1:
                        os << " " << Game_State::_playerMarkers[2] << " | ";
                        break;
                    case 0:
                    default:
                        os << " " << Game_State::_playerMarkers[0] << " | ";
                }
            }
            os << std::endl;
        }
        os << "------------" << std::endl;
    }



}
