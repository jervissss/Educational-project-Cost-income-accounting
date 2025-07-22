#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Подключение к БД
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={SQL Server};SERVER=JERVISSSS\\LOCALSQL;DATABASE=Nurlan");
    db.setUserName("Nurlan123");
    db.setPassword("123");
    db.open();

}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

// Анимация проявления
void OpacityAnimation(int Delay ,QWidget *LabelPix, bool OpacityBool)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(LabelPix);
    LabelPix->setGraphicsEffect(opacityEffect);
    QPropertyAnimation *fadeInAnimation = new QPropertyAnimation(opacityEffect, "opacity", LabelPix);
    fadeInAnimation->setDuration(Delay);
    fadeInAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    if (OpacityBool)
    {
        fadeInAnimation->setStartValue(0.0);
        fadeInAnimation->setEndValue(1.0);
        fadeInAnimation->start();
    }
    else if (!OpacityBool)
    {
        fadeInAnimation->setStartValue(1.0);
        fadeInAnimation->setEndValue(0.0);
        fadeInAnimation->start();
    }
}

// Кнопка "+" на прибыли
void MainWindow::on_IncomeAddBtn_clicked()
{
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(1);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

//Кнопка "отмены" в окне прибыли
void MainWindow::on_CancelIncomeBtn_clicked()
{
    //Анимация проявления к главному окну
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(0);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

// Кнопка "+" на расходах
void MainWindow::on_ExpenseAddBtn_clicked()
{
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(2);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

//Кнопка "отмены" в окне расходов
void MainWindow::on_CancelExpenseBtn_clicked()
{
    //Анимация проявления к главному окну
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(0);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

// Кнопка "+" на балансе
void MainWindow::on_BalanceAddBtn_clicked()
{
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(3);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

//Кнопка "отмены" в окне балансе
void MainWindow::on_CancelBalanceBtn_clicked()
{
    //Анимация проявления к главному окну
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(0);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

//Кнопка "сохранения" в окне прибыли
void MainWindow::on_SaveIncomeBtn_clicked()
{
    //Добавление и сохренение суммы к прибыли и в аудит
    Inc.SetData(ui->SumIncomeLE->text().toInt());
    ui->IncomeLbl->setText(QString::number(Inc.GetSum()) + " ₽");


    //Сохранение категории в аудит
    Inc.SetData(ui->CategoryIncomeCB->currentText());

    //Сохранение даты в аудит
    Inc.SetData(QDateTime::fromString(ui->DateIncomeLE->text(), "dd.MM.yyyy"));

    //Очистка полей в окне прибыли
    ui->SumIncomeLE->clear();
    ui->DateIncomeLE->clear();
    ui->CategoryIncomeCB->setCurrentIndex(0);
    ui->DescIncomeLE->clear();

    //Анимация проявления к главному окну
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(0);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

//Кнопка "сохранения" в окне расходов
void MainWindow::on_SaveExpenseBtn_clicked()
{
    //Добавление и сохренение суммы к расходу и в аудит
    Ex.SetData(ui->SumExpenseLE->text().toInt());
    ui->ExpenseLbl->setText(QString::number(Ex.GetSum()) + " ₽");

    //Сохранение категории в аудит
    Ex.SetData(ui->CategoryExpenseCB->currentText());

    //Сохранение даты в аудит
    Ex.SetData(QDateTime::fromString(ui->DateExpenseLE->text(), "dd.MM.yyyy"));

    //Очистка полей в окне расходов
    ui->SumExpenseLE->clear();
    ui->DateExpenseLE->clear();
    ui->CategoryExpenseCB->setCurrentIndex(0);
    ui->DescExpenseLE->clear();

    //Анимация проявления к главному окну
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this]()
                       {
                           ui->stackedWidget->setCurrentIndex(0);
                           OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
                       });
}

