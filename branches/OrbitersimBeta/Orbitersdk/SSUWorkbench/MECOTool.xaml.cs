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
            target.MECOPe = 55.56;
            CB_MECO_Alt.SelectedIndex = 2;// 59NM

            target.OMS1TIG = "000:00:10:30";
            target.OMS1VX = 0.0;
            target.OMS1VY = 0.0;
            target.OMS1VZ = 0.0;
            target.OMS2TIG = "000:00:40:00";
            target.OMS2VX = 0.0;
            target.OMS2VY = 0.0;
            target.OMS2VZ = 0.0;
            
            DataContext = target;
        }

        private double TTA(double fRadius, double fApR, double fPeR)
        {
            const double MU_EARTH = 398600439968871.19;
            double fSma = 0.5 * (fApR + fPeR);
            double fEcc = (fApR - fPeR) / (fApR + fPeR);
            double fPhiE = Math.Acos((1 - fRadius / fSma) / fEcc);
            double ft = Math.Sqrt(Math.Pow(fSma, 3) / MU_EARTH) * (Math.PI - (fPhiE - fEcc * Math.Sin(fPhiE)));
            return ft;
        }

        double OrbitPeriod(double fApR, double fPeR)
        {
            const double MU_EARTH = 398600439968871.19;
            double fSma = 0.5 * (fApR + fPeR);

            return Math.Sqrt(Math.Pow(fSma, 3) / MU_EARTH) * 2.0 * Math.PI;
        }

        private double VisViva(double fRadius, double fApR, double fPeR)
        {
            const double MU_EARTH = 398600439968871.19;
            return Math.Sqrt(MU_EARTH * ((2.0 / fRadius) - (2.0 / (fApR + fPeR))));
        }

        double FPA(double fRadius, double fApR, double fPeR)
        {
            double fh = 2 * fPeR - 2 * Math.Pow(fPeR, 2) / (fApR + fPeR);
            double fx = 2 * fRadius - 2 * Math.Pow(fRadius, 2) / (fApR + fPeR);
            return Math.Acos(Math.Sqrt(fh / fx));
        }

        double RadiusAtTime( double R, double ApR, double PeR, double t )
        {
			const double MU_EARTH = 398600439968871.19;
            double e = ecc( ApR, PeR );
            double a = SMa( ApR, PeR );

			double n = Math.Sqrt( MU_EARTH / (a * a * a) );
			double v = Radius2TrueAnomaly( R, e, a );
			double E = TrueAnomaly2EccentricAnomaly( v, e );
			double Mo = EccentricAnomaly2MeanAnomaly( E, e );

			double M = (n * t) + Mo;
            v = M + (2 * e * Math.Sin( M )) + (1.25 * e * e * Math.Sin( 2 * M ));

            return TrueAnomaly2Radius( v, e, a );
        }

        double ecc( double ApR, double PeR )
        {
            return (ApR - PeR) / (ApR + PeR);
        }

        double SMa( double ApR, double PeR )
        {
            return (ApR + PeR) / 2.0;
        }

		double TrueAnomaly2Radius( double v, double e, double a )
		{
			 return (a * (1 - (e * e))) / (1 + (e * Math.Cos( v )));
		}

		double Radius2TrueAnomaly( double R, double e, double a )
		{
			return Math.Acos( (((a * (1 - (e * e))) / R) - 1) / e );
		}

		double TrueAnomaly2EccentricAnomaly( double v, double e )
		{
			return Math.Acos( (e + Math.Cos( v )) / (1 + (e * Math.Cos( v ))) );
		}

		double EccentricAnomaly2MeanAnomaly( double E, double e )
		{
			return E - (e * Math.Sin( E ));
		}

		double GetVel( double R, double ApR, double FPA )
		{
			const double MU_EARTH = 398600439968871.19;
			double k = ApR / R;
			double sing = Math.Sin( (Math.PI / 2) - FPA );

			return Math.Sqrt( ((k - (k * k)) * 2 * MU_EARTH) / (((sing * sing) - (k * k)) * R) );
		}

		double GetPeR( double r1, double v1, double FPA )
		{
			const double MU_EARTH = 398600439968871.19;
			double C = (2 * MU_EARTH) / (r1 * v1 * v1);
			double sing = Math.Sin( (Math.PI / 2) - FPA );
			return ((-C + Math.Sqrt( (C * C) + (4 * (1 - C) * sing * sing) )) / (2 * (1 - C))) * r1;
		}

        private void btnCalculateClick(object sender, RoutedEventArgs e)
        {
            const double RADIUS_EARTH = 6371010;
            const double MU_EARTH = 398600439968871.19;
            const double TMECO = 510.0;
            const double NM2KM = 1.852;

            double fRadiusTargetOrbit = (target.TargetAlt * 1000.0) + RADIUS_EARTH;
            //Apogee target for MECO orbit
            double fApogeeMECOOrbit = ((85 * NM2KM) * 1000.0) + RADIUS_EARTH;
            double fPerigeeMECOOrbit = ((3 * NM2KM) * 1000.0) + RADIUS_EARTH;

            double fOMS1Apogee = fRadiusTargetOrbit;

            if (target.InsertionMode == InsertionMode.Direct)
            {
                fApogeeMECOOrbit = fRadiusTargetOrbit;
                fPerigeeMECOOrbit = (target.MECOPe * 1000.0) + RADIUS_EARTH;
                //Select an as high as possible periapsis for minimum fuel consumption
            }

            target.MECOAlt = 59 * NM2KM;// default index 2
            if (CB_MECO_Alt.SelectedIndex == 0) target.MECOAlt = 63 * NM2KM;
            else if (CB_MECO_Alt.SelectedIndex == 1) target.MECOAlt = 61 * NM2KM;
            else if (CB_MECO_Alt.SelectedIndex == 3) target.MECOAlt = 57 * NM2KM;
            else if (CB_MECO_Alt.SelectedIndex == 4) target.MECOAlt = 52 * NM2KM;

            double fMECORadius = 1000.0 * target.MECOAlt + RADIUS_EARTH;
            double fSmaMECO = 0.5 * (fApogeeMECOOrbit + fPerigeeMECOOrbit);

            target.MECOVel = VisViva(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
            target.MECOFPA = FPA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit) * 180.0 / Math.PI;
            double fTIG_OMS1 = 0.0;
            double fTIG_OMS2 = 0.0;
            double fDV_OMS1 = 0.0;
            double fDV_OMS2 = 0.0;
            double fVTargetOrbit = Math.Sqrt(MU_EARTH / fRadiusTargetOrbit);

            if (target.InsertionMode == InsertionMode.Direct)
            {
                //Direct ascent
                fTIG_OMS1 = TMECO + 120.0;
                fTIG_OMS2 = TMECO + TTA( fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit );

                fDV_OMS2 = fVTargetOrbit - VisViva(fRadiusTargetOrbit, fApogeeMECOOrbit, fPerigeeMECOOrbit);
            }
            else
            {
                //Standard ascent
                fTIG_OMS1 = TMECO + 120;
				double MECO120Radius = RadiusAtTime( fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit, 120 );
				double MECO120FPA = FPA( MECO120Radius, fApogeeMECOOrbit, fPerigeeMECOOrbit );
				double MECO120v = VisViva( MECO120Radius, fApogeeMECOOrbit, fPerigeeMECOOrbit );
				double postOMS1v = GetVel( MECO120Radius, fOMS1Apogee, MECO120FPA );
				fDV_OMS1 = postOMS1v - MECO120v;

				target.OMS1VX = (fDV_OMS1 * Math.Cos( MECO120FPA )) / 0.3048;
				target.OMS1VZ = (fDV_OMS1 * Math.Sin( MECO120FPA )) / 0.3048;

				double postOMS1PeR = GetPeR( MECO120Radius, postOMS1v, MECO120FPA );
                fTIG_OMS2 = fTIG_OMS1 + TTA( MECO120Radius, fOMS1Apogee, postOMS1PeR );
                fDV_OMS2 = fVTargetOrbit - VisViva( fOMS1Apogee, fOMS1Apogee, postOMS1PeR );
            }

            fTIG_OMS2 -= fDV_OMS2 / (3 * 0.3048);

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


            target.FileOutput = string.Format("TargetInc={0:f6}\r\nTargetLAN={1:f6}\r\nMECOAlt={2:f6}\r\nMECOVel={3:f6}\r\nMECOFPA={4:f6}",
                target.TargetInc, target.TargetLAN, target.MECOAlt * 1000.0,
                target.MECOVel, target.MECOFPA);
        }
    }
}
