#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "myQtWinManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MyQtWinManager *qtWinMngr, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void updateWin();
    void showWin();

private slots:
    void on_buttonTest_clicked();

private:
    Ui::MainWindow *ui;

    MyQtWinManager *qtWinMngr;
};

#endif // MAINWINDOW_H
