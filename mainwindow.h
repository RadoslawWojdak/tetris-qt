#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_columnsLineEdit_editingFinished();

    void on_rowsLineEdit_editingFinished();

    void on_startButton_clicked();

private:
    void initGameTableItems(int rows, int columns);
    void adjustGameTableItemsSize();

    void initGameTable(int rows, int columns);
    void adjustGameTableSize();

    void InitNextBlockTableItems();

    void adjustGameWindowSize();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
