#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include "gameengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QLabel *gameOverLabel;

    GameEngine *engine;

    QTimer *timer;

    Direction direction;
    int moveBlockDownTime;
    bool immediatelyMoveBlockDown;
    bool rotate;
    bool keyLeftPressed, keyRightPressed, keyDownPressed, keySpacePressed;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void mainLoop();

    void on_columnsLineEdit_editingFinished();

    void on_rowsLineEdit_editingFinished();

    void on_startButton_clicked();

private:
    void initGameTableItems(int rows, int columns);
    void adjustGameTableItemsSize();

    void initGameTable(int rows, int columns);
    void adjustGameTableSize();

    void initNextBlockTableItems();

    void adjustGameWindowSize();

    void initGameOverLabel();
    void showGameOverLabel();
    void hideGameOverLabel();

    void refreshStats();
    void refreshNextBlockTable();

    void addMoveBlockDownTime(int level);

    QBrush getBlockColor(BlockType blockType);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
