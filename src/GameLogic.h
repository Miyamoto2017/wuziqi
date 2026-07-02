#pragma once

#include <vector>
#include <QPoint>
#include <optional>

class GameLogic
{
public:
    GameLogic(int size = 15);

    int size() const { return m_size; }
    int at(int r, int c) const;
    bool play(int r, int c);
    void undo();
    void redo();
    bool hasWinner() const { return m_hasWinner; }
    std::vector<QPoint> winningLine() const { return m_winLine; }

    bool save(const QString &path) const;
    bool load(const QString &path);

    int currentPlayer() const { return m_currentPlayer; }

private:
    void checkWin(int lastR, int lastC);
    int m_size;
    std::vector<int> m_board; // 0 empty, 1 black, 2 white
    std::vector<QPoint> m_moves;
    std::vector<QPoint> m_redoMoves;
    int m_currentPlayer; // 1 black, 2 white
    bool m_hasWinner;
    std::vector<QPoint> m_winLine;

    int index(int r, int c) const { return r * m_size + c; }
};
