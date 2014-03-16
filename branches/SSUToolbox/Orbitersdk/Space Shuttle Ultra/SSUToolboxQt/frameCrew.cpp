#include "frameCrew.h"
#include "ui_frameCrew.h"

FrameCrew::FrameCrew(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameCrew)
{
    ui->setupUi(this);
}

FrameCrew::~FrameCrew()
{
    delete ui;
}
