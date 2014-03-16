#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QDir>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //tab widget
    ui->tabWidget->removeTab(0);
    ui->tabWidget->setCurrentIndex(0);

    //create frames

    frameMission = new FrameMission();
    ui->tabWidget->addTab(frameMission,"General");
    connect(frameMission, SIGNAL(generateFiles()), this, SLOT(generateFiles()));

    frameMECO = new FrameMECO();
    ui->tabWidget->addTab(frameMECO,"MECO");

    frameKits = new FrameKits();
    ui->tabWidget->addTab(frameKits,"Mission Kits");

    frameLanding = new FrameLanding();
    ui->tabWidget->addTab(frameLanding,"Landing");

    frameCrew = new FrameCrew();
    ui->tabWidget->addTab(frameCrew,"Crew");

    frameFlightData = new FrameFlightData();
    ui->tabWidget->addTab(frameFlightData,"Flight Data Files");

    //orbiter path
    orbiterPath = orbiterPath = QDir::currentPath();
    int pos = orbiterPath.lastIndexOf("/SSUToolbox");
    orbiterPath.remove(pos,orbiterPath.length());
    orbiterPath.append("/");
    QString exePath = orbiterPath;
    exePath.append("orbiter.exe");
    QFile orbiterExe(exePath);
    if(!orbiterExe.exists())
    {   QMessageBox::information(this,"Error","Please put this application in the \"Orbiter\\SSUToolbox\" directory.",QMessageBox::Ok);
        orbiterPath = "";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString boolString(bool val)
{
    if(val) return "TRUE";
    else return "FALSE";
}

void MainWindow::generateFiles()
{
    if(orbiterPath=="")
    {
        QMessageBox::information(this,"Error","Please put this application in the  \"Orbiter\\SSUToolbox\" directory.",QMessageBox::Ok);
        return;
    }

    frameMission->updateParameters();
    frameMECO->updateParameters();
    frameKits->updateParameters();

    QString missionFile = orbiterPath;
    missionFile.append("/Missions/SSU/");
    missionFile.append(frameMission->missionName);
    missionFile.append(".cfg");
    QFile file(missionFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {   QMessageBox::information(this,"Error","Failed creating the file; watch for special characters on the mission name.",QMessageBox::Ok);
        return;
    }
    QTextStream stream(&file);
    stream << "Name=" << frameMission->missionName << endl;
    stream << "Orbiter=" << FrameMission::orbiterNames[frameMission->orbiter] << endl;
    //stream << "OrbiterTexture=" << "?" << endl;
    stream << "TargetINC=" << frameMECO->dfTargetInc << endl;
    stream << "TargetLAN=" << frameMECO->dfTargetLAN << endl;
    stream << "MECOAlt=" << frameMECO->dfMECOAlt << endl;
    stream << "MECOVel=" << frameMECO->dfMECOVel << endl;
    stream << "MECOFPA=" << frameMECO->dfMECOFPA << endl;
    //stream << "UseExtAl=" << "FALSE" << endl;
    stream << "UseRMS=" << boolString(frameKits->kits[0]->item) << endl;
    stream << "UseODS=" << boolString(frameKits->kits[1]->item) << endl;
    stream << "UseKUBand=" << boolString(frameKits->kits[5]->item) << endl;
    //stream << "PerformRollToHeadsUp=" << "FALSE" << endl;
    //stream << "MaxSSMEThrust=" << "100.0" << endl;
    //stream << "HasBulkheadFloodlights=" << "TRUE" << endl;
    file.close();
    QMessageBox::information(this,"Done","Mission file created",QMessageBox::Ok);
}
