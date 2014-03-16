#include "frameMECO.h"
#include "ui_frameMECO.h"

#include <QtMath>

FrameMECO::FrameMECO(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameMECO)
{
    ui->setupUi(this);

    ui->ascentMode->addItem("Standard");
    ui->ascentMode->addItem("Direct");
}

FrameMECO::~FrameMECO()
{
    delete ui;
}

double FrameMECO::TTA(double fRadius, double fApR, double fPeR)
{
    const double MU_EARTH = 3.986004418E14;
    double fSma = 0.5 * (fApR + fPeR);
    double fEcc = (fApR-fPeR)/(fApR + fPeR);
    double fPhiE = acos((1-fRadius/fSma)/fEcc);
    double ft = sqrt(pow(fSma, 3)/MU_EARTH) * (M_PI - (fPhiE - fEcc * sin(fPhiE)));
    return ft;
}

double FrameMECO::OrbitPeriod(double fApR, double fPeR)
{
    const double MU_EARTH = 3.986004418E14;
    double fSma = 0.5 * (fApR + fPeR);

    return sqrt(pow(fSma, 3)/MU_EARTH) * 2.0 * M_PI;
}

double FrameMECO::VisViva(double fRadius, double fApR, double fPeR)
{
    const double MU_EARTH = 3.986004418E14;
    return sqrt(MU_EARTH * ((2.0/fRadius) - (2.0/(fApR + fPeR))));
}

double FrameMECO::FPA(double fRadius, double fApR, double fPeR)
{
    //const double MU_EARTH = 3.986004418E14;
    double fh = 2*fPeR-2*pow(fPeR, 2)/(fApR+fPeR);
    double fx = 2*fRadius - 2*pow(fRadius, 2)/(fApR+fPeR);
    return acos(sqrt(fh/fx));
}

void FrameMECO::updateParameters() //called by the main window when it needs calculation results
{
    on_buttonCalculate_clicked();
}

void FrameMECO::on_buttonCalculate_clicked()
{
    const double RADIUS_EARTH = 6.378E6;
    const double MU_EARTH = 3.986004418E14;
    const double TMECO = 520.0;

    //read user input data
    dfTargetInc = ui->targetInc->value();
    dfTargetAlt = ui->targetAlt->value();
    dfTargetLAN = 0.0;

    int iAscentMode = ui->ascentMode->currentIndex();

    double fRadiusTargetOrbit = dfTargetAlt * 1000.0 + RADIUS_EARTH;
    //Apogee target for MECO orbit
    double fApogeeMECOOrbit = 110.0E3 + RADIUS_EARTH;
    double fPerigeeMECOOrbit = RADIUS_EARTH;

    double fOMS1Apogee = fRadiusTargetOrbit;
    double fOMS1Perigee = fApogeeMECOOrbit;

    if(iAscentMode == 1)
    {
        //Direct ascent
        fApogeeMECOOrbit = fRadiusTargetOrbit;
        fPerigeeMECOOrbit = 55.0E3 + RADIUS_EARTH;
        //Select an as high as possible periapsis for minimum
        //fuel consumption
        //
        QString m_OMS1TIG = "000:00:12:00";
        dfOMS1VX = 0.0;
        dfOMS1VY = 0.0;
        dfOMS1VZ = 0.0;

    } else {
        m_OMS1TIG = "000:00:12:00";
        dfOMS1VX = 0.0;
        dfOMS1VY = 0.0;
        dfOMS1VZ = 0.0;
    }

    m_OMS2TIG = "000:00:57:00";
    dfOMS2VX = 0.0;
    dfOMS2VY = 0.0;
    dfOMS2VZ = 0.0;

    dfMECOAlt = 105.0;
    double fMECORadius = 1000.0 * dfMECOAlt + RADIUS_EARTH;
    //double fSmaMECO = 0.5 * (fApogeeMECOOrbit + fPerigeeMECOOrbit);

    dfMECOVel = VisViva(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
    dfMECOFPA = FPA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit) * 180.0/M_PI;
    double fTIG1 = TTA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
    double fTIG_OMS1 = 0.0;
    double fTIG_OMS2 = 0.0;
    double fDV_OMS1 = 0.0;
    double fDV_OMS2 = 0.0;
    double fVTargetOrbit = sqrt(MU_EARTH/fRadiusTargetOrbit);

    if(iAscentMode == 1)
    {
        //Direct ascent
        fTIG_OMS1 = TMECO + 120.0;
        fTIG_OMS2 = TMECO + fTIG1;

        fDV_OMS2 = fVTargetOrbit - VisViva(fRadiusTargetOrbit, fApogeeMECOOrbit, fPerigeeMECOOrbit);
    }
    else {
        //Standard ascent
        fTIG_OMS1 = TMECO + fTIG1;
        fTIG_OMS2 = fTIG_OMS1 + 0.5 * OrbitPeriod(fOMS1Apogee, fOMS1Perigee);
        fDV_OMS1 = VisViva(fOMS1Perigee, fOMS1Apogee, fOMS1Perigee) - VisViva(fApogeeMECOOrbit, fApogeeMECOOrbit, fPerigeeMECOOrbit);
        fDV_OMS2 = fVTargetOrbit - VisViva(fOMS1Apogee, fOMS1Apogee, fOMS1Perigee);

    }

    fTIG_OMS1 -= fDV_OMS1/(3 * 0.3048);
    fTIG_OMS2 -= fDV_OMS2/(3 * 0.3048);

    dfOMS1VX = fDV_OMS1 / 0.3048;
    dfOMS2VX = fDV_OMS2 / 0.3048;

    int iTIG = (int)(fTIG_OMS1);
    int iDay = iTIG/86400;
    iTIG -= iDay * 86400;
    int iHour = iTIG/3600;
    iTIG -= 3600 * iHour;
    int iMin = iTIG/60;
    int iSec = iTIG - 60 * iMin;

    m_OMS1TIG = QString("%1:%2:%3:%4").arg(iDay,3,'g',-1,'0').arg(iHour,2,'g',-1,'0').arg(iMin,2,'g',-1,'0').arg(iSec,2,'g',-1,'0');

    iTIG = (int)(fTIG_OMS2);
    iDay = iTIG/86400;
    iTIG -= iDay * 86400;
    iHour = iTIG/3600;
    iTIG -= 3600 * iHour;
    iMin = iTIG/60;
    iSec = iTIG - 60 * iMin;

    m_OMS2TIG = QString("%1:%2:%3:%4").arg(iDay,3,'g',-1,'0').arg(iHour,2,'g',-1,'0').arg(iMin,2,'g',-1,'0').arg(iSec,2,'g',-1,'0');

    ui->mecoVel->setText(QString::number(dfMECOVel));
    ui->mecoFPA->setText(QString::number(dfMECOFPA));
    ui->mecoAlt->setText(QString::number(dfMECOAlt));
    ui->oms1TIG->setText(m_OMS1TIG);
    ui->oms1VX->setText(QString::number(dfOMS1VX));
    ui->oms1VY->setText(QString::number(dfOMS1VY));
    ui->oms1VZ->setText(QString::number(dfOMS1VZ));
    ui->oms2TIG->setText(m_OMS2TIG);
    ui->oms2VX->setText(QString::number(dfOMS2VX));
    ui->oms2VY->setText(QString::number(dfOMS2VY));
    ui->oms2VZ->setText(QString::number(dfOMS2VZ));

    ui->txtFileOutput->clear();
    ui->txtFileOutput->insertPlainText(QString("AUTOPILOT %1 %2 %3 %4 %5").
                                            arg(dfTargetInc).arg(dfTargetLAN).arg(dfMECOAlt*1000.0).arg(dfMECOVel).arg(dfMECOFPA));
}
