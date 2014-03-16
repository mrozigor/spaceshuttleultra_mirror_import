#ifndef FRAMEFLIGHTDATA_H
#define FRAMEFLIGHTDATA_H

#include <QFrame>

namespace Ui {
class FrameFlightData;
}

class FrameFlightData : public QFrame
{
    Q_OBJECT

public:
    explicit FrameFlightData(QWidget *parent = 0);
    ~FrameFlightData();

private:
    Ui::FrameFlightData *ui;
};

#endif // FRAMEFLIGHTDATA_H
