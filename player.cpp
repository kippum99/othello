#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    side = s; 
    other = (s == BLACK) ? WHITE : BLACK;
    // this, but it's provided for convenience.

    board = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board; 
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */

// Heuristic function, only meant to beat the simpleplayer
int Player::score(Move *m, Side s)  {
    Board *copy = board->copy();
    copy->doMove(m, s);
    int score = copy->getScore(side);
    if (((m->getX() == 0) || (m->getX() == 7)) && ((m->getY() == 0) || (m->getY() == 7)))   {
        score += 20;
    }
    else if (((m->getX() == 1) || (m->getX() == 6)) && ((m->getY() == 1) || (m->getY() == 6)))  {
        score -= 5; 
    }
    delete copy;
    return score; 
}


Move *Player::randomMove(Move *opponentsMove)   {
    board -> doMove(opponentsMove, other);
    Move *m = new Move(0, 0);
    if (board -> hasMoves(side))    {
        while (true)  {
            int x = rand()%8;
            int y = rand()%8;
            m->setX(x);
            m->setY(y);
            if (board -> checkMove(m, side))    {
                break;
            }
        }
        board -> doMove(m, side);
        return  m;
    }
    delete m;
    return nullptr;
}

Move *Player::greedyMove(Move *opponentsMove)   {
    board->doMove(opponentsMove, other);
    if (board->hasMoves(side))    {
        int best = -100;
        Move *temp = new Move(0, 0);
        Move *bestMove = new Move(0, 0); 
        for (int x = 0; x < 8; x++)    {
            for (int y = 0; y < 8; y++) {
                temp->setX(x);
                temp->setY(y);
                if (board->checkMove(temp, side))   {
                    int score = this->score(temp, side);
                    if (score > best)  {
                        best = score;
                        bestMove->setX(x);
                        bestMove->setY(y);
                    }
                }
            }
        }
        delete temp;
        board -> doMove(bestMove, side);
        return bestMove; //need to delete this wherever it is used
    }
    return nullptr;
}

int Player::minimax(Board *copy, Move* m, int depth, bool maximizingPlayer){
    Side s;
    if (maximizingPlayer) {
        s = other;
    }
    else    {
        s = side; 
    }
    
    if (depth == 0){
        return this->score(m, s);
    }

    int best;
    //unnecessary unless tree is deeper than 2-ply, as this part is done in doMinimax
    if (maximizingPlayer){
        copy->doMove(m, other);
        if (copy->hasMoves(side)){
            best = -1000;
            Move *temp = new Move(0, 0);
            for (int x = 0; x < 8; x++)    {
                for (int y = 0; y < 8; y++) {
                    temp->setX(x);
                    temp->setY(y);
                    if (copy->checkMove(temp, side)){
                        int score = this->minimax(copy, temp, depth-1, false);
                        best = max(best, score);
                    }
                }
            }
            return best;
        }
    }

    else{
        copy->doMove(m, side);
        if (copy->hasMoves(other)){
            best = 1000;
            Move *temp = new Move(0, 0);
            for (int x = 0; x < 8; x++)    {
                for (int y = 0; y < 8; y++) {
                    temp->setX(x);
                    temp->setY(y);
                    if (copy->checkMove(temp, other)){
                        int score = this->minimax(copy, temp, depth-1, true);
                        best = min(best, score);
                    }
                }
            }
            return best;
        }
    }
    return this->score(m, s);
}

Move *Player::doMinimax(Move *opponentsMove, int msLeft){
    Board * copy = board->copy();
    board->doMove(opponentsMove, other);
    if (board->hasMoves(side)){
        int best = -1000;
        Move *temp = new Move(0, 0);
        Move *bestMove = new Move(0, 0);
        for (int x = 0; x < 8; x++){
            for (int y = 0; y < 8; y++) {
                temp->setX(x);
                temp->setY(y);
                if (board->checkMove(temp, side)){
                    int score = this->minimax(copy, temp, 7, false);
                    if (score > best)  {
                        best = score;
                        bestMove->setX(x);
                        bestMove->setY(y);
                    }
                }
            }
        }
        delete temp;
        board -> doMove(bestMove, side);
        return bestMove;
    }
    return nullptr;
}

Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's move before calculating your own move
     */
    //return randomMove(opponentsMove);
    //return greedyMove(opponentsMove);
    return doMinimax(opponentsMove, msLeft);
    
        // Possible implementation for the minmax tree:
        // A more efficient way would be to look at the stones we have, and explore
        // in all four directions. If there is a stone of the other side next to our stone,
        // we keep moving in the direction, until we find an empty square.
        // We make a list of all these empty squares, and these are our possible moves. 
        // We then choose one by comparing their scores. 
        // To compare scores we make a copy of the board, update it, and then get the score of that
}
