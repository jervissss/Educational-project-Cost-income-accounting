#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDateTime>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include "QtSql"
#include "QSqlDatabase"
#include "QSqlQuery"

class Transaction
{
public:

    void SetData(int Num)
    {
        Sum = Sum + Num;
    }

    void SetData(QString NewCategory)
    {
        Category = NewCategory;
    }
    void SetData(QDateTime NewDate)
    {
        Date = NewDate;
    }

    int GetSum()
    {
        return Sum;
    }

    QString GetCategory()
    {
        return Category;
    }

    QDateTime GetDate()
    {
        return Date;
    }
private:

    QString Description;
    int Sum = 0;
    QString Category;
    QDateTime Date;
};

class Income : public Transaction
{
private:
    QString Source;
};

class Expense : public Transaction
{
private:
    QString PaymentMethod;
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

private:
    Ui::MainWindow *ui;
    Transaction Tr;
    Income Inc;
    Expense Ex;
    QSqlDatabase db;
};


#endif // MAINWINDOW_H
