#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

int Board::getScore(Side side) {
    // To calculate score, access the black bitset initialized in board.hpp
    // The black bitset has 64 elements [0 to 63], each representing a square
    // It is true or false depending on whether that square is black or not.  
    int blacknum = black.count();
    int whitenum = countWhite();
    if (taken[0] == 1){
        if (black [0] == 1){
            blacknum += 2;
        }
        else whitenum += 2;
    }

    if (taken[7] == 1){
        if (black [7] == 1){
            blacknum += 2;
        }
        else whitenum += 2;
    }

    if (taken[56] == 1){
        if (black [56] == 1){
            blacknum += 2;
        }
        else whitenum += 2;
    }

    if (taken[63] == 1){
        if (black [63] == 1){
            blacknum += 2;
        }
        else whitenum += 2;
    }

    if (taken[1] == 1){
        if (black[1] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[6] == 1){
        if (black[6] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[8] == 1){
        if (black[8] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[15] == 1){
        if (black[15] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[48] == 1){
        if (black[6] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[55] == 1){
        if (black[6] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[57] == 1){
        if (black[6] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[62] == 1){
        if (black[6] == 1){
            blacknum -= 3;
        }
        else whitenum -= 3;
    }

    if (taken[9] == 1){
        if (black[9] == 1){
            blacknum -= 4;
        }
        else whitenum -= 4;
    }

    if (taken[14] == 1){
        if (black[14] == 1){
            blacknum -= 4;
        }
        else whitenum -= 4;
    }

    if (taken[49] == 1){
        if (black[49] == 1){
            blacknum -= 4;
        }
        else whitenum -= 4;
    }

    if (taken[54] == 1){
        if (black[54] == 1){
            blacknum -= 4;
        }
        else whitenum -= 4;
    }

    for (int i = 2; i < 6; i++){
        if (taken[i] == 1){
            if (black[i] == 1){
                blacknum += 1;
            }
            else whitenum += 1;
        }
    }

    for (int i = 58; i < 62; i++){
        if (taken[i] == 1){
            if (black[i] == 1){
                blacknum += 1;
            }
            else whitenum += 1;
        }
    }

    for (int i = 2; i < 6; i++){
        if (taken[i*8] == 1){
            if (black[i*8] == 1){
                blacknum += 1;
            }
            else whitenum += 1;
        }
    }

    for (int i = 2; i < 6; i++){
        if (taken[i*8+7] == 1){
            if (black[i*8+7] == 1){
                blacknum += 1;
            }
            else whitenum += 1;
        }
    }

    if (side == BLACK)  {
        return blacknum - whitenum;
    }
    else    {
        return whitenum - blacknum;
    }    
}
