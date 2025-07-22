#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDateTime>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include "QSqlDatabase"
#include "QSqlQuery"

class Transaction
{
public:

    //Установка данных
    void SetData(QString NewDesc, int NewSum, QString NewCategory, QDateTime NewDate)
    {
        Sum = NewSum;
        Category = NewCategory;
        Date = NewDate;
        Description= NewDesc;
    }

    //Получение данных
    int GetSum()
    {
        return Sum;
    }


    //Перенос данных в БД
    void Set2SQL(QSqlDatabase &db, int index)
    {
        QString TableName;
        switch (index)
        {
            case(1):
            TableName = "IncomeTable";
            break;

            case(2):
            TableName = "ExpenseTable";
            break;

            case(3):
            TableName = "BalanceTable";
            break;

            default:
            break;
        }

        db.open();
        QSqlQuery query;
        query.prepare("INSERT INTO dbo."+TableName+" (Description, Sum, Category, Date) VALUES (:desc, :sum, :category, :date)");
        query.bindValue(":desc", Description);
        query.bindValue(":sum", Sum);
        query.bindValue(":category", Category);
        query.bindValue(":date", Date);

        query.exec();
        db.close();
    }

private:

    QString Description;
    int Sum = 0;
    QString Category;
    QDateTime Date;
};

//Дочерний класс прибыли
class Income : public Transaction
{
private:
    QString Source;
};

//Дочерний класс расходов
class Expense : public Transaction
{
private:
    QString PaymentMethod;
};

//Дочерний класс баланса
class Balance : public Transaction
{

};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_IncomeAddBtn_clicked();

    void on_CancelIncomeBtn_clicked();

    void on_ExpenseAddBtn_clicked();

    void on_CancelExpenseBtn_clicked();

    void on_BalanceAddBtn_clicked();

    void on_CancelBalanceBtn_clicked();

    void on_SaveIncomeBtn_clicked();

    void on_SaveExpenseBtn_clicked();

    void on_SaveBalanceBtn_clicked();

private:
    Ui::MainWindow *ui;
    Transaction Tr;
    Income Inc;
    Expense Ex;
    Balance Bal;
    QSqlDatabase db;
};


#endif // MAINWINDOW_H
