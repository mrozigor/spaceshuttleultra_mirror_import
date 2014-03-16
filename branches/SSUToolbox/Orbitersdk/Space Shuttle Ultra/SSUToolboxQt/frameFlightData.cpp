#include "frameFlightData.h"
#include "ui_frameFlightData.h"

FrameFlightData::FrameFlightData(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameFlightData)
{
    ui->setupUi(this);
}

FrameFlightData::~FrameFlightData()
{
    delete ui;
}
