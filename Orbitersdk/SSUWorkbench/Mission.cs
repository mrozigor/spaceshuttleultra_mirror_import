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
			set { missionName = value; }
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
		private DateTime scndatetime;
		public DateTime scnDatetime
		{
			get { return scndatetime; }
			set { scndatetime = value; }
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
			set { scncameramode = value; }
		}
		/// <summary>
		/// Scenario camera field of view
		/// </summary>
		private double scncamerafov;
		public double scnCameraFOV
		{
			get { return scncamerafov; }
			set { scncamerafov = value; }
		}
		/// <summary>
		/// Scenario camera position
		/// </summary>
		private double scncameraposx;
		public double scnCameraPosX
		{
			get { return scncameraposx; }
			set { scncameraposx = value; }
		}
		private double scncameraposy;
		public double scnCameraPosY
		{
			get { return scncameraposy; }
			set { scncameraposy = value; }
		}
		private double scncameraposz;
		public double scnCameraPosZ
		{
			get { return scncameraposz; }
			set { scncameraposz = value; }
		}
		/// <summary>
		/// Scenario camera track mode
		/// </summary>
		private string scncameratrackmode;
		public string scnCameraTrackMode
		{
			get { return scncameratrackmode; }
			set { scncameratrackmode = value; }
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
			set { scncockpittype = value; }
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
			set { scnhudtype = value; }
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
		/// Is the KU-band Antenna is installed
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
		/// Is the ODS is installed
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
		/// Is the External A/L is installed
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
		/// Is the SILTS pod is installed
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


		public event PropertyChangedEventHandler PropertyChanged;

		private void OnPropertyChanged( string prop )
		{
			if (PropertyChanged != null)
			{
				PropertyChanged( this, new PropertyChangedEventArgs( prop ) );
			}
		}

		public void LoadDefault()
		{
			ovname = "Atlantis";
			missionName = "STS-101";
			description = "The STS-101 launch scenario, as simulated by Space Shuttle Ultra.";
			rms = true;
			kubandantenna = true;
			ods = true;
			extal = true;
			fwdbulkdocklights = true;
			dragchute = true;
			silts = false;
			logssmedata = false;
			return;
		}
	}
}
