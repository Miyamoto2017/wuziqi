#include "GameBoard.h"
#include "GameLogic.h"
#include "AIPlayer.h"

#include <QPainter>
#include <QMouseEvent>
#include <QTimer>

GameBoard::GameBoard(QWidget *parent)
    : QWidget(parent), m_size(15), m_logic(nullptr), m_ai(nullptr), m_margin(20), m_cell(30), m_showHint(true), m_aiLevel(2)
{
    newGame(m_size);
}

void GameBoard::newGame(int size)
{
    m_size = size;
    if (m_logic) delete m_logic;
    if (m_ai) delete m_ai;
    m_logic = new GameLogic(m_size);
    m_ai = new AIPlayer();
    m_ai->setLevel(m_aiLevel);
    m_hintMove.reset();
    updateBoardGeometry();
    update();
}

bool GameBoard::saveToFile(const QString &path)
{
    return m_logic->save(path);
}

bool GameBoard::loadFromFile(const QString &path)
{
    bool ok = m_logic->load(path);
    if (ok) {
        m_size = m_logic->size();
        updateBoardGeometry();
        update();
    }
    return ok;
}

void GameBoard::undo()
{
    if (m_logic) {
        m_logic->undo();
        update();
    }
}

void GameBoard::redo()
{
    if (m_logic) {
        m_logic->redo();
        update();
    }
}

void GameBoard::setAIDifficulty(int level)
{
    m_aiLevel = level;
    if (m_ai) m_ai->setLevel(level);
}

void GameBoard::updateBoardGeometry()
{
    int w = width();
    int h = height();
    int avail = qMin(w, h) - 2 * m_margin;
    m_cell = avail / (m_size - 1);
    if (m_cell < 6) m_cell = 6;
}

QPointF GameBoard::cellTopLeft(int r, int c) const
{
    int startX = (width() - (m_size - 1) * m_cell) / 2;
    int startY = (height() - (m_size - 1) * m_cell) / 2;
    return QPointF(startX + c * m_cell, startY + r * m_cell);
}

void GameBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // background
    p.fillRect(rect(), QColor(245, 222, 179));

    // grid
    int startX = (width() - (m_size - 1) * m_cell) / 2;
    int startY = (height() - (m_size - 1) * m_cell) / 2;
    p.setPen(QPen(Qt::black, 1));
    for (int i = 0; i < m_size; ++i) {
        p.drawLine(startX, startY + i * m_cell, startX + (m_size - 1) * m_cell, startY + i * m_cell);
        p.drawLine(startX + i * m_cell, startY, startX + i * m_cell, startY + (m_size - 1) * m_cell);
    }

    // star points for standard sizes
    if (m_size >= 15) {
        int stars[] = {3, 7, 11};
        for (int r : stars) for (int c : stars) {
            QPointF tl = cellTopLeft(r, c);
            p.setBrush(Qt::black);
            p.drawEllipse(tl.x() - 3, tl.y() - 3, 6, 6);
            p.setBrush(Qt::NoBrush);
        }
    }

    // pieces
    if (!m_logic) return;
    for (int r = 0; r < m_size; ++r) for (int c = 0; c < m_size; ++c) {
        int v = m_logic->at(r, c);
        if (v == GameBoard::Empty) continue;
        QPointF tl = cellTopLeft(r, c);
        QRectF circle(tl.x() - m_cell/2 + 1, tl.y() - m_cell/2 + 1, m_cell - 2, m_cell - 2);
        if (v == GameBoard::Black) {
            QRadialGradient grad(circle.center(), m_cell/2);
            grad.setColorAt(0, QColor(30,30,30));
            grad.setColorAt(1, QColor(0,0,0));
            p.setBrush(grad);
        } else {
            p.setBrush(QBrush(Qt::white));
        }
        p.setPen(QPen(Qt::black, 1));
        p.drawEllipse(circle);
    }

    // highlight winning line
    if (m_logic->hasWinner()) {
        auto line = m_logic->winningLine();
        p.setPen(QPen(Qt::red, 4));
        for (auto &pt : line) {
            QPointF tl = cellTopLeft(pt.x(), pt.y());
            p.drawEllipse(tl.x() - m_cell/2 + 2, tl.y() - m_cell/2 + 2, m_cell - 4, m_cell - 4);
        }
    }

    // hint
    if (m_showHint && m_hintMove) {
        QPoint pt = *m_hintMove;
        QPointF tl = cellTopLeft(pt.x(), pt.y());
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 255, 0, 120));
        p.drawEllipse(tl.x() - 6, tl.y() - 6, 12, 12);
    }
}

void GameBoard::mousePressEvent(QMouseEvent *event)
{
    if (!m_logic) return;
    if (m_logic->hasWinner()) return;

    // map to nearest cell
    int startX = (width() - (m_size - 1) * m_cell) / 2;
    int startY = (height() - (m_size - 1) * m_cell) / 2;
    int x = event->x();
    int y = event->y();

    int c = qRound((x - startX) / (double)m_cell);
    int r = qRound((y - startY) / (double)m_cell);
    if (r < 0 || r >= m_size || c < 0 || c >= m_size) return;

    if (m_logic->at(r, c) != Empty) return;

    m_logic->play(r, c);
    m_hintMove.reset();
    update();

    // if AI is on and game not over, schedule AI move
    if (!m_logic->hasWinner() && m_aiLevel > 0) {
        QTimer::singleShot(200, this, &GameBoard::onAIMoveRequested);
    }
}

void GameBoard::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateBoardGeometry();
}

void GameBoard::onAIMoveRequested()
{
    if (!m_logic || !m_ai) return;
    if (m_logic->hasWinner()) return;
    if (m_aiLevel == 0) return;

    // AI decides based on current player to move
    QPoint mv = m_ai->bestMove(*m_logic);
    if (mv.x() >= 0) {
        m_logic->play(mv.x(), mv.y());
        update();
    }
}
