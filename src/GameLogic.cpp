#include "GameLogic.h"
#include <QFile>
#include <QTextStream>

GameLogic::GameLogic(int size)
    : m_size(size), m_board(size * size, 0), m_currentPlayer(1), m_hasWinner(false)
{
}

int GameLogic::at(int r, int c) const
{
    if (r < 0 || r >= m_size || c < 0 || c >= m_size) return 0;
    return m_board[index(r, c)];
}

bool GameLogic::play(int r, int c)
{
    if (m_hasWinner) return false;
    if (at(r, c) != 0) return false;
    m_board[index(r, c)] = m_currentPlayer;
    m_moves.emplace_back(r, c);
    m_redoMoves.clear();
    checkWin(r, c);
    m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
    return true;
}

void GameLogic::undo()
{
    if (m_moves.empty()) return;
    QPoint last = m_moves.back();
    m_moves.pop_back();
    m_redoMoves.push_back(last);
    m_board[index(last.x(), last.y())] = 0;
    m_hasWinner = false;
    m_winLine.clear();
    m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
}

void GameLogic::redo()
{
    if (m_redoMoves.empty()) return;
    QPoint mv = m_redoMoves.back();
    m_redoMoves.pop_back();
    m_board[index(mv.x(), mv.y())] = m_currentPlayer;
    m_moves.push_back(mv);
    checkWin(mv.x(), mv.y());
    m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
}

void GameLogic::checkWin(int lastR, int lastC)
{
    m_hasWinner = false;
    m_winLine.clear();
    int player = at(lastR, lastC);
    if (player == 0) return;

    const int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int d = 0; d < 4; ++d) {
        int dr = dirs[d][0];
        int dc = dirs[d][1];
        std::vector<QPoint> line;
        line.emplace_back(lastR, lastC);
        // backward
        int r = lastR - dr, c = lastC - dc;
        while (r >= 0 && r < m_size && c >= 0 && c < m_size && at(r,c) == player) {
            line.insert(line.begin(), QPoint(r,c));
            r -= dr; c -= dc;
        }
        // forward
        r = lastR + dr; c = lastC + dc;
        while (r >= 0 && r < m_size && c >= 0 && c < m_size && at(r,c) == player) {
            line.emplace_back(r,c);
            r += dr; c += dc;
        }
        if ((int)line.size() >= 5) {
            m_hasWinner = true;
            m_winLine = line;
            return;
        }
    }
}

bool GameLogic::save(const QString &path) const
{
    QFile f(path);
    if (!f.open(QFile::WriteOnly | QFile::Text)) return false;
    QTextStream out(&f);
    out << m_size << "\n";
    out << m_currentPlayer << "\n";
    for (auto &mv : m_moves) {
        out << mv.x() << " " << mv.y() << "\n";
    }
    return true;
}

bool GameLogic::load(const QString &path)
{
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text)) return false;
    QTextStream in(&f);
    int size;
    if (in.atEnd()) return false;
    in >> size;
    if (size <= 0) return false;
    int cur;
    in >> cur;
    std::vector<int> board(size * size, 0);
    std::vector<QPoint> moves;
    while (!in.atEnd()) {
        int r, c;
        in >> r >> c;
        if (in.status() != QTextStream::Ok) break;
        moves.emplace_back(r, c);
    }
    // apply
    m_size = size;
    m_board.assign(size * size, 0);
    m_moves.clear();
    m_redoMoves.clear();
    m_currentPlayer = 1;
    m_hasWinner = false;
    m_winLine.clear();
    for (auto &mv : moves) {
        if (mv.x() < 0 || mv.x() >= m_size || mv.y() < 0 || mv.y() >= m_size) break;
        m_board[index(mv.x(), mv.y())] = m_currentPlayer;
        m_moves.push_back(mv);
        checkWin(mv.x(), mv.y());
        m_currentPlayer = (m_currentPlayer == 1) ? 2 : 1;
        if (m_hasWinner) break;
    }
    return true;
}
