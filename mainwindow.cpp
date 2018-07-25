#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    QIntValidator *validator = new QIntValidator(0, 99);
    ui->columnsLineEdit->setValidator(validator);
    ui->rowsLineEdit->setValidator(validator);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::InitNextBlockTableItems()
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

        InitNextBlockTableItems();
        initGameTable(rows, cols);

        if (ui->adjustWinSizeCheckBox->isChecked())
            adjustGameWindowSize();
    }
}