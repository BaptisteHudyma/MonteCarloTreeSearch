#include "puissance4.hpp"

namespace MCTS {

    Puissance4::Puissance4() {
        _board.fill(0);
        _turn = 0;
        _winner = 0;
        this->fill_moves();
    }

    Puissance4::Puissance4(const Puissance4* gs) {
        for(unsigned int index = 0; index < Puissance4::_boardWidth * Puissance4::_boardHeight; ++index) {
            _board[index] = gs->_board[index];
        }
        _turn = 1 - gs->_turn;
        _winner = 0;
        this->fill_moves();
    }
    Puissance4::~Puissance4() {

    }


    float Puissance4::get_score() const {
        if(_winner < 0)
            return 0;
        else if (_winner == 0)
            return 0.5;
        return 1;
    }

    bool Puissance4::is_game_over() const {
        if(_winner != 0)
            return true;

        return this->get_move_count() == 0;
    }

    unsigned int Puissance4::get_move_count() const {
        return _nextMoves.size();
    }

    Puissance4* Puissance4::do_move(unsigned int index) {
        if(index >= this->get_move_count())
            std::cerr << "Error: index to execute is > to max index: " << index << " " << _nextMoves.size() << std::endl;
        Puissance4* newGS = new Puissance4(this);
        Index move = _nextMoves[index];
        newGS->set_board_at(move.x, move.y);

        return newGS;
    }

    void Puissance4::fill_moves() {
        _nextMoves.clear();
        _nextMoves.reserve(Puissance4::_boardWidth);

        for(unsigned int x = 0; x < Puissance4::_boardWidth; ++x) {
            //each line starting at the bottom
            for(unsigned int y = 0; y < Puissance4::_boardHeight; ++y) {
                if(this->get_board_at(x, y) == 0) {
                    _nextMoves.push_back(Index(x, y));
                    break;
                }
            }
        }

    }

    bool Puissance4::is_full() {
        for(unsigned int x = 0; x < Puissance4::_boardWidth; ++x) {
            for(unsigned int y = 0; y < Puissance4::_boardHeight; ++y) {
                if(this->get_board_at(x, y) == 0)
                    return false;
            }
        }
        return true;
    }

    int Puissance4::get_winner(int x, int y) const {
        unsigned int minY = std::max(0, y - 3);                              //y > 0
        unsigned int maxY = std::min((int)Puissance4::_boardHeight, y + 4);  //y < _boardHeight

        unsigned int minX = std::max(0, x - 3);                              //x > 0
        unsigned int maxX = std::min((int)Puissance4::_boardWidth, x + 4);   //x < _boardWidth

        //check lines
        for(unsigned int iy = minY; iy < maxY; ++iy) {
            unsigned int sumOfLine = 0;
            int lastToTrack = 0;
            for(unsigned int ix = minX; ix < maxX; ++ix) {
                int value = get_board_at(ix, iy);

                if(lastToTrack != value) {
                    lastToTrack = value;
                    sumOfLine = 0;
                }

                sumOfLine += 1;

                if(value != 0 and sumOfLine >= 4)
                    return lastToTrack;
            }
        }
        //check columns
        for(unsigned int ix = minX; ix < maxX; ++ix) {
            unsigned int sumOfLine = 0;
            int lastToTrack = 0;
            for(unsigned int iy = minY; iy < maxY; ++iy) {
                int value = get_board_at(ix, iy);

                if(lastToTrack != value) {
                    lastToTrack = value;
                    sumOfLine = 0;
                }

                sumOfLine += 1;

                if(value != 0 and sumOfLine >= 4)
                    return lastToTrack;
            }
        }

        unsigned int sumOfLine = 0;
        int lastToTrack = 0;
        for(int i = -3, j = -3; i < 4 and j < 4; ++i, ++j) {
            if(x + i < 0 or y + j < 0)
                continue;
            //std::cout << "[" << x + i << ", " << y + i << "] " << std::endl;
            int value = get_board_at(x + i, y + j);
            if(lastToTrack != value) {
                lastToTrack = value;
                sumOfLine = 0;
            }
            sumOfLine += 1;
            if(value != 0 and sumOfLine >= 4)
                return lastToTrack;
        }
        sumOfLine = 0;
        lastToTrack = 0;
        for(int i = -3, j = 3; i < 4 and j > -4 ; ++i, --j) {
            if(x + i < 0 or y + j < 0)
                continue;
            //std::cout << "[" << x + i << ", " << y + j << "] " << std::endl;
            int value = get_board_at(x + i, y + j);
            if(lastToTrack != value) {
                lastToTrack = value;
                sumOfLine = 0;
            }
            sumOfLine += 1;
            if(value != 0 and sumOfLine >= 4)
                return lastToTrack;
        }

        return 0;
    }

    //get/set array
    void Puissance4::set_board_at(unsigned int x, unsigned int y) {
        if(x >= Puissance4::_boardWidth or y >= Puissance4::_boardHeight) {
            //std::cerr << "set_board_at() " << x << " " << y << " must be between 0 and 2" << std::endl;
            return;
        }
        _board[x * Puissance4::_boardHeight + y] = _turn * 2 - 1;  //-1 to 1
        _winner = get_winner(x, y);
        this->fill_moves();
    }
    int Puissance4::get_board_at(unsigned int x, unsigned int y) const {
        if(x >= Puissance4::_boardWidth or y >= Puissance4::_boardHeight) {
            //std::cerr << x << " " << y << " must be between 0 and " << Puissance4::_boardHeight << "/" << Puissance4::_boardWidth<< std::endl;
            return 0;
        }
        return _board[x * Puissance4::_boardHeight + y];
    }



    void Puissance4::show(std::ostream& os) const {
        for(unsigned int y = Puissance4::_boardHeight; y > 0; y--) {
            for(unsigned int x = Puissance4::_boardWidth; x > 0; x--) {
                int val = get_board_at(x - 1, y - 1);
                if(val < 0)
                    os << " 0 |";
                else if(val > 0)
                    os << " X |";
                else
                    os << "   |"; 

            }
            os <<  std::endl;
        }

    }

    void Puissance4::set_turn(int turn) {
        _turn = turn;
    }


} /**MCTS**/



