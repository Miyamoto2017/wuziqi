#pragma once

#include <QPoint>
class GameLogic;

class AIPlayer
{
public:
    AIPlayer();
    void setLevel(int lv);
    QPoint bestMove(const GameLogic &state);

private:
    int m_level;
    int evaluatePosition(const GameLogic &state, int r, int c, int player) const;
};
