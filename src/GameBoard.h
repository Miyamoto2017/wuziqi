#pragma once

#include <QWidget>
#include <vector>
#include <optional>

class GameLogic;
class AIPlayer;

class GameBoard : public QWidget
{
    Q_OBJECT
public:
    explicit GameBoard(QWidget *parent = nullptr);

    enum Player { Empty = 0, Black = 1, White = 2 };

    void newGame(int size = 15);
    bool saveToFile(const QString &path);
    bool loadFromFile(const QString &path);
    void undo();
    void redo();

    void setAIDifficulty(int level); // 0 = off, 1 = easy, 2 = medium, 3 = hard

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateBoardGeometry();
    QPointF cellTopLeft(int r, int c) const;

    int m_size;
    GameLogic *m_logic;
    AIPlayer *m_ai;
    int m_margin;
    int m_cell;
    bool m_showHint;
    int m_aiLevel;

    std::optional<QPoint> m_hintMove;

private slots:
    void onAIMoveRequested();
};
