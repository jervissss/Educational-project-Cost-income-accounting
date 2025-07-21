#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
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

