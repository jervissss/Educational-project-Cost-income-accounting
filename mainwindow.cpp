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

    //Проставление сегодняшней даты в DateEdit
    ui->DateIncomeDE->setDate(QDate::currentDate());
    ui->DateExpenseDE->setDate(QDate::currentDate());

    //Обновление информации из БД
    UpdateInfo();
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


//Кнопка "сохранения" в окне прибыли
void MainWindow::on_SaveIncomeBtn_clicked()
{
    //Сохранение данных в БД
    Inc.SetData(ui->DescIncomeLE->text(), ui->SumIncomeLE->text().toInt(), ui->CategoryIncomeCB->currentText(), ui->DateIncomeDE->dateTime());
    Inc.Set2SQL(db, ui->stackedWidget->currentIndex());

    //Обновление информации в интерфейсе
    UpdateInfo();

    //Очистка полей в окне прибыли
    ui->SumIncomeLE->clear();
    ui->DateIncomeDE->setDate(QDate::currentDate());
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
    //Сохранение данных в БД
    Ex.SetData(ui->DescExpenseLE->text(), ui->SumExpenseLE->text().toInt(), ui->CategoryExpenseCB->currentText(), ui->DateExpenseDE->dateTime());
    Ex.Set2SQL(db, ui->stackedWidget->currentIndex());

    //Обновление информации в интерфейсе
    UpdateInfo();

    //Очистка полей в окне расходов
    ui->SumExpenseLE->clear();
    ui->DateExpenseDE->setDate(QDate::currentDate());
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

//Метод обновления информации из БД
void MainWindow::UpdateInfo()
{
    db.open();
    QSqlQuery query;
    query.exec("SELECT Income FROM AccountingAllTable");
    query.next();
    int Income = query.value(0).toInt();
    ui->IncomeLbl->setText(QString::number(Income) + " ₽");
    query.exec("SELECT Expense FROM AccountingAllTable");
    query.next();
    int Expense = query.value(0).toInt();
    ui->ExpenseLbl->setText(QString::number(Expense) + " ₽");
    int balance = Income - Expense;
    ui->BalanceLbl->setText(QString::number(balance) + " ₽");
}
