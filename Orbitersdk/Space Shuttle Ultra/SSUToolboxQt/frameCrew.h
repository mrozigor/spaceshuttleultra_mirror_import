#ifndef FRAMECREW_H
#define FRAMECREW_H

#include <QFrame>

namespace Ui {
class FrameCrew;
}

class FrameCrew : public QFrame
{
    Q_OBJECT

public:
    explicit FrameCrew(QWidget *parent = 0);
    ~FrameCrew();

private:
    Ui::FrameCrew *ui;
};

#endif // FRAMECREW_H
