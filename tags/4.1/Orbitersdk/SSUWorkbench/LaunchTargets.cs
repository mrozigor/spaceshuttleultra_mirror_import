using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench
{
    public enum InsertionMode
    {
        Standard,
        Direct
    }

    class LaunchTargets : INotifyPropertyChanged
    {
		double targetInc;
        public double TargetInc
		{
			get { return targetInc; }
			set
			{
				targetInc = value;
				OnPropertyChanged("TargetInc");
			}
		}
		double targetLAN;
        public double TargetLAN
		{
			get { return targetLAN; }
			set
			{
				targetLAN = value;
				OnPropertyChanged("TargetLAN");
			}
		}
		double targetAlt;
        public double TargetAlt
		{
			get { return targetAlt; }
			set
			{
				targetAlt = value;
				OnPropertyChanged("TargetAlt");
			}
		}
		InsertionMode insertionMode;
        public InsertionMode InsertionMode
		{
			get { return insertionMode; }
			set
			{
				insertionMode = value;
				OnPropertyChanged("InsertionMode");
			}
		}

		string fileOutput;
        public string FileOutput
		{
			get { return fileOutput; }
			set
			{
				fileOutput = value;
				OnPropertyChanged("FileOutput");
			}
		}

		double mecoVel;
        public double MECOVel
		{
			get { return mecoVel; }
			set
			{
				mecoVel = value;
				OnPropertyChanged("MECOVel");
			}
		}
		double mecoFPA;
        public double MECOFPA
		{
			get { return mecoFPA; }
			set
			{
				mecoFPA = value;
				OnPropertyChanged("MECOFPA");
			}
		}
		double mecoAlt;
        public double MECOAlt
		{
			get { return mecoAlt; }
			set
			{
				mecoAlt = value;
				OnPropertyChanged("MECOAlt");
			}
		}

        private string oms1tig;
        public string OMS1TIG
        {
            get { return oms1tig; }
            set
            {
                oms1tig = value;
                OnPropertyChanged("OMS1TIG");
            }
        }
        private double oms1vx, oms1vy, oms1vz;
        public double OMS1VX
        {
            get { return oms1vx; }
            set
            {
                oms1vx = value;
                OnPropertyChanged("OMS1VX");
            }
        }
        public double OMS1VY
        {
            get { return oms1vy; }
            set
            {
                oms1vy = value;
                OnPropertyChanged("OMS1VY");
            }
        }
        public double OMS1VZ
        {
            get { return oms1vz; }
            set
            {
                oms1vz = value;
                OnPropertyChanged("OMS1VZ");
            }
        }

        private string oms2tig;
        public string OMS2TIG
        {
            get { return oms2tig; }
            set
            {
                oms2tig = value;
                OnPropertyChanged("OMS2TIG");
            }
        }
        private double oms2vx, oms2vy, oms2vz;
        public double OMS2VX
        {
            get { return oms2vx; }
            set
            {
                oms2vx = value;
                OnPropertyChanged("OMS2VX");
            }
        }
        public double OMS2VY
        {
            get { return oms2vy; }
            set
            {
                oms2vy = value;
                OnPropertyChanged("OMS2VY");
            }
        }
        public double OMS2VZ
        {
            get { return oms2vz; }
            set
            {
                oms2vz = value;
                OnPropertyChanged("OMS2VZ");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void OnPropertyChanged(string prop)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(prop));
            }
        }
    }
}
