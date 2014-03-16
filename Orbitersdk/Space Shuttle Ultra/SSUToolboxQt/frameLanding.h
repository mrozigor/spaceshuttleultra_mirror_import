#ifndef FRAMELANDING_H
#define FRAMELANDING_H

#include <QFrame>

namespace Ui {
class FrameLanding;
}

class FrameLanding : public QFrame
{
    Q_OBJECT

public:
    explicit FrameLanding(QWidget *parent = 0);
    ~FrameLanding();

private:
    Ui::FrameLanding *ui;
};

#endif // FRAMELANDING_H
