using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	/// <summary>
	/// Description of a mission in a program.
	/// </summary>
	/// <remarks>
	/// <para>
	/// Describes the resources used in a mission, the launch and landing parameters and the important milestone dates.
	/// For historic reproductions of missions and events, its possible to define additional launch attempts and anomalies.
	/// For free-style missions, it will be possible to define launch window constraints.
	/// </para>
	/// <para>
	/// A mission that is created independent of a program, will get created with a minimal stub program to define its resources.
	/// </para>
	/// </remarks>
	/// <seealso cref="SSUWorkbench.model.STSProgram"/>
	class Mission : INotifyPropertyChanged
	{
		public Mission()
		{
			LoadDefault();
		}

		public void LoadDefault()
		{
			//description = "The STS-101 launch scenario, as simulated by Space Shuttle Ultra.";
			missionName = "STS-101";
			ovname = "Atlantis";
			ovtex = "Atlantis_5thmod";
			meco_inc = 51.62;
			meco_lan = 0;
			meco_alt = 105564;
			meco_vel = 7903.449390;
			meco_fpa = 0.708380;
			rthu = true;
			omsassistena = true;
			omsassistdur = 59.0;
			maxthrust = 104.5;
			qpoly3 = 792.0;
			qpoly4 = 1304.0;
			rms = true;
			kubandantenna = true;
			stbdmpms = false;
			ods = true;
			extal = true;
			tta = false;
			afttta = false;
			fwdbulkdocklights = true;
			dragchute = true;
			silts = false;
			logssmedata = false;
			ciss = false;
			cissgprime = true;
			ase_ius = false;
			ase_iusaft = false;
			return;
		}

		/// <summary>
		/// The program, to which the mission belongs.
		/// </summary>
		private STSProgram program;

		/// <summary>
		/// The name of the launch site
		/// </summary>
		private string launchSiteName;

		/// <summary>
		/// The orbiter vehicle used in this mission
		/// </summary>
		private Orbiter orbitervehicle;

		/// <summary>
		/// The name of the launch site
		/// </summary>
		public string LaunchSiteName
		{
			get { return launchSiteName; }
		}

		/// <summary>
		/// The name of the mission
		/// </summary>
		private string missionName;
		public string MissionName
		{
			get { return missionName; }
			set
			{
				missionName = value;
				OnPropertyChanged( "MissionName" );
			}
		}

		/// <summary>
		/// The orbiter vehicle used in this mission
		/// </summary>
		public Orbiter OV
		{
			get { return orbitervehicle; }
			set { orbitervehicle = value; }
		}

		/// <summary>
		/// Name of OV used in this mission
		/// </summary>
		private string ovname;
		public string OVname
		{
			get { return ovname; }
			set
			{
				if ((value == "Columbia") || 
					(value == "Challenger") || 
					(value == "Discovery") || 
					(value == "Atlantis") || 
					(value == "Endeavour"))
				{
					ovname = value;
				}
				else
				{
					ovname = "Atlantis";
				}
			}
		}
		public int OVnameCB
		{
			get
			{
				switch (ovname)
				{
					case "Columbia":
						return 0;
					case "Challenger":
						return 1;
					case "Discovery":
						return 2;
					case "Atlantis":
						return 3;
					case "Endeavour":
						return 4;
					default:
						return 3;
				}
			}
			set
			{
				switch (value)
				{
					case 0:
						ovname = "Columbia";
						break;
					case 1:
						ovname = "Challenger";
						break;
					case 2:
						ovname = "Discovery";
						break;
					case 3:
						ovname = "Atlantis";
						break;
					case 4:
						ovname = "Endeavour";
						break;
					default:
						ovname = "Atlantis";
						break;
				}
			}
		}

		/// <summary>
		/// The name of the OV texture
		/// </summary>
		private string ovtex;
		public string OVtex
		{
			get { return ovtex; }
			set { ovtex = value; }
		}

		/// <summary>
		/// MECO target inclination
		/// </summary>
		private double meco_inc;
		public double MECO_Inc
		{
			get { return meco_inc; }
			set { meco_inc = value; }
		}

		/// <summary>
		/// MECO target longitude of ascending node
		/// </summary>
		private double meco_lan;
		public double MECO_LAN
		{
			get { return meco_lan; }
			set { meco_lan = value; }
		}

		/// <summary>
		/// MECO target altitude
		/// </summary>
		private double meco_alt;
		public double MECO_Alt
		{
			get { return meco_alt; }
			set { meco_alt = value; }
		}

		/// <summary>
		/// MECO target velocity
		/// </summary>
		private double meco_vel;
		public double MECO_Vel
		{
			get { return meco_vel; }
			set { meco_vel = value; }
		}

		/// <summary>
		/// MECO target flight path angle
		/// </summary>
		private double meco_fpa;
		public double MECO_FPA
		{
			get { return meco_fpa; }
			set { meco_fpa = value; }
		}

		/// <summary>
		/// Perform roll-to-heads-up
		/// </summary>
		private bool rthu;
		public bool RTHU
		{
			get { return rthu; }
			set
			{
				rthu = value;
				OnPropertyChanged( "RTHU" );
			}
		}

		/// <summary>
		/// Perform OMS assist
		/// </summary>
		private bool omsassistena;
		public bool OMSAssistEnable
		{
			get { return omsassistena; }
			set
			{
				omsassistena = value;
				OnPropertyChanged( "OMSAssistEnable" );
			}
		}

		/// <summary>
		/// OMS assist duration
		/// </summary>
		private double omsassistdur;
		public double OMSAssistDuration
		{
			get { return omsassistdur; }
			set
			{
				omsassistdur = value;
				OnPropertyChanged( "OMSAssistDuration" );
			}
		}

		/// <summary>
		/// SSME max thrust
		/// </summary>
		private double maxthrust;
		public double MaxThrust
		{
			get { return maxthrust; }
			set
			{
				maxthrust = value;
				OnPropertyChanged( "MaxThrust" );
			}
		}

		/// <summary>
		/// SSME 1º stage throttle down velocity (QPOLY3)
		/// </summary>
		private double qpoly3;
		public double QPOLY3
		{
			get { return qpoly3; }
			set { qpoly3 = value; }
		}

		/// <summary>
		/// SSME 1º stage throttle up velocity (QPOLY4)
		/// </summary>
		private double qpoly4;
		public double QPOLY4
		{
			get { return qpoly4; }
			set { qpoly4 = value; }
		}

		/// <summary>
		/// Is the RMS is installed
		/// </summary>
		private bool rms;
		public bool RMS
		{
			get { return rms; }
			set
			{
				rms = value;
				OnPropertyChanged( "RMS" );
			}
		}

		/// <summary>
		/// Is the KU-band Antenna installed
		/// </summary>
		private bool kubandantenna;
		public bool KUbandAntenna
		{
			get { return kubandantenna; }
			set
			{
				kubandantenna = value;
				OnPropertyChanged( "KUbandAntenna" );
			}
		}

		/// <summary>
		/// Are the starboard MPMs installed
		/// </summary>
		private bool stbdmpms;
		public bool STBDMPMs
		{
			get { return stbdmpms; }
			set
			{
				stbdmpms = value;
				OnPropertyChanged( "STBDMPMs" );
			}
		}

		/// <summary>
		/// Is the ODS installed
		/// </summary>
		private bool ods;
		public bool ODS
		{
			get { return ods; }
			set
			{
				ods = value;
				OnPropertyChanged( "ODS" );
			}
		}

		/// <summary>
		/// Is the External A/L installed
		/// </summary>
		private bool extal;
		public bool ExtAL
		{
			get { return extal; }
			set
			{
				extal = value;
				OnPropertyChanged( "ExtAL" );
			}
		}

		/// <summary>
		/// Is the TTA installed
		/// </summary>
		private bool tta;
		public bool TTA
		{
			get { return tta; }
			set { tta = value; }
		}

		/// <summary>
		/// If installed, is the TTA in the aft position
		/// </summary>
		private bool afttta;
		public bool AftTTA
		{
			get { return afttta; }
			set { afttta = value; }
		}

		/// <summary>
		/// Are the forward bulkhead and docking lights installed
		/// </summary>
		private bool fwdbulkdocklights;
		public bool FwdBulkDockLights
		{
			get { return fwdbulkdocklights; }
			set
			{
				fwdbulkdocklights = value;
				OnPropertyChanged( "FwdBulkDockLights" );
			}
		}

		/// <summary>
		/// Is the Drag Chute installed
		/// </summary>
		private bool dragchute;
		public bool DragChute
		{
			get { return dragchute; }
			set
			{
				dragchute = value;
				OnPropertyChanged( "DragChute" );
			}
		}

		/// <summary>
		/// Is the SILTS pod installed
		/// </summary>
		private bool silts;
		public bool SILTS
		{
			get { return silts; }
			set
			{
				silts = value;
				OnPropertyChanged( "SILTS" );
			}
		}

		/// <summary>
		/// Is SSME data logging enabled
		/// </summary>
		private bool logssmedata;
		public bool LogSSMEData
		{
			get { return logssmedata; }
			set
			{
				logssmedata = value;
				OnPropertyChanged( "LogSSMEData" );
			}
		}

		/// <summary>
		/// Is the CISS installed
		/// </summary>
		private bool ciss;
		public bool CISS
		{
			get { return ciss; }
			set { ciss = value; }
		}

		/// <summary>
		/// If installed, is the CISS of the G Prime type
		/// </summary>
		private bool cissgprime;
		public bool CISSGPrime
		{
			get { return cissgprime; }
			set { cissgprime = value; }
		}

		/// <summary>
		/// Is the ASE for the IUS installed
		/// </summary>
		private bool ase_ius;
		public bool ASE_IUS
		{
			get { return ase_ius; }
			set { ase_ius = value; }
		}

		/// <summary>
		/// If installed, is the ASE for the IUS in the aft position
		/// </summary>
		private bool ase_iusaft;
		public bool ASE_IUSAft
		{
			get { return ase_iusaft; }
			set { ase_iusaft = value; }
		}


		public event PropertyChangedEventHandler PropertyChanged;

		private void OnPropertyChanged( string prop )
		{
			if (PropertyChanged != null)
			{
				PropertyChanged( this, new PropertyChangedEventArgs( prop ) );
			}
		}
	}
}
