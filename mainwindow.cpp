#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    QIntValidator *validator = new QIntValidator(0, 99);
    ui->columnsLineEdit->setValidator(validator);
    ui->rowsLineEdit->setValidator(validator);

    timer = new QTimer(this);
    direction = DIR_NONE;
    immediatelyMoveBlockDown = false;
    keyLeftPressed = false;
    keyRightPressed = false;
    keyDownPressed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete engine;
    delete timer;
}

void MainWindow::mainLoop()
{
    bool **board = engine->getBoard();

    engine->moveBlockToTheSide(direction);

    moveBlockDownTime--;
    if (moveBlockDownTime <= 0 || immediatelyMoveBlockDown)
    {
        immediatelyMoveBlockDown = false;

        engine->moveBlockDown();
        addMoveBlockDownTime(engine->getLevel());
    }
    direction = DIR_NONE;

    for (int i = 0; i < static_cast<int>(engine->getRows()); i++)
    {
        for (int j = 0; j < static_cast<int>(engine->getColumns()); j++)
        {
            if (board[j][i])
                ui->gameTable->item(i, j)->setBackground(Qt::red);
            else
                ui->gameTable->item(i, j)->setBackground(Qt::white);
        }
    }
}

void MainWindow::initGameTableItems(int rows, int columns)
{
    ui->gameTable->setRowCount(rows);
    ui->gameTable->setColumnCount(columns);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            ui->gameTable->setItem(i, j, new QTableWidgetItem);

    adjustGameTableItemsSize();
}

void MainWindow::adjustGameTableItemsSize()
{
    int rows = ui->gameTable->rowCount();
    int columns = ui->gameTable->columnCount();

    if (rows > 0 && columns > 0)
    {
        QSize maxSize;
        if (ui->adjustWinSizeCheckBox->isChecked())
            maxSize = QSize(QApplication::desktop()->availableGeometry().width() - 156,
                            QApplication::desktop()->availableGeometry().height() - 64);
        else
            maxSize = QSize(ui->gamePage->width() - 156, ui->gamePage->height() - 20);

        int newSize = (maxSize.width() / columns < maxSize.height() / rows
                       ? maxSize.width() / columns : maxSize.height() / rows);

        for (int i = 0; i < columns; i++)
            ui->gameTable->setColumnWidth(i, newSize);
        for (int i = 0; i < rows; i++)
            ui->gameTable->setRowHeight(i, newSize);
    }
}

void MainWindow::initGameTable(int rows, int columns)
{
    initGameTableItems(rows, columns);
    adjustGameTableSize();
}

void MainWindow::adjustGameTableSize()
{
    int rows = ui->gameTable->rowCount();
    int cols = ui->gameTable->columnCount();

    if (rows > 0 && cols > 0)
        ui->gameTable->setMaximumSize(ui->gameTable->columnWidth(0) * cols,
                                      ui->gameTable->rowHeight(0) * rows);
}

void MainWindow::initNextBlockTableItems()
{
    int cols = ui->nextBlockTable->columnCount();
    int rows = ui->nextBlockTable->rowCount();

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            ui->nextBlockTable->setItem(i, j, new QTableWidgetItem);

    for (int i = 0; i < rows; i++)
        ui->nextBlockTable->setColumnWidth(i, ui->nextBlockTable->width() / 3);
    for (int i = 0; i < cols; i++)
        ui->nextBlockTable->setRowHeight(i, ui->nextBlockTable->height() / 3);
}

void MainWindow::adjustGameWindowSize()
{
    this->setGeometry(0, 0, ui->gameTable->maximumWidth() + 160,
                      ui->gameTable->maximumHeight() + 20);
    this->move(0,0);
}

void MainWindow::on_columnsLineEdit_editingFinished()
{
    if (ui->columnsLineEdit->text().toInt() < 5)
        ui->columnsLineEdit->setText("5");
}

void MainWindow::on_rowsLineEdit_editingFinished()
{
    if (ui->rowsLineEdit->text().toInt() < 5)
        ui->rowsLineEdit->setText("5");
}

void MainWindow::on_startButton_clicked()
{
    int rows = ui->rowsLineEdit->text().toInt();
    int cols = ui->columnsLineEdit->text().toInt();

    if (rows > 0 && cols > 0)
    {
        ui->stackedWidget->setCurrentWidget(ui->gamePage);

        initNextBlockTableItems();
        initGameTable(rows, cols);

        if (ui->adjustWinSizeCheckBox->isChecked())
            adjustGameWindowSize();

        engine = new GameEngine(rows, cols);

        connect(timer, SIGNAL(timeout()), this, SLOT(mainLoop()));
        timer->start(10);
        addMoveBlockDownTime(engine->getLevel());
    }
}

void MainWindow::addMoveBlockDownTime(int level)
{
    moveBlockDownTime = engine->MAX_LEVEL - level + 3;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!keyLeftPressed && event->key() == Qt::Key_Left)
    {
        keyLeftPressed = true;
        direction = DIR_LEFT;
    }
    if (!keyRightPressed && event->key() == Qt::Key_Right)
    {
        keyRightPressed = true;
        if (direction == DIR_LEFT)
            direction = DIR_NONE;
        else
            direction = DIR_RIGHT;
    }
    if (!keyDownPressed && event->key() == Qt::Key_Down)
    {
        keyDownPressed = true;
        immediatelyMoveBlockDown = true;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        keyLeftPressed = false;
    if (event->key() == Qt::Key_Right)
        keyRightPressed = false;
    if (event->key() == Qt::Key_Down)
        keyDownPressed = false;
}
