#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDateTime>

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

private:
    Ui::MainWindow *ui;
};

class Transaction
{
public:

void GetSum()
    {

    }

void ChangeCategory(QString NewCategory)
    {

    }

void SetSum()
    {

    }

private:

    QString Description;
    int Sum;
    QString Category;
    QDateTime Date;

};

class Income : Transaction
{
private:
    QString Source;
};

class Expense : Transaction
{
private:
    QString PaymentMethod;
};

#endif // MAINWINDOW_H
