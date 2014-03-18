#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Orbitersdk.h"

#include "windows.h"

MainWindow::MainWindow(MyQtWinManager *qtWinMngr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    qtWinMngr(qtWinMngr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showWin()
{
    this->show();

    //SetWindowLong((HWND)this->winId(), GWL_HWNDPARENT, (LONG)qtWinMngr->hOrbiterWin);

    /*
    //fullscreen workarounds
    SetWindowLongPtr(qtWinMngr->hOrbiterWin, GWL_STYLE,
        WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
    MoveWindow(qtWinMngr->hOrbiterWin, 0, 0, 1280, 720, TRUE);
    */
}

void MainWindow::updateWin()
{
    //SetForegroundWindow((HWND)this->effectiveWinId());
}

void MainWindow::on_buttonTest_clicked()
{
    OBJHANDLE hVessel = oapiGetFocusObject();
    char name[256];
    oapiGetObjectName(hVessel,name,256);
    ui->buttonTest->setText(QLatin1String(name));
}
