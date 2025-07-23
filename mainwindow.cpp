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

    //Обработка ошибок
    if (!db.open())
    {
        QMessageBox::critical(this, "Ошибка", "База данных не открыта " + db.lastError().text());
        qApp->exit(1);
    }

    //Проставление сегодняшней даты в DateEdit
    ui->DateIncomeDE->setDate(QDate::currentDate());
    ui->DateExpenseDE->setDate(QDate::currentDate());

    //Обновление информации из БД
    UpdateInfo();

    //Скрытие кнопок в "цели"
    ui->GoalNowLE->setVisible(false);
    ui->GreenAcceptNow->setVisible(false);
    ui->GoalNeedLE->setVisible(false);
    ui->GreenAcceptNeed->setVisible(false);

    //Валидация ввода
    ui->SumIncomeLE->setValidator(new QIntValidator(0, 100000000, this));
    ui->SumExpenseLE->setValidator(new QIntValidator(0, 100000000, this));
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

//Перелистываение страницы
void MainWindow::SwitchPage(int newIndex)
{
    OpacityAnimation(450, ui->stackedWidget->currentWidget(), false);
    QTimer::singleShot(450, [this, newIndex]() {
        ui->stackedWidget->setCurrentIndex(newIndex);
        OpacityAnimation(450, ui->stackedWidget->currentWidget(), true);
    });
}

// Кнопка "+" на прибыли
void MainWindow::on_IncomeAddBtn_clicked()
{
    SwitchPage(1);
}

//Кнопка "отмены" в окне прибыли
void MainWindow::on_CancelIncomeBtn_clicked()
{
    SwitchPage(0);
}

// Кнопка "+" на расходах
void MainWindow::on_ExpenseAddBtn_clicked()
{
    SwitchPage(2);
}

//Кнопка "отмены" в окне расходов
void MainWindow::on_CancelExpenseBtn_clicked()
{
    SwitchPage(0);
}


//Кнопка "сохранения" в окне прибыли
void MainWindow::on_SaveIncomeBtn_clicked()
{
    if ((ui->CategoryIncomeCB->currentText() == "Выберите категорию") or (ui->SumIncomeLE->text() == ""))
    {
        QMessageBox::warning(this, "Предупреждение", "Не выбрана категория или не введена сумма");
    }
    else
    {
        //Сохранение данных в БД
        Inc.SetData(ui->DescIncomeLE->text(), ui->SumIncomeLE->text().toInt(), ui->CategoryIncomeCB->currentText(), ui->DateIncomeDE->dateTime());
        Inc.Set2SQL(ui->stackedWidget->currentIndex());

        //Обновление информации в интерфейсе
        UpdateInfo();

        //Сохранение источника дохода в БД и ООП
        Inc.SetSourse2SQL(ui->IncomeSourceLE->text());

        //Очистка полей в окне прибыли
        ui->SumIncomeLE->clear();
        ui->DateIncomeDE->setDate(QDate::currentDate());
        ui->CategoryIncomeCB->setCurrentIndex(0);
        ui->DescIncomeLE->clear();

        //Переход к главному окну
        SwitchPage(0);
    }
}

//Кнопка "сохранения" в окне расходов
void MainWindow::on_SaveExpenseBtn_clicked()
{

    if ((ui->CategoryExpenseCB->currentText() == "Выберите категорию") or (ui->SumExpenseLE->text() == ""))
    {
        QMessageBox::warning(this, "Предупреждение", "Не выбрана категория или не введена сумма");
    }
    else
    {
        //Сохранение данных в БД
        Ex.SetData(ui->DescExpenseLE->text(), ui->SumExpenseLE->text().toInt(), ui->CategoryExpenseCB->currentText(), ui->DateExpenseDE->dateTime());
        Ex.Set2SQL(ui->stackedWidget->currentIndex());

        //Обновление информации в интерфейсе
        UpdateInfo();

        //Очистка полей в окне расходов
        ui->SumExpenseLE->clear();
        ui->DateExpenseDE->setDate(QDate::currentDate());
        ui->CategoryExpenseCB->setCurrentIndex(0);
        ui->DescExpenseLE->clear();

        //Сохранение метода оплаты в БД и ООП
        Ex.SetPaymentMethod2SQL(ui->ExpensePaymentLE->text());

        //Переход к главному окну
        SwitchPage(0);
    }
}

//Метод обновления информации из БД
void MainWindow::UpdateInfo()
{
    //Логика обновление информации прибыли\расходов и цели
    QSqlQuery query;

    if (!query.exec("SELECT Income, Expense, GoalNow, GoalNeed FROM AccountingAllTable"))
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка обновления информации на главный интерфейс!");
        return;
    }
    if (query.next())
    {
        int Income = query.value(0).toInt();
        int Expense = query.value(1).toInt();
        int GoalValueNow = query.value(2).toInt();
        int GoalValueNeed = query.value(3).toInt();
        ui->IncomeLbl->setText(QString::number(Income) + " ₽");
        ui->ExpenseLbl->setText(QString::number(Expense) + " ₽");
        int balance = Income - Expense;
        ui->BalanceLbl->setText(QString::number(balance) + " ₽");
        ui->GoalNowSumLbl->setText(QString::number(GoalValueNow) + " ₽");
        ui->GoalNeedSumLbl->setText(QString::number(GoalValueNeed) + " ₽");
    }
}

//Кнопка редактировании кнопки "сейчас"
void MainWindow::on_GoalNowBtn_clicked()
{
    ui->GoalNowLE->setVisible(true);
    ui->GreenAcceptNow->setVisible(true);
}

//Кнопка принятия цели "сейчас"
void MainWindow::on_GreenAcceptNow_clicked()
{
    QSqlQuery query;
    query.prepare("UPDATE dbo.AccountingAllTable SET GoalNow = :value");
    query.bindValue(":value", ui->GoalNowLE->text().toInt());
    query.exec();
    ui->GoalNowLE->setVisible(false);
    ui->GreenAcceptNow->setVisible(false);
    UpdateInfo();
    ui->GoalNowLE->clear();
}

//Кнопка редактировании кнопки "требуется"
void MainWindow::on_GoalNeedBtn_clicked()
{
    ui->GoalNeedLE->setVisible(true);
    ui->GreenAcceptNeed->setVisible(true);
}

//Кнопка принятия цели "требуется"
void MainWindow::on_GreenAcceptNeed_clicked()
{
    QSqlQuery query;
    query.prepare("UPDATE dbo.AccountingAllTable SET GoalNeed = :value");
    query.bindValue(":value", ui->GoalNeedLE->text().toInt());
    query.exec();
    ui->GoalNeedLE->setVisible(false);
    ui->GreenAcceptNeed->setVisible(false);
    UpdateInfo();
    ui->GoalNeedLE->clear();
}


void MainWindow::on_MainMenuBtn_clicked()
{
    //Переход к главному окну
    SwitchPage(0);
}


void MainWindow::on_AuditLogBtnM_clicked()
{
    //Отображение аудита для прибыли
    QSqlTableModel *modelInc = new QSqlTableModel(this, db);
    modelInc->setTable("IncomeTable");
    modelInc->setSort(modelInc->fieldIndex("id"), Qt::DescendingOrder);
    modelInc->select();
    modelInc->setHeaderData(0, Qt::Horizontal, tr("ID"));
    modelInc->setHeaderData(1, Qt::Horizontal, tr("Описание"));
    modelInc->setHeaderData(2, Qt::Horizontal, tr("Сумма"));
    modelInc->setHeaderData(3, Qt::Horizontal, tr("Категория"));
    modelInc->setHeaderData(4, Qt::Horizontal, tr("Дата"));
    modelInc->setHeaderData(5, Qt::Horizontal, tr("Источник дохода"));
    ui->IncomeTV->setModel(modelInc);
    ui->IncomeTV->hideColumn(0);
    ui->IncomeTV->resizeColumnsToContents();
    ui->IncomeTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->IncomeTV->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->IncomeTV->verticalHeader()->setDefaultSectionSize(40); // Высота строк


    //Отображение аудита для расходов
    QSqlTableModel *modelEx = new QSqlTableModel(this, db);
    modelEx->setTable("ExpenseTable");
    modelEx->setSort(modelEx->fieldIndex("id"), Qt::DescendingOrder);
    modelEx->select();
    modelEx->setHeaderData(0, Qt::Horizontal, tr("ID"));
    modelEx->setHeaderData(1, Qt::Horizontal, tr("Описание"));
    modelEx->setHeaderData(2, Qt::Horizontal, tr("Сумма"));
    modelEx->setHeaderData(3, Qt::Horizontal, tr("Категория"));
    modelEx->setHeaderData(4, Qt::Horizontal, tr("Дата"));
    modelEx->setHeaderData(5, Qt::Horizontal, tr("Способ оплаты"));
    ui->ExpenseTV->setModel(modelEx);
    ui->ExpenseTV->hideColumn(0);
    ui->ExpenseTV->resizeColumnsToContents();
    ui->ExpenseTV->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ExpenseTV->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->ExpenseTV->verticalHeader()->setDefaultSectionSize(40); // Высота строк

    //Анимация проявления журнала аудита
    SwitchPage(3);
}

