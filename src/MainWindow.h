#pragma once

#include <QMainWindow>

class GameBoard;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void saveGame();
    void loadGame();
    void undoMove();
    void redoMove();
    void showAbout();

private:
    void createActions();
    void createMenus();

    GameBoard *m_board;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QAction *newAct;
    QAction *saveAct;
    QAction *loadAct;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *aboutAct;
};
