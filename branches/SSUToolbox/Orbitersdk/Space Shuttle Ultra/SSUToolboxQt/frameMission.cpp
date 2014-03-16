#include "frameMission.h"
#include "ui_frameMission.h"

FrameMission::FrameMission(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameMission)
{
    ui->setupUi(this);

    ui->orbiter->addItem("Atlantis");
    ui->orbiter->addItem("Challenger");
    ui->orbiter->addItem("Columbia");
    ui->orbiter->addItem("Discovery");
    ui->orbiter->addItem("Enterprise");
    ui->orbiter->addItem("Endeavour");
}

FrameMission::~FrameMission()
{
    delete ui;
}

const QString FrameMission::orbiterNames[7] = {
    "NotSSUOrbiter", "Atlantis", "Challenger", "Columbia", "Discovery", "Enterprise", "Endeavour" };

void FrameMission::on_buttonMakeFiles_clicked()
{
    emit generateFiles();
}

void FrameMission::updateParameters()
{
    missionName = ui->missionName->text();
    orbiter = Orbiter(ui->orbiter->currentIndex()+1);
}
