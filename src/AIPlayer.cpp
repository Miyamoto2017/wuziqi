#include "AIPlayer.h"
#include "GameLogic.h"

AIPlayer::AIPlayer() : m_level(2) {}

void AIPlayer::setLevel(int lv) { m_level = lv; }

// Very simple heuristic: score by checking lines
int AIPlayer::evaluatePosition(const GameLogic &state, int r, int c, int player) const
{
    int s = 0;
    const int size = state.size();
    const int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int d=0; d<4; ++d) {
        int count = 1; // include the placed stone
        int blocks = 0;
        int dr = dirs[d][0], dc = dirs[d][1];
        int rr = r + dr, cc = c + dc;
        while (rr>=0 && rr<size && cc>=0 && cc<size && state.at(rr,cc)==player) { count++; rr+=dr; cc+=dc; }
        if (!(rr>=0 && rr<size && cc>=0 && cc<size) || state.at(rr,cc) != 0) blocks++;
        rr = r - dr; cc = c - dc;
        while (rr>=0 && rr<size && cc>=0 && cc<size && state.at(rr,cc)==player) { count++; rr-=dr; cc-=dc; }
        if (!(rr>=0 && rr<size && cc>=0 && cc<size) || state.at(rr,cc) != 0) blocks++;

        if (count >= 5) return 100000;
        if (count == 4 && blocks == 0) s += 10000;
        else if (count == 4 && blocks == 1) s += 1000;
        else if (count == 3 && blocks == 0) s += 500;
        else if (count == 3 && blocks == 1) s += 100;
        else if (count == 2 && blocks == 0) s += 50;
        else if (count == 2 && blocks == 1) s += 10;
    }
    return s;
}

QPoint AIPlayer::bestMove(const GameLogic &state)
{
    // For simplicity, we score each empty position and choose best for current player
    int size = state.size();
    int player = state.currentPlayer();
    int bestScore = -1;
    QPoint best(-1,-1);
    for (int r=0;r<size;++r) for (int c=0;c<size;++c) {
        if (state.at(r,c) != 0) continue;
        int myScore = evaluatePosition(state, r, c, player);
        int oppScore = evaluatePosition(state, r, c, (player==1)?2:1);
        int score = myScore + oppScore/2;
        if (score > bestScore) {
            bestScore = score;
            best = QPoint(r,c);
        }
    }
    return best;
}
