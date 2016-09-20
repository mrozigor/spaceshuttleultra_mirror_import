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
		/// The date/time at the start of the simulation
		/// </summary>
		private DateTime scndatetime;

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
		/// The date/time at the start of the simulation
		/// </summary>
		public DateTime scnDatetime
		{
			get { return scndatetime; }
			set { scndatetime = value; }
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
