#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initGameOverLabel();

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    QIntValidator *miniValidator = new QIntValidator(0, 9);
    ui->startingLevelLineEdit->setValidator(miniValidator);

    QIntValidator *validator = new QIntValidator(0, 99);
    ui->columnsLineEdit->setValidator(validator);
    ui->rowsLineEdit->setValidator(validator);

    timer = new QTimer(this);

    direction = DIR_NONE;
    immediatelyMoveBlockDown = false;
    rotate = false;

    keyLeftPressed = false;
    keyRightPressed = false;
    keyDownPressed = false;
    keySpacePressed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameOverLabel;
    delete engine;
    delete timer;
}

void MainWindow::mainLoop()
{
    if (!engine->isGameOver())
    {
        BlockType **board = engine->getBoard();

        engine->moveBlockToTheSide(direction);

        moveBlockDownTime--;
        if (moveBlockDownTime <= 0 || immediatelyMoveBlockDown)
        {
            immediatelyMoveBlockDown = false;

            engine->moveBlockDown();
            addMoveBlockDownTime(engine->getLevel());

            refreshStats();
            refreshNextBlockTable();
        }
        direction = DIR_NONE;

        if (rotate)
        {
            rotate = false;
            engine->rotateBlock();
        }

        for (int i = 0; i < static_cast<int>(engine->getRows()); i++)
        {
            for (int j = 0; j < static_cast<int>(engine->getColumns()); j++)
            {
                ui->gameTable->item(i, j)->setBackground(getBlockColor(board[j][i]));
            }
        }

        for (unsigned int i = 0; i < engine->getColumns(); i++)
            delete[] board[i];
        delete[] board;

        if (engine->isGameOver())
            showGameOverLabel();
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

        if (ui->gridCheckBox->isChecked())
            ui->gameTable->setShowGrid(true);
        else
            ui->gameTable->setShowGrid(false);

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
        ui->nextBlockTable->setColumnWidth(i, ui->nextBlockTable->width() / 4);
    for (int i = 0; i < cols; i++)
        ui->nextBlockTable->setRowHeight(i, ui->nextBlockTable->height() / 4);
}

void MainWindow::adjustGameWindowSize()
{
    this->setGeometry(0, 0, ui->gameTable->maximumWidth() + 160,
                      ui->gameTable->maximumHeight() + 20);
    this->move(0,0);
}

void MainWindow::initGameOverLabel()
{
    gameOverLabel = new QLabel(this);
    gameOverLabel->setText("GAME OVER");
    gameOverLabel->setFont(QFont("Arial", 32));
    gameOverLabel->setAlignment(Qt::AlignCenter);

    QPalette palette = gameOverLabel->palette();
    palette.setColor(gameOverLabel->foregroundRole(), QColor(196, 0, 0));
    gameOverLabel->setPalette(palette);

    gameOverLabel->hide();
}

void MainWindow::showGameOverLabel()
{
    gameOverLabel->setFixedSize(size());
    gameOverLabel->show();
}

void MainWindow::hideGameOverLabel()
{
    gameOverLabel->hide();
}

void MainWindow::refreshStats()
{
    ui->levelLabel->setText("Level: " + QString::number(engine->getLevel()));
    ui->scoreLabel->setText("Score: " + QString::number(engine->getScore()));
}

void MainWindow::refreshNextBlockTable()
{
    bool nextBlock[4][4];
    engine->getBlockAppearance(engine->getNextBlockType(), nextBlock);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            ui->nextBlockTable->item(i, j)->setBackground(nextBlock[j][i] ? getBlockColor(engine->getNextBlockType())
                                                                          : Qt::white);
}

void MainWindow::addMoveBlockDownTime(int level)
{
    moveBlockDownTime = static_cast<int>(((engine->MAX_LEVEL * 6) / (level * 0.8)) - 6);
}

QBrush MainWindow::getBlockColor(BlockType blockType)
{
    switch(blockType)
    {
    case BLOCK_NONE: return Qt::gray;
    case BLOCK_I: return Qt::red;
    case BLOCK_L: return Qt::magenta;
    case BLOCK_J: return Qt::yellow;
    case BLOCK_O: return Qt::blue;
    case BLOCK_S: return Qt::cyan;
    case BLOCK_T: return Qt::green;
    case BLOCK_Z: return QColor(255,128,0); //Orange
    }
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

        engine = new GameEngine(rows, cols, ui->startingLevelLineEdit->text().toInt());
        refreshStats();
        refreshNextBlockTable();

        connect(timer, SIGNAL(timeout()), this, SLOT(mainLoop()));
        timer->start(60);
        addMoveBlockDownTime(engine->getLevel());
    }
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
    if (!keyDownPressed && event->key() == Qt::Key_Space)
    {
        keySpacePressed = true;
        rotate = true;
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
    if (event->key() == Qt::Key_Space)
        keySpacePressed = false;
}
