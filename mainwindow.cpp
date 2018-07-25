#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    QIntValidator *validator = new QIntValidator(0, 99);
    ui->columnsLineEdit->setValidator(validator);
    ui->rowsLineEdit->setValidator(validator);

    InitNextBlockTableItems();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
}
