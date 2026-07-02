#include "MainWindow.h"
#include "GameBoard.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_board = new GameBoard(this);
    setCentralWidget(m_board);
    setWindowTitle("五子棋 Wuziqi");
    resize(800, 800);

    createActions();
    createMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::createActions()
{
    newAct = new QAction(tr("新游戏"), this);
    connect(newAct, &QAction::triggered, this, &MainWindow::newGame);

    saveAct = new QAction(tr("保存棋谱"), this);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveGame);

    loadAct = new QAction(tr("加载棋谱"), this);
    connect(loadAct, &QAction::triggered, this, &MainWindow::loadGame);

    exitAct = new QAction(tr("退出"), this);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    undoAct = new QAction(tr("悔棋"), this);
    connect(undoAct, &QAction::triggered, this, &MainWindow::undoMove);

    redoAct = new QAction(tr("重做"), this);
    connect(redoAct, &QAction::triggered, this, &MainWindow::redoMove);

    aboutAct = new QAction(tr("关于"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(loadAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("编辑"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    helpMenu = menuBar()->addMenu(tr("帮助"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::newGame()
{
    m_board->newGame();
}

void MainWindow::saveGame()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存棋谱"), QString(), tr("棋谱文件 (*.wzq);;文本 (*.txt)"));
    if (fileName.isEmpty()) return;
    if (!m_board->saveToFile(fileName)) {
        QMessageBox::warning(this, tr("保存失败"), tr("无法保存文件"));
    }
}

void MainWindow::loadGame()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("加载棋谱"), QString(), tr("棋谱文件 (*.wzq *.txt);;所有文件 (*.*)"));
    if (fileName.isEmpty()) return;
    if (!m_board->loadFromFile(fileName)) {
        QMessageBox::warning(this, tr("加载失败"), tr("无法加载文件或文件格式错误"));
    }
}

void MainWindow::undoMove()
{
    m_board->undo();
}

void MainWindow::redoMove()
{
    m_board->redo();
}

void MainWindow::showAbout()
{
    QMessageBox::information(this, tr("关于 五子棋"), tr("五子棋 (Wuziqi)\n作者: Copilot 补全\n功能: 本地对弈、人机、悔棋、存档、回放"));
}
