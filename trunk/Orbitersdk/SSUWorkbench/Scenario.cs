using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class Scenario : INotifyPropertyChanged
	{
		public struct MFDtype
		{
			public bool on;
			public string type;
			public List<string> _params;
		}

		public Scenario()
		{
			scnvessels = new List<OrbiterVessel>();
			mfds = new MFDtype[12];
			for (int i = 0; i < 12; i++) mfds[i]._params = new List<string>();
		}

		public void Load( string scnfile, Mission mission, string orbiterpath )
		{
			// TODO missing scenario parameters:
			// Date JD
			// Date JE
			// Help
			// Script
			// GROUNDLOCATION
			// GROUNDDIRECTION
			// BEGIN_PRESET
			// REF (HUD)

			string line;

			System.IO.StreamReader file = new System.IO.StreamReader( scnfile );
			while ((line = file.ReadLine()) != null)
			{
				line = line.TrimStart( ' ' );
				// TODO parse scenario file
				if (line == "BEGIN_DESC")
				{
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_DESC") break;
						else
						{
							if (Description == null) Description = line;
							else Description += "\r\n" + line;
						}
					}
				}
				else if (line == "BEGIN_ENVIRONMENT" )
				{
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_ENVIRONMENT") break;
						else if (line.StartsWith( "System " ))
						{
							scnSystem = line.Substring( 7, line.Length - 7 );
						}
						else if (line.StartsWith( "Date MJD " ))
						{
							double num = Convert.ToDouble( line.Substring( 9, line.Length - 9 ) ) - 15018.0;
							DateTime dt = DateTime.FromOADate( num );
							scnYear = dt.Year;
							scnMonth = dt.Month;
							scnDay = dt.Day;
							scnHour = dt.Hour;
							scnMinute = dt.Minute;
							scnSecond = dt.Second + (0.001 * dt.Millisecond);
						}
						else if (line.StartsWith( "Context " ))
						{
							scnContext = line.Substring( 8, line.Length - 8 );
						}
					}
				}
				else if (line == "BEGIN_FOCUS" )
				{
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_FOCUS") break;
						else if (line.StartsWith( "Ship " ))
						{
							scnShip = line.Substring( 5, line.Length - 5 );
						}
					}
				}
				else if (line == "BEGIN_CAMERA" )
				{
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_CAMERA") break;
						else if (line.StartsWith( "TARGET " ))
						{
							scnCameraTarget = line.Substring( 7, line.Length - 7 );
						}
						else if (line.StartsWith( "MODE " ))
						{
							if (line.Substring( 5, line.Length - 5 ) == "Extern") scnCameraMode = 1;
						}
						else if (line.StartsWith( "POS " ))
						{
							string tmp = line.Substring( 4, line.Length - 4 );
							double []num = Array.ConvertAll( tmp.Split( ' ' ), Double.Parse );
							if (num.Count() == 3)
							{
								scnCameraPosX = num[0];
								scnCameraPosY = num[1];
								scnCameraPosZ = num[2];
							}
						}
						else if (line.StartsWith( "TRACKMODE " ))
						{
							string tmp = line.Substring( 10, line.Length - 10 );
							if (tmp.StartsWith( "AbsoluteDirection" )) scnCameraTrackMode = 1;
							else if (tmp.StartsWith( "GlobalFrame" )) scnCameraTrackMode = 2;
							else if (tmp.StartsWith( "TargetTo" ))
							{
								scnCameraTrackMode = 3;
								scnCameraTrackModeTarget = tmp.Substring( 9 );
							}
							else if (tmp.StartsWith( "TargetFrom" ))
							{
								scnCameraTrackMode = 4;
								scnCameraTrackModeTarget = tmp.Substring( 11 );
							}
							else if (tmp.StartsWith( "Ground" ))
							{
								scnCameraTrackMode = 5;
								scnCameraTrackModeTarget = tmp.Substring( 7 );
							}
							else /*if (tmp.StartsWith( "TargetRelative" ))*/ scnCameraTrackMode = 0;
						}
						else if (line.StartsWith( "FOV " ))
						{
							scnCameraFOV = Convert.ToDouble( line.Substring( 4, line.Length - 4 ) );
						}
					}
				}
				else if (line == "BEGIN_HUD" )
				{
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_HUD") break;
						else if (line.StartsWith( "TYPE " ))
						{
							string tmp = line.Substring( 5, line.Length - 5 );
							if (tmp == "Orbit") scnHUDType = 1;
							else if (tmp == "Surface") scnHUDType = 2;
							else scnHUDType = 3;
						}
						else if (line.StartsWith( "REF " ))
						{
							scnHUDRef = line.Substring( 4, line.Length - 4 );
						}
						else if (line.StartsWith( "NAV " ))
						{
							scnHUDNAV = Convert.ToInt32( line.Substring( 4, line.Length - 4 ) );
						}
					}
				}
				else if (line.StartsWith( "BEGIN_MFD " ))
				{
					string strid = line.Substring( 10, line.Length - 10 );
					int id = 0;
					if (strid == "Left") id = 0;
					else if (strid == "Right") id = 1;
					else id = Convert.ToInt32( strid ) - 1;
					mfds[id].on = true;

					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_MFD") break;
						else if (line.StartsWith( "TYPE " ))
						{
							mfds[id].type = line.Substring( 5, line.Length - 5 );
						}
						else
						{
							mfds[id]._params.Add( line );
						}
					}
				}
				else if (line.StartsWith( "BEGIN_PANEL" ))
				{
					scnCockpitType = 1;
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_PANEL") break;
					}
				}
				else if (line.StartsWith( "BEGIN_VC" ))
				{
					scnCockpitType = 2;
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_VC") break;
					}
				}
				else if (line.StartsWith( "BEGIN_SHIPS" ))
				{
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_SHIPS") break;
						else if (line.EndsWith( ":SpaceShuttleUltra" ))
						{
							// main vessel
							SSUVessel ssuv = new SSUVessel( mission, orbiterpath );
							ssuv.Load( line, file );
							scnvessels.Add( ssuv );
						}
						//else if (line.EndsWith( ":SSU_CentaurG" ))
						//{
						//	// TODO centaur G
						//}
						//else if (line.EndsWith( ":SSU_CentaurGPrime" ))
						//{
						//	// TODO centaur G'
						//}
						//else if (line.EndsWith( ":SSU_Chute" ))
						//{
						//	// TODO chute
						//}
						//else if (line.EndsWith( ":SSU_CT" ))
						//{
						//	// TODO ct
						//}
						//else if (line.EndsWith( ":SSU_CT_1980" ))
						//{
						//	// TODO ct 1980
						//}
						//else if (line.EndsWith( ":SSU_DemoSat" ))
						//{
						//	// TODO demosat
						//}
						//else if (line.EndsWith( ":SSU_DFI_pallet" ))
						//{
						//	// TODO dfi pallet
						//}
						//else if (line.EndsWith( ":SSU_IUS" ))
						//{
						//	// TODO ius
						//}
						//else if (line.EndsWith( ":SSU_IUS_STAGE1" ))
						//{
						//	// TODO ius 1st stg
						//}
						//else if (line.EndsWith( ":SSU_LCC" ))
						//{
						//	// TODO lcc
						//}
						//else if (line.EndsWith( ":SSU_LSRB" ))
						//{
						//	// TODO l srb
						//}
						//else if (line.EndsWith( ":SSU_LSRB_FWC" ))
						//{
						//	// TODO l srb fwc
						//}
						//else if (line.EndsWith( ":SSU_LSRB_RSRM" ))
						//{
						//	// TODO l srb rsrm
						//}
						//else if (line.EndsWith( ":SSU_LWT" ))
						//{
						//	// TODO et lwt
						//}
						//else if (line.EndsWith( ":SSU_MLP" ))
						//{
						//	// TODO mlp
						//}
						//else if (line.EndsWith( ":SSU_OBSS" ))
						//{
						//	// TODO obss
						//}
						//else if (line.EndsWith( ":SSU_Pad" ))
						//{
						//	// TODO pad39
						//}
						//else if (line.EndsWith( ":SSU_Pad1985" ))
						//{
						//	// TODO pad39 1985
						//}
						//else if (line.EndsWith( ":SSU_RSRB" ))
						//{
						//	// TODO r srb
						//}
						//else if (line.EndsWith( ":SSU_RSRB_FWC" ))
						//{
						//	// TODO r srb fwc
						//}
						//else if (line.EndsWith( ":SSU_RSRB_RSRM" ))
						//{
						//	// TODO r srb rsrm
						//}
						//else if (line.EndsWith( ":SSU_SLC6" ))
						//{
						//	// TODO slc6
						//}
						//else if (line.EndsWith( ":SSU_SLWT" ))
						//{
						//	// TODO et slwt
						//}
						//else if (line.EndsWith( ":SSU_SWT" ))
						//{
						//	// TODO et swt
						//}
						//else if (line.EndsWith( ":SSU_VAB" ))
						//{
						//	// TODO vab
						//}
						//else if (line.EndsWith( ":SSU_Xenon_Lights" ))
						//{
						//	// TODO lights
						//}
						else
						{
							// other non-SSU vessel
							OrbiterVessel ship = new OrbiterVessel();
							ship.Load( line, file );
							scnvessels.Add( ship );
						}
					}
				}
			}
			file.Close();

			return;
		}

		public void Save( string scnfile, string orbiterpath )
		{
			System.IO.StreamWriter file = new System.IO.StreamWriter( scnfile );
			// TODO write scn file
			////////////////// description //////////////////
			file.WriteLine( "BEGIN_DESC" );
			file.WriteLine( Description );
			file.WriteLine( "END_DESC" );
			file.WriteLine( "" );

			////////////////// environment //////////////////
			file.WriteLine( "BEGIN_ENVIRONMENT" );
			file.WriteLine( "  System " + scnSystem );
			int ms = Convert.ToInt32( 1000 * (scnSecond - (int)scnSecond) );
			DateTime dt = new DateTime( scnYear, scnMonth, scnDay, scnHour, scnMinute, (int)scnSecond, ms );
			file.WriteLine( "  Date MJD " + string.Format( "{0:f10}", dt.ToOADate() + 15018.0 ) );
			if (scnContext != null) file.WriteLine( "  Context " + scnContext );
			file.WriteLine( "END_ENVIRONMENT" );
			file.WriteLine( "" );

			////////////////// focus //////////////////
			file.WriteLine( "BEGIN_FOCUS" );
			file.WriteLine( "  Ship " + scnShip );
			file.WriteLine( "END_FOCUS\r\n" );

			////////////////// camera //////////////////
			file.WriteLine( "BEGIN_CAMERA" );
			/*if (scnCameraMode == 1)*/ file.WriteLine( "  TARGET " + scnCameraTarget );// seems to be used on both camera modes
			if (scnCameraMode == 0) file.WriteLine( "  MODE Cockpit" );
			else file.WriteLine( "  MODE Extern" );
			if (scnCameraMode == 1) file.WriteLine( "  POS " + string.Format( "{0:f6} {1:f6} {2:f6}", scnCameraPosX, scnCameraPosY, scnCameraPosZ ) );
			if (scnCameraMode == 1)
			{
				if (scnCameraTrackMode == 1) file.WriteLine( "  TRACKMODE AbsoluteDirection" );
				else if (scnCameraTrackMode == 2) file.WriteLine( "  TRACKMODE GlobalFrame" );
				else if (scnCameraTrackMode == 1) file.WriteLine( "  TRACKMODE TargetTo " + scnCameraTrackModeTarget );
				else if (scnCameraTrackMode == 1) file.WriteLine( "  TRACKMODE TargetFrom " + scnCameraTrackModeTarget );
				else if (scnCameraTrackMode == 1) file.WriteLine( "  TRACKMODE Ground " + scnCameraTrackModeTarget );
				else /*if (scnCameraTrackMode == 0)*/ file.WriteLine( "  TRACKMODE TargetRelative" );
			}
			file.WriteLine( "  FOV " + string.Format( "{0:f2}", scnCameraFOV ) );
			file.WriteLine( "END_CAMERA" );
			file.WriteLine( "" );

			////////////////// hud //////////////////
			if (scnHUDType != 0)
			{
				file.WriteLine( "BEGIN_HUD" );
				if (scnHUDType == 1)
				{
					file.WriteLine( "  TYPE Orbit" );
					if (!String.IsNullOrEmpty( scnHUDRef )) file.WriteLine( "  REF " + scnHUDRef );
				}
				else if (scnHUDType == 2) file.WriteLine( "  TYPE Surface" );
				else
				{
					file.WriteLine( "  TYPE Docking" );
					if (scnHUDNAV != -1) file.WriteLine( "  NAV " + scnHUDNAV );
				}
				file.WriteLine( "END_HUD" );
				file.WriteLine( "" );
			}

			////////////////// mfd //////////////////
			string[] MFDname = {"Left", "Right", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
			for (int i = 0; i < 12; i++)
			{
				if (mfds[i].on)
				{
					file.WriteLine( "BEGIN_MFD " + MFDname[i] );
					file.WriteLine( "  TYPE " + mfds[i].type );
					for (int k = 0; k < mfds[i]._params.Count; k++)
					{
						file.WriteLine( "  " + mfds[i]._params[k] );
					}
					file.WriteLine( "END_MFD" );
					file.WriteLine( "" );
				}
			}

			////////////////// panel //////////////////
			if (scnCockpitType == 1)
			{
				file.WriteLine( "BEGIN_PANEL" );
				file.WriteLine( "END_PANEL" );
				file.WriteLine( "" );
			}

			////////////////// vc //////////////////
			if (scnCockpitType == 2)
			{
				file.WriteLine( "BEGIN_VC" );
				file.WriteLine( "END_VC" );
				file.WriteLine( "" );
			}

			////////////////// ships //////////////////
			file.WriteLine( "BEGIN_SHIPS" );
			for (int i = 0; i < scnvessels.Count; i++) scnvessels[i].Save( file );
			file.WriteLine( "END_SHIPS" );

			file.Close();

			return;
		}

		/// <summary>
		/// List of vessels used in this mission
		/// </summary>
		public List<OrbiterVessel> scnvessels;

		/// <summary>
		/// Description to be used in the scenario file 
		/// </summary>
		private string description;
		public string Description
		{
			get { return description; }
			set
			{
				description = value;
				OnPropertyChanged( "Description" );
			}
		}

		/// <summary>
		/// Scenario system
		/// </summary>
		private string scnsystem;
		public string scnSystem
		{
			get { return scnsystem; }
			set { scnsystem = value; }
		}
		/// <summary>
		/// The date/time at the start of the simulation
		/// </summary>
		private int scnyear;
		public int scnYear
		{
			get { return scnyear; }
			set
			{
				scnyear = value;
				OnPropertyChanged( "scnYear" );
			}
		}
		private int scnmonth;
		public int scnMonth
		{
			get { return scnmonth; }
			set
			{
				scnmonth = value;
				OnPropertyChanged( "scnMonth" );
			}
		}
		private int scnday;
		public int scnDay
		{
			get { return scnday; }
			set
			{
				scnday = value;
				OnPropertyChanged( "scnDay" );
			}
		}
		private int scnhour;
		public int scnHour
		{
			get { return scnhour; }
			set
			{
				scnhour = value;
				OnPropertyChanged( "scnHour" );
			}
		}
		private int scnminute;
		public int scnMinute
		{
			get { return scnminute; }
			set
			{
				scnminute = value;
				OnPropertyChanged( "scnMinute" );
			}
		}
		private double scnsecond;
		public double scnSecond
		{
			get { return scnsecond; }
			set
			{
				scnsecond = value;
				OnPropertyChanged( "scnSecond" );
			}
		}

		/// <summary>
		/// Scenario context
		/// (null if none)
		/// </summary>
		private string scncontext;
		public string scnContext
		{
			get { return scncontext; }
			set { scncontext = value; }
		}
		/// <summary>
		/// Ship controlled in the scenario
		/// </summary>
		private string scnship;
		public string scnShip
		{
			get { return scnship; }
			set { scnship = value; }
		}
		/// <summary>
		/// Scenario camera target
		/// </summary>
		private string scncameratarget;
		public string scnCameraTarget
		{
			get { return scncameratarget; }
			set { scncameratarget = value; }
		}
		
		/// <summary>
		/// Scenario camera mode
		/// 0 = Cockpit
		/// 1 = Extern
		/// </summary>
		private int scncameramode;
		public int scnCameraMode
		{
			get { return scncameramode; }
			set
			{
				scncameramode = value;
				OnPropertyChanged( "scnCameraMode" );
			}
		}
		/// <summary>
		/// Scenario camera field of view
		/// </summary>
		private double scncamerafov;
		public double scnCameraFOV
		{
			get { return scncamerafov; }
			set
			{
				scncamerafov = value;
				OnPropertyChanged( "scnCameraFOV" );
			}
		}
		/// <summary>
		/// Scenario camera position (for External camera mode only)
		/// </summary>
		private double scncameraposx;
		public double scnCameraPosX
		{
			get { return scncameraposx; }
			set
			{
				scncameraposx = value;
				OnPropertyChanged( "scnCameraPosX" );
			}
		}
		private double scncameraposy;
		public double scnCameraPosY
		{
			get { return scncameraposy; }
			set
			{
				scncameraposy = value;
				OnPropertyChanged( "scnCameraPosY" );
			}
		}
		private double scncameraposz;
		public double scnCameraPosZ
		{
			get { return scncameraposz; }
			set
			{
				scncameraposz = value;
				OnPropertyChanged( "scnCameraPosZ" );
			}
		}
		/// <summary>
		/// Scenario camera track mode (for External camera mode only)
		/// 0 = Target Relative
		/// 1 = Absolute Direction
		/// 2 = Global Frame
		/// 3 = Target To
		/// 4 = Target From
		/// 5 = Ground
		/// </summary>
		private int scncameratrackmode;
		public int scnCameraTrackMode
		{
			get { return scncameratrackmode; }
			set
			{
				scncameratrackmode = value;
				OnPropertyChanged( "scnCameraTrackMode" );
			}
		}
		/// <summary>
		/// Scenario camera track mode target (for Target To, Target From and Ground modes only)
		/// </summary>
		private string scncameratrackmodetarget;
		public string scnCameraTrackModeTarget
		{
			get { return scncameratrackmodetarget; }
			set
			{
				scncameratrackmodetarget = value;
				OnPropertyChanged( "scnCameraTrackModeTarget" );
			}
		}
		/// <summary>
		/// Scenario cockpit type
		/// 0 = normal
		/// 1 = 2D
		/// 2 = 3D
		/// </summary>
		private int scncockpittype;
		public int scnCockpitType
		{
			get { return scncockpittype; }
			set
			{
				scncockpittype = value;
				OnPropertyChanged( "scnCockpitType" );
			}
		}
		
		/// <summary>
		/// HUD type
		/// 0 = (off)
		/// 1 = Orbit
		/// 2 = Surface
		/// 3 = Docking
		/// </summary>
		private int scnhudtype;
		public int scnHUDType
		{
			get { return scnhudtype; }
			set
			{
				scnhudtype = value;
				OnPropertyChanged( "scnHUDType" );
			}
		}

		/// <summary>
		/// HUD reference (Orbit type only)
		/// </summary>
		private string scnhudref;
		public string scnHUDRef
		{
			get { return scnhudref; }
			set
			{
				scnhudref = value;
				OnPropertyChanged( "scnHUDRef" );
			}
		}

		/// <summary>
		/// HUD nav (Docking type only)
		/// </summary>
		private int scnhudnav;
		public int scnHUDNAV
		{
			get { return scnhudnav; }
			set
			{
				scnhudnav = value;
				OnPropertyChanged( "scnHUDNAV" );
			}
		}

		private MFDtype[] mfds;

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
