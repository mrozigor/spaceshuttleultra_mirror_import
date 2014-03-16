#include "frameLanding.h"
#include "ui_frameLanding.h"

FrameLanding::FrameLanding(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameLanding)
{
    ui->setupUi(this);
}

FrameLanding::~FrameLanding()
{
    delete ui;
}
