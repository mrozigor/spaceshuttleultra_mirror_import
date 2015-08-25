using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SSUWorkbench
{
    /// <summary>
    /// Interaction logic for MECOTool.xaml
    /// </summary>
    public partial class MECOTool : UserControl
    {
        internal LaunchTargets target { get; private set; }

        public MECOTool()
        {
            InitializeComponent();
            target = new LaunchTargets();

            target.TargetInc = 28.5;
            target.TargetLAN = 0;
            target.TargetAlt = 250.0;
            target.InsertionMode = InsertionMode.Direct;
            
            DataContext = target;
        }

        private double TTA(double fRadius, double fApR, double fPeR)
        {
            const double MU_EARTH = 3.986004418E14;
            double fSma = 0.5 * (fApR + fPeR);
            double fEcc = (fApR - fPeR) / (fApR + fPeR);
            double fPhiE = Math.Acos((1 - fRadius / fSma) / fEcc);
            double ft = Math.Sqrt(Math.Pow(fSma, 3) / MU_EARTH) * (Math.PI - (fPhiE - fEcc * Math.Sin(fPhiE)));
            return ft;
        }

        double OrbitPeriod(double fApR, double fPeR)
        {
            const double MU_EARTH = 3.986004418E14;
            double fSma = 0.5 * (fApR + fPeR);

            return Math.Sqrt(Math.Pow(fSma, 3) / MU_EARTH) * 2.0 * Math.PI;
        }

        private double VisViva(double fRadius, double fApR, double fPeR)
        {
            const double MU_EARTH = 3.986004418E14;
            return Math.Sqrt(MU_EARTH * ((2.0 / fRadius) - (2.0 / (fApR + fPeR))));
        }

        double FPA(double fRadius, double fApR, double fPeR)
        {
            const double MU_EARTH = 3.986004418E14;
            double fh = 2 * fPeR - 2 * Math.Pow(fPeR, 2) / (fApR + fPeR);
            double fx = 2 * fRadius - 2 * Math.Pow(fRadius, 2) / (fApR + fPeR);
            return Math.Acos(Math.Sqrt(fh / fx));
        }

        private void btnCalculateClick(object sender, RoutedEventArgs e)
        {
            const double RADIUS_EARTH = 6.378E6;
            const double MU_EARTH = 3.986004418E14;
            const double TMECO = 520.0;

            double fRadiusTargetOrbit = target.TargetAlt * 1000.0 + RADIUS_EARTH;
            //Apogee target for MECO orbit
            double fApogeeMECOOrbit = 110.0E3 + RADIUS_EARTH;
            double fPerigeeMECOOrbit = RADIUS_EARTH;

            double fOMS1Apogee = fRadiusTargetOrbit;
            double fOMS1Perigee = fApogeeMECOOrbit;

            if (target.InsertionMode == InsertionMode.Direct)
            {
                //Direct ascent
                fApogeeMECOOrbit = fRadiusTargetOrbit;
                fPerigeeMECOOrbit = 55.0E3 + RADIUS_EARTH;
                //Select an as high as possible periapsis for minimum 
                //fuel consumption
                //
                target.OMS1TIG = "000:00:12:00";
                target.OMS1VX = 0.0;
                target.OMS1VY = 0.0;
                target.OMS1VZ = 0.0;

            }
            else
            {
                target.OMS1TIG = "000:00:12:00";
                target.OMS1VX = 0.0;
                target.OMS1VY = 0.0;
                target.OMS1VZ = 0.0;
            }

            target.OMS2TIG = "000:00:57:00";
            target.OMS2VX = 0.0;
            target.OMS2VY = 0.0;
            target.OMS2VZ = 0.0;

            target.MECOAlt = 105.0;
            double fMECORadius = 1000.0 * target.MECOAlt + RADIUS_EARTH;
            double fSmaMECO = 0.5 * (fApogeeMECOOrbit + fPerigeeMECOOrbit);

            target.MECOVel = VisViva(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
            target.MECOFPA = FPA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit) * 180.0 / Math.PI;
            double fTIG1 = TTA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
            double fTIG_OMS1 = 0.0;
            double fTIG_OMS2 = 0.0;
            double fDV_OMS1 = 0.0;
            double fDV_OMS2 = 0.0;
            double fVTargetOrbit = Math.Sqrt(MU_EARTH / fRadiusTargetOrbit);

            if (target.InsertionMode == InsertionMode.Direct)
            {
                //Direct ascent
                fTIG_OMS1 = TMECO + 120.0;
                fTIG_OMS2 = TMECO + fTIG1;

                fDV_OMS2 = fVTargetOrbit - VisViva(fRadiusTargetOrbit, fApogeeMECOOrbit, fPerigeeMECOOrbit);
            }
            else
            {
                //Standard ascent
                fTIG_OMS1 = TMECO + fTIG1;
                fTIG_OMS2 = fTIG_OMS1 + 0.5 * OrbitPeriod(fOMS1Apogee, fOMS1Perigee);
                fDV_OMS1 = VisViva(fOMS1Perigee, fOMS1Apogee, fOMS1Perigee) - VisViva(fApogeeMECOOrbit, fApogeeMECOOrbit, fPerigeeMECOOrbit);
                fDV_OMS2 = fVTargetOrbit - VisViva(fOMS1Apogee, fOMS1Apogee, fOMS1Perigee);

            }

            fTIG_OMS1 -= fDV_OMS1 / (3 * 0.3048);
            fTIG_OMS2 -= fDV_OMS2 / (3 * 0.3048);

            target.OMS1VX = fDV_OMS1 / 0.3048;
            target.OMS2VX = fDV_OMS2 / 0.3048;

            int iTIG = (int)(fTIG_OMS1);
            int iDay = iTIG / 86400;
            iTIG -= iDay * 86400;
            int iHour = iTIG / 3600;
            iTIG -= 3600 * iHour;
            int iMin = iTIG / 60;
            int iSec = iTIG - 60 * iMin;

            target.OMS1TIG = string.Format("{0:d3}:{1:d2}:{2:d2}:{3:d2}", iDay, iHour, iMin, iSec);

            iTIG = (int)(fTIG_OMS2);
            iDay = iTIG / 86400;
            iTIG -= iDay * 86400;
            iHour = iTIG / 3600;
            iTIG -= 3600 * iHour;
            iMin = iTIG / 60;
            iSec = iTIG - 60 * iMin;

            target.OMS2TIG = string.Format("{0:d3}:{1:d2}:{2:d2}:{3:d2}", iDay, iHour, iMin, iSec);


            target.FileOutput = string.Format("TargetInc={0:f}\r\nTargetLAN={1:f}\r\nMECOAlt={2:f}\r\nMECOVel={3:f}\r\nMECOFPA={4:f}",
                target.TargetInc, target.TargetLAN, target.MECOAlt * 1000.0,
                target.MECOVel, target.MECOFPA);
        }
    }
}
