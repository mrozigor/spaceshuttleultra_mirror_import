#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "frameMission.h"
#include "frameMECO.h"
#include "frameKits.h"
#include "frameLanding.h"
#include "frameCrew.h"
#include "frameFlightData.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void generateFiles();

private:
    Ui::MainWindow *ui;

    FrameMission *frameMission;
    FrameMECO *frameMECO;
    FrameKits *frameKits;
    FrameLanding *frameLanding;
    FrameCrew *frameCrew;
    FrameFlightData *frameFlightData;

    QString orbiterPath;
};

#endif // MAINWINDOW_H
