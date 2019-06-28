#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Инициализация счетчика команд
    actionCounter = 0;

    //Создание регулярного выражения
    QString strRange = "([а-я А-Я]*[{]?[0-9]*[,]?[0-9]*[}]?)";
    QRegExp strRegex ("^" + strRange + "$");
    //Создание валидатора
    QRegExpValidator *strValidator = new QRegExpValidator(strRegex, this);
    ui->lineEdit->setValidator(strValidator);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->lineEdit->text();

    objectGoOn = new GoOnDistance(str);
    QPoint p = objectGoOn->getPoint();
    objectGoOn->getObject();
    objectGoOn->getDirection();
    objectGoOn->getAction();


    if (objectGoOn->testData()) {
        qDebug() << "x =" << p.x() << " y =" << p.y();
        qDebug() << "Объект" << objectGoOn->getObject();
        qDebug() << "Направление" << objectGoOn->getDirection();
        vectorData.push_back(objectGoOn);
        setText(str);
    }
    else {
        qDebug() << "Ошибка с введенными данными";
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (actionCounter > vectorData.size() - 1) {
        qDebug() << "Действия завершены";
    }
    else {
        qDebug() << vectorData.at(actionCounter)->getAction();
        actionCounter++;
    }
}

void MainWindow::setText(QString &strInput)
{
    ui->listWidget->addItem(strInput);
    ui->lineEdit->clear();
}
