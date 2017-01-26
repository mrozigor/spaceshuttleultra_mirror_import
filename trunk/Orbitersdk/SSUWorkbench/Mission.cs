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
			Bridgerails = new bool[13];
			for (int i = 0; i < 13; i++) Bridgerails[i] = false;
			PayloadZPos = new double[15];
			for (int i = 0; i < 15; i++) PayloadZPos[i] = -9999.0;

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
			launchsite = 0;
			launchpad = 0;
			mlp = 1;
			ettype = 2;
			etfrl = false;
			srmtype = 3;
			return;
		}

		public void Load( string missionfile )
		{
			string line;
			string key;
			string val;
			int eq = -1;

			System.IO.StreamReader file = new System.IO.StreamReader( missionfile );
			while ((line = file.ReadLine()) != null)
			{
				eq = line.IndexOf( "=" );

				if (eq != -1)
				{
					key = line.Substring( 0, eq );
					val = line.Substring( eq + 1, line.Length - eq - 1 );
					switch (key)
					{
						case "Name":
							MissionName = val;
							break;
						case "Orbiter":
							OVname = val;
							break;
						case "OrbiterTexture":
							OVtex = val;
							break;
						case "ET":
							if (val == "SWT") ETType = 0;
							else if (val == "LWT") ETType = 1;
							else ETType = 2;
							break;
						case "ETFRL":
							ETFRL = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "SRM":
							if (val == "SPM") SRMType = 0;
							else if (val == "HPM") SRMType = 1;
							else if (val == "FWC") SRMType = 2;
							else SRMType = 3;
							break;
						case "LaunchSite":
							if (val == "VAFB") LaunchSite = 1;
							else LaunchSite = 0;
							break;
						case "LaunchPad":
							if (val == "LC-39B") LaunchPad = 1;
							else LaunchPad = 0;
							LaunchSite = 0;// force KSC
							break;
						case "MLP":
							MLP = Convert.ToInt32( val ) - 1;
							LaunchSite = 0;// force KSC
							break;
						case "TargetInc":
							MECO_Inc = Convert.ToDouble( val );
							break;
						case "TargetLAN":
							MECO_LAN = Convert.ToDouble( val );
							break;
						case "MECOAlt":
							MECO_Alt = Convert.ToDouble( val );
							break;
						case "MECOVel":
							MECO_Vel = Convert.ToDouble( val );
							break;
						case "MECOFPA":
							MECO_FPA = Convert.ToDouble( val );
							break;
						case "PerformRollToHeadsUp":
							RTHU = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "OMSAssistEnable":
							OMSAssistEnable = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "OMSAssistDuration":
							OMSAssistDuration = Convert.ToDouble( val );
							break;
						case "MaxSSMEThrust":
							MaxThrust = Convert.ToDouble( val );
							break;
						case "ThrottleDown":
							QPOLY3 = Convert.ToDouble( val );
							break;
						case "ThrottleUp":
							QPOLY4 = Convert.ToDouble( val );
							break;
						case "UseRMS":
							RMS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseKUBand":
							KUbandAntenna = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseSTBDMPM":
							STBDMPMs = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseODS":
							ODS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseExtAL":
							ExtAL = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseTAA":
							TTA = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "AftTAA":
							AftTTA = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "HasBulkheadFloodlights":
							FwdBulkDockLights = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "HasDragChute":
							DragChute = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "Bridgerails":
							{
								int []num = Array.ConvertAll( val.Split( ',' ), Int32.Parse );
								foreach (int i in num) Bridgerails[i] = true;
							}
							break;
						/*case "PayloadZPos":
							// in default block
							break;*/
						case "SILTS":
							SILTS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "LogSSMEData":
							LogSSMEData = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseCISS":
							CISS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "CISS_GPrime":
							CISSGPrime = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseASE_IUS":
							ASE_IUS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "ASE_IUS_AftLocation":
							ASE_IUSAft = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						default:
							if (key.StartsWith( "PayloadZPos" ))
							{
								key = key.Substring( 11 );
								int id = Convert.ToInt32( key );
								if ((id >= 5) && (id <= 19)) PayloadZPos[id - 5] = Convert.ToDouble( val );
							}
							break;

					}
				}
			}
			file.Close();
			return;
		}

		public void Save( string missionfile )
		{
			System.IO.StreamWriter file = new System.IO.StreamWriter( missionfile );
			
			file.WriteLine( "Name=" + MissionName );
			file.WriteLine( "Orbiter=" + OVname );
			file.WriteLine( "OrbiterTexture=" + OVtex );
			if (ETType == 0)
			{
				file.WriteLine( "ET=SWT" );
				if (ETFRL) file.WriteLine( "ETFRL=TRUE" );
			}
			else if (ETType == 1) file.WriteLine( "ET=LWT" );
			else file.WriteLine( "ET=SLWT" );
			if (SRMType == 0) file.WriteLine( "SRM=SPM" );
			else if (SRMType == 1) file.WriteLine( "SRM=HPM" );
			else if (SRMType == 2) file.WriteLine( "SRM=FWC" );
			else file.WriteLine( "SRM=RSRM" );
			if (LaunchSite == 1) file.WriteLine( "LaunchSite=VAFB" );
			else
			{
				file.WriteLine( "LaunchSite=KSC" );
				if (LaunchPad == 1) file.WriteLine( "LaunchPad=LC-39B" );
				else file.WriteLine( "LaunchPad=LC-39A" );
				file.WriteLine( "MLP=" + (MLP + 1) );
			}
			file.WriteLine( "TargetInc=" + string.Format( "{0:f6}", MECO_Inc ) );
			file.WriteLine( "TargetLAN=" + string.Format( "{0:f6}", MECO_LAN ) );
			file.WriteLine( "MECOAlt=" + string.Format( "{0:f6}", MECO_Alt ) );
			file.WriteLine( "MECOVel=" + string.Format( "{0:f6}", MECO_Vel ) );
			file.WriteLine( "MECOFPA=" + string.Format( "{0:f6}", MECO_FPA ) );
			file.WriteLine( "PerformRollToHeadsUp=" + (RTHU ? "TRUE" : "FALSE") );
			file.WriteLine( "OMSAssistEnable=" + (OMSAssistEnable ? "TRUE" : "FALSE") );
			file.WriteLine( "OMSAssistDuration=" + string.Format( "{0:f6}", OMSAssistDuration ) );
			file.WriteLine( "MaxSSMEThrust=" + MaxThrust );
			file.WriteLine( "ThrottleDown=" + string.Format( "{0:f3}", QPOLY3 ) );
			file.WriteLine( "ThrottleUp=" + string.Format( "{0:f3}", QPOLY4 ) );
			file.WriteLine( "UseRMS=" + (RMS ? "TRUE" : "FALSE") );
			file.WriteLine( "UseKUBand=" + (KUbandAntenna ? "TRUE" : "FALSE") );
			file.WriteLine( "UseSTBDMPM=" + (STBDMPMs ? "TRUE" : "FALSE") );
			file.WriteLine( "UseODS=" + (ODS ? "TRUE" : "FALSE") );
			file.WriteLine( "UseExtAL=" + (ExtAL ? "TRUE" : "FALSE") );
			file.WriteLine( "UseTAA=" + (TTA ? "TRUE" : "FALSE") );
			file.WriteLine( "AftTAA=" + (AftTTA ? "TRUE" : "FALSE") );
			file.WriteLine( "HasBulkheadFloodlights=" + (FwdBulkDockLights ? "TRUE" : "FALSE") );
			file.WriteLine( "HasDragChute=" + (DragChute ? "TRUE" : "FALSE") );
			string str = "";
			for (int i = 0; i < 13; i++)
			{
				if (Bridgerails[i]) str = str + i + ",";
			}
			if (str.Length > 0) file.WriteLine( "Bridgerails=" + str.Substring( 0, str.Length - 1 ) );
			for (int i = 0; i < 15; i++)
			{
				if (PayloadZPos[i] != -9999.0) file.WriteLine( "PayloadZPos" + (i + 5) + "=" + PayloadZPos[i] );
			}
			file.WriteLine( "SILTS=" + (SILTS ? "TRUE" : "FALSE") );
			file.WriteLine( "LogSSMEData=" + (LogSSMEData ? "TRUE" : "FALSE") );
			file.WriteLine( "UseCISS=" + (CISS ? "TRUE" : "FALSE") );
			file.WriteLine( "CISS_GPrime=" + (CISSGPrime ? "TRUE" : "FALSE") );
			file.WriteLine( "UseASE_IUS=" + (ASE_IUS ? "TRUE" : "FALSE") );
			file.WriteLine( "ASE_IUS_AftLocation=" + (ASE_IUSAft ? "TRUE" : "FALSE") );
			
			file.Close();
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
		/// Is bridgerail installed
		/// </summary>
		private bool[] Bridgerails;

		/// <summary>
		/// Payload attachment offset
		/// </summary>
		private double[] PayloadZPos;

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

		/// <summary>
		/// Launch Site
		/// 0 = KSC
		/// 1 = VAFB
		/// </summary>
		private int launchsite;
		public int LaunchSite
		{
			get { return launchsite; }
			set
			{
				launchsite = value;
				OnPropertyChanged( "LaunchSite" );
			}
		}

		/// <summary>
		/// Launch Pad
		/// 0 = LC-39A (for KSC launch site only)
		/// 1 = LC-39B (for KSC launch site only)
		/// (0 = SLC-6 (for VAFB launch site only))
		/// </summary>
		private int launchpad;
		public int LaunchPad
		{
			get { return launchpad; }
			set
			{
				launchpad = value;
				OnPropertyChanged( "LaunchPad" );
			}
		}

		/// <summary>
		/// MLP number (for KSC launch site only)
		/// 0 = MLP-1
		/// 1 = MLP-2
		/// 2 = MLP-3
		/// </summary>
		private int mlp;
		public int MLP
		{
			get { return mlp; }
			set
			{
				mlp = value;
				OnPropertyChanged( "MLP" );
			}
		}

		/// <summary>
		/// ET type
		/// 0 = SWT
		/// 1 = LWT
		/// 2 = SLWT
		/// </summary>
		private int ettype;
		public int ETType
		{
			get { return ettype; }
			set
			{
				ettype = value;
				OnPropertyChanged( "ETType" );
			}
		}

		/// <summary>
		/// Does the ET have FRL (for SWT ET only)
		/// </summary>
		private bool etfrl;
		public bool ETFRL
		{
			get { return etfrl; }
			set
			{
				etfrl = value;
				OnPropertyChanged( "ETFRL" );
			}
		}

		/// <summary>
		/// SRM type
		/// 0 = SRM (SPM)
		/// 1 = SRM (HPM)
		/// 2 = FWC
		/// 3 = RSRM
		/// </summary>
		private int srmtype;
		public int SRMType
		{
			get { return srmtype; }
			set
			{
				srmtype = value;
				OnPropertyChanged( "SRMType" );
			}
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
