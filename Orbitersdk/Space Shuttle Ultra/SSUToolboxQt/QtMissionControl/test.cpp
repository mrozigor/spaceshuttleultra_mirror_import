#include "test.h"

#define STRICT
#define ORBITER_MODULE
#include "windows.h"
#include "orbitersdk.h"

#include "mainwindow.h"

#include "myQtWinManager.h"

#include <QApplication>
#include <QThread>

class AppThread : public QThread
{
    int & argc;
    char ** argv;
    int result;

  void run()
    {
        int argc__ = 1;
        static char* argv__[] = {"Orbiter"};
        app = new QApplication(argc__, argv__);
        myQtWinManager = new MyQtWinManager;

        qtwin = new MainWindow(myQtWinManager);

        QObject::connect(myQtWinManager, &MyQtWinManager::updateWin, qtwin, &MainWindow::updateWin);
        QObject::connect(myQtWinManager, &MyQtWinManager::showWin, qtwin, &MainWindow::showWin);

        result = app->exec();
        delete qtwin;
    }
public:
    MyQtWinManager *myQtWinManager;
    MainWindow *qtwin;
    QApplication *app;

    AppThread(int & argc_, char ** argv_) : argc(argc_), argv(NULL) {}
};

AppThread * thread;

HINSTANCE g_hInst;  // module instance handle

DLLCLBK void opcOpenRenderViewport(HWND hRenderWnd, DWORD width, DWORD height, BOOL fullscreen)
{
    thread->myQtWinManager->hOrbiterWin = hRenderWnd;
    thread->myQtWinManager->sendShowWin();
}

DLLCLBK void opcPreStep(double simt,double  simdt,double  mjd)
{
    thread->myQtWinManager->sendUpdateWindow();
}

// ==============================================================
// Close the dialog

DLLCLBK void InitModule (HINSTANCE hDLL)
{
    g_hInst = hDLL; // remember the instance handle

    // To allow the user to open our new dialog box, we create
    // an entry in the "Custom Functions" list which is accessed
    // in Orbiter via Ctrl-F4.
    /*
    g_dwCmd = oapiRegisterCustomCmd ("My dialog",
        "Opens a test dialog box which doesn't do much.",
        OpenDlgClbk, NULL);
    */

    int argc = 0;
    thread = new AppThread(argc,NULL);

    thread->start();
}

DLLCLBK void ExitModule (HINSTANCE hDLL)
{
    ;
}
