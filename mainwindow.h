#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QDebug>
#include <math.h>
#include <QRegExp>
#include <QRegExpValidator>
#include <goondistance.h>
#include <QVector>
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QVector<GoOnDistance*> vectorData;
    GoOnDistance *objectGoOn;
    int actionCounter;

    void setText(QString&);
};

#endif // MAINWINDOW_H
