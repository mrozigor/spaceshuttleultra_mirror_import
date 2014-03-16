#ifndef FRAMEMECO_H
#define FRAMEMECO_H

#include <QFrame>

namespace Ui {
class FrameMECO;
}

class FrameMECO : public QFrame
{
    Q_OBJECT

public:
    explicit FrameMECO(QWidget *parent = 0);
    ~FrameMECO();

    void updateParameters();

private slots:
    void on_buttonCalculate_clicked();

public:
    double dfTargetInc;
    double dfTargetAlt;
    double dfTargetLAN;

    QString m_OMS1TIG;
    double dfOMS1VX;
    double dfOMS1VY;
    double dfOMS1VZ;
    QString m_OMS2TIG;
    double dfOMS2VX;
    double dfOMS2VY;
    double dfOMS2VZ;

    double dfMECOAlt;
    double dfMECOVel;
    double dfMECOFPA;

private:
    Ui::FrameMECO *ui;

    double TTA(double fRadius, double fApR, double fPeR);
    double OrbitPeriod(double fApR, double fPeR);
    double VisViva(double fRadius, double fApR, double fPeR);
    double FPA(double fRadius, double fApR, double fPeR);
};

#endif // FRAMEMECO_H
