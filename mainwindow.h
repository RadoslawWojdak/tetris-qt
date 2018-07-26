#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "User32.lib")

#include <QMainWindow>
#include "gameengine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    GameEngine *engine;
    QTimer *timer;
    Direction direction;

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

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
