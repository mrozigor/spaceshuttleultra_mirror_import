using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SSUToolbox.Forms
{
    public partial class MECO : UserControl
    {
        public MECO()
        {
            InitializeComponent();

            ascentMode.SelectedItem = ascentMode.Items[0];

            orbiterWeight.Text = dfOrbiterWeight.ToString();
        }

        public double dfTargetInc;
        public double dfTargetAlt;
        public double dfTargetLAN;
        public double dfOrbiterWeight = 2000000.0;

        public double dfMECOVel;
        public double dfMECOFPA;
        public double dfMECOAlt;

        public String m_OMS1TIG;
        public double dfOMS1VX;
        public double dfOMS1VY;
        public double dfOMS1VZ;
        public String m_OMS2TIG;
        public double dfOMS2VX;
        public double dfOMS2VY;
        public double dfOMS2VZ;

        public void updateParameters()
        {
            calculate_Click(null,null);
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

        private double OrbitPeriod(double fApR, double fPeR)
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

        private double FPA(double fRadius, double fApR, double fPeR)
        {
            //const double MU_EARTH = 3.986004418E14;
            double fh = 2 * fPeR - 2 * Math.Pow(fPeR, 2) / (fApR + fPeR);
            double fx = 2 * fRadius - 2 * Math.Pow(fRadius, 2) / (fApR + fPeR);
            return Math.Acos(Math.Sqrt(fh / fx));
        }

        private void calculate_Click(object sender, EventArgs e)
        {
            const double RADIUS_EARTH = 6.378E6;
            const double MU_EARTH = 3.986004418E14;
            const double TMECO = 520.0;

            //read user input data
            dfTargetInc = (double)targetInc.Value;
            dfTargetAlt = (double)targetAlt.Value;
            dfTargetLAN = (double)targetLAN.Value;

            String sAscentMode = ascentMode.GetItemText(ascentMode.SelectedItem);

            double fRadiusTargetOrbit = dfTargetAlt * 1000.0 + RADIUS_EARTH;
            //Apogee target for MECO orbit
            double fApogeeMECOOrbit = 110.0E3 + RADIUS_EARTH;
            double fPerigeeMECOOrbit = RADIUS_EARTH;

            double fOMS1Apogee = fRadiusTargetOrbit;
            double fOMS1Perigee = fApogeeMECOOrbit;

            if(sAscentMode == "Direct")
            {
                fApogeeMECOOrbit = fRadiusTargetOrbit;
                fPerigeeMECOOrbit = 55.0E3 + RADIUS_EARTH;
                //Select an as high as possible periapsis for minimum
                //fuel consumption
                //
                m_OMS1TIG = "000:00:12:00";
                dfOMS1VX = 0.0;
                dfOMS1VY = 0.0;
                dfOMS1VZ = 0.0;

            } else {
                //Standard ascent
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
            dfMECOFPA = FPA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit) * 180.0/Math.PI;
            double fTIG1 = TTA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
            double fTIG_OMS1 = 0.0;
            double fTIG_OMS2 = 0.0;
            double fDV_OMS1 = 0.0;
            double fDV_OMS2 = 0.0;
            double fVTargetOrbit = Math.Sqrt(MU_EARTH/fRadiusTargetOrbit);

            if(sAscentMode == "Direct")
            {
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

            m_OMS1TIG = String.Format("{0:000}:{1:00}:{2:00}:{3:00}", iDay, iHour, iMin, iSec);

            iTIG = (int)(fTIG_OMS2);
            iDay = iTIG/86400;
            iTIG -= iDay * 86400;
            iHour = iTIG/3600;
            iTIG -= 3600 * iHour;
            iMin = iTIG/60;
            iSec = iTIG - 60 * iMin;

            m_OMS2TIG = String.Format("{0:000}:{1:00}:{2:00}:{3:00}", iDay, iHour, iMin, iSec);

            mecoVel.Text = dfMECOVel.ToString("F4");
            mecoFPA.Text = dfMECOFPA.ToString("F4");
            mecoAlt.Text = dfMECOAlt.ToString("F4");
            oms1TIG.Text = m_OMS1TIG;
            oms1VX.Text = dfOMS1VX.ToString("F4");
            oms1VY.Text = dfOMS1VY.ToString("F4");
            oms1VZ.Text = dfOMS1VZ.ToString("F4");
            oms2TIG.Text = m_OMS2TIG;
            oms2VX.Text = dfOMS2VX.ToString("F4");
            oms2VY.Text = dfOMS2VY.ToString("F4");
            oms2VZ.Text = dfOMS2VZ.ToString("F4");

            outputScn1_06.Clear();
            outputScn1_06.Text = String.Format("AUTOPILOT {0} {1} {2} {3} {4}", dfTargetInc, dfTargetLAN, dfMECOAlt*1000.0, dfMECOVel, dfMECOFPA);
        }
    }
}
