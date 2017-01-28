using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	public enum MissionPhase
	{
		LaunchT20m,
		LaunchT9m,
		LaunchT31s,
		Orbit,
		EI,
		TAEM,
		AL
	}

	class Scenario : INotifyPropertyChanged
	{
		public struct MFDtype
		{
			public bool on;
			public string type;
			public List<string> _params;
		}

		public Scenario( Mission mission )
		{
			this.mission = mission;

			scnvessels = new List<OrbiterVessel>();
			SSU_LCC ssulcc = new SSU_LCC( mission );
			scnvessels.Add( ssulcc );
			SSU_Pad ssupad = new SSU_Pad( mission );
			scnvessels.Add( ssupad );
			SSU_MLP ssumlp = new SSU_MLP( mission );
			scnvessels.Add( ssumlp );
			SSUVessel ssuv = new SSUVessel( mission );
			scnvessels.Add( ssuv );
			// TODO other vessels

			missionphase = MissionPhase.LaunchT31s;// TODO

			mfds = new MFDtype[12];
			for (int i = 0; i < 12; i++) mfds[i]._params = new List<string>();
		}

		public void Create()
		{
			for (int i = 0; i < scnvessels.Count; i++) scnvessels[i].PreSave( missionphase );// TODO better name

			scnSystem = "Sol";

			switch (missionphase)
			{
				case MissionPhase.LaunchT20m:
					{
						int ms = Convert.ToInt32( 1000 * (mission.T0Second - (int)mission.T0Second) );
						DateTime dt = new DateTime( mission.T0Year, mission.T0Month, mission.T0Day, mission.T0Hour, mission.T0Minute, (int)mission.T0Second, ms );
						dt = dt.AddMinutes( -20.0 );// TODO should include holds
						scnYear = dt.Year;
						scnMonth = dt.Month;
						scnDay = dt.Day;
						scnHour = dt.Hour;
						scnMinute = dt.Minute;
						scnSecond = dt.Second + (0.001 * dt.Millisecond);
					}
					break;
				case MissionPhase.LaunchT9m:
					{
						int ms = Convert.ToInt32( 1000 * (mission.T0Second - (int)mission.T0Second) );
						DateTime dt = new DateTime( mission.T0Year, mission.T0Month, mission.T0Day, mission.T0Hour, mission.T0Minute, (int)mission.T0Second, ms );
						dt = dt.AddMinutes( -9.0 );
						scnYear = dt.Year;
						scnMonth = dt.Month;
						scnDay = dt.Day;
						scnHour = dt.Hour;
						scnMinute = dt.Minute;
						scnSecond = dt.Second + (0.001 * dt.Millisecond);
					}
					break;
				case MissionPhase.LaunchT31s:
					{
						int ms = Convert.ToInt32( 1000 * (mission.T0Second - (int)mission.T0Second) );
						DateTime dt = new DateTime( mission.T0Year, mission.T0Month, mission.T0Day, mission.T0Hour, mission.T0Minute, (int)mission.T0Second, ms );
						dt = dt.AddSeconds( -31.0 );
						scnYear = dt.Year;
						scnMonth = dt.Month;
						scnDay = dt.Day;
						scnHour = dt.Hour;
						scnMinute = dt.Minute;
						scnSecond = dt.Second + (0.001 * dt.Millisecond);
					}
					break;
			}

			scnContext = "SSU";
			scnShip = mission.OVname;
			scnCameraTarget = mission.OVname;
			scnCameraFOV = 40.0;

			scnCockpitType = 2;
			return;
		}

		public void Save( string scnfile )
		{
			System.IO.StreamWriter file = new System.IO.StreamWriter( scnfile );
			
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
			if (!String.IsNullOrEmpty( scnContext )) file.WriteLine( "  Context " + scnContext );
			if (!String.IsNullOrEmpty( scnScript )) file.WriteLine( "  Context " + scnScript );
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
				else if (scnCameraTrackMode == 3) file.WriteLine( "  TRACKMODE TargetTo " + scnCameraTrackModeRef );
				else if (scnCameraTrackMode == 4) file.WriteLine( "  TRACKMODE TargetFrom " + scnCameraTrackModeRef );
				else if (scnCameraTrackMode == 5)
				{
					file.WriteLine( "  TRACKMODE Ground " + scnCameraTrackModeRef );
					file.WriteLine( "  GROUNDLOCATION " + string.Format( "{0:f5} {1:f5} {2:f2}", scnCameraGrPosLon, scnCameraGrPosLat, scnCameraGrPosAlt ) );
					file.WriteLine( "  GROUNDDIRECTION " + string.Format( "{0:f2} {1:f2}", scnCameraGrDirH, scnCameraGrDirV ) );
				}
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

		Mission mission;
		MissionPhase missionphase;

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
		/// Scenario script
		/// (null if none)
		/// </summary>
		private string scnscript;
		public string scnScript
		{
			get { return scnscript; }
			set { scnscript = value; }
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
		/// Scenario camera track mode reference (for Target To, Target From and Ground modes only)
		/// </summary>
		private string scncameratrackmoderef;
		public string scnCameraTrackModeRef
		{
			get { return scncameratrackmoderef; }
			set
			{
				scncameratrackmoderef = value;
				OnPropertyChanged( "scnCameraTrackModeRef" );
			}
		}

		/// <summary>
		/// Scenario camera position for Ground mode
		/// </summary>
		private double scncameragrposlon;
		public double scnCameraGrPosLon
		{
			get { return scncameragrposlon; }
			set
			{
				scncameragrposlon = value;
				OnPropertyChanged( "scnCameraGrPosLon" );
			}
		}
		private double scncameragrposlat;
		public double scnCameraGrPosLat
		{
			get { return scncameragrposlat; }
			set
			{
				scncameragrposlat = value;
				OnPropertyChanged( "scnCameraGrPosLat" );
			}
		}
		private double scncameragrposalt;
		public double scnCameraGrPosAlt
		{
			get { return scncameragrposalt; }
			set
			{
				scncameragrposalt = value;
				OnPropertyChanged( "scnCameraGrPosAlt" );
			}
		}

		/// <summary>
		/// Scenario camera position for Ground mode
		/// </summary>
		private double scncameragrdirh;
		public double scnCameraGrDirH
		{
			get { return scncameragrdirh; }
			set
			{
				scncameragrdirh = value;
				OnPropertyChanged( "scnCameraGrDirH" );
			}
		}
		private double scncameragrdirv;
		public double scnCameraGrDirV
		{
			get { return scncameragrdirv; }
			set
			{
				scncameragrdirv = value;
				OnPropertyChanged( "scnCameraGrDirV" );
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
