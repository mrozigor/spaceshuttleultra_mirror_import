using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace SSUWorkbench.model
{
	class SSUW_File_IO
	{
		public SSUW_File_IO()
		{
		}

		public Mission Load( string scnfile, string orbiterpath )
		{
			Mission mission = new Mission();
			
			string line;

			System.IO.StreamReader file = new System.IO.StreamReader( scnfile );
			while ((line = file.ReadLine()) != null)
			{
				// TODO parse scenario file

				// TODO get mission file name into mission.MissionName
			}
			file.Close();
			
			LoadMissionFile( orbiterpath + "Missions\\SSU\\" + mission.MissionName + ".cfg", mission );
			return mission;
		}

		public void Save( string scnfile, string orbiterpath, Mission mission )
		{
			System.IO.StreamWriter file = new System.IO.StreamWriter( scnfile );
			// TODO write scn file
			////////////////// description //////////////////
			file.WriteLine( "BEGIN_DESC" );
			file.WriteLine( "  " + mission.Description );
			file.WriteLine( "END_DESC\r\n" );

			////////////////// environment //////////////////
			file.WriteLine( "BEGIN_ENVIRONMENT" );
			file.WriteLine( "  System Sol" );
			file.WriteLine( "  Date MJD " + mission.scnDatetime.ToOADate() + 2415018.5 );
			file.WriteLine( "  Context SSU" );
			file.WriteLine( "END_ENVIRONMENT\r\n" );

			////////////////// focus //////////////////
			file.WriteLine( "BEGIN_FOCUS" );
			file.WriteLine( "  Ship " + "TODO" );// TODO focus
			file.WriteLine( "END_FOCUS\r\n" );

			////////////////// camera //////////////////
			file.WriteLine( "BEGIN_CAMERA" );
			file.WriteLine( "  TARGET " + "TODO" );// TODO target
			file.WriteLine( "  MODE " + "TODO" );// TODO mode
			file.WriteLine( "  POS " + "TODO" );// TODO pos
			file.WriteLine( "  TRACKMODE " + "TODO" );// TODO track mode
			file.WriteLine( "  FOV " + "40" );// TODO fov
			file.WriteLine( "END_CAMERA\r\n" );

			////////////////// mfd //////////////////
			string[] MFDname = {"Left", "Right", "3", "4", "5", "6", "7", "8", "9", "10", "11"};
			for (int i = 0; i < 11; i++)
			{
				// TODO add condition to output only if mfd is on
				file.WriteLine( "BEGIN_MFD " + MFDname[i] );
				// TODO mfd
				file.WriteLine( "END_MFD\r\n" );
			}

			////////////////// vc //////////////////
			file.WriteLine( "BEGIN_VC" );
			file.WriteLine( "END_VC\r\n" );

			////////////////// ships //////////////////
			file.WriteLine( "BEGIN_SHIPS" );
			// TODO ships
			file.WriteLine( "END_SHIPS\r\n" );

			////////////////// extmfd //////////////////
			file.WriteLine( "BEGIN_ExtMFD" );
			// TODO extmfd
			file.WriteLine( "END" );
			
			file.Close();

			SaveMissionFile( orbiterpath + "Missions\\SSU\\" + mission.MissionName + ".cfg", mission );
			return;
		}

		private void LoadMissionFile( string missionfile, Mission mission )
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
							mission.MissionName = val;
							break;
						case "Orbiter":
							mission.OVname = val;
							break;
						case "OrbiterTexture":
							break;
						case "TargetInc":
							break;
						case "MECOAlt":
							break;
						case "MECOVel":
							break;
						case "MECOFPA":
							break;
						case "PerformRollToHeadsUp":
							break;
						case "OMSAssistEnable":
							break;
						case "OMSAssistDuration":
							break;
						case "MaxSSMEThrust":
							break;
						case "ThrottleDown":
							break;
						case "ThrottleUp":
							break;
						case "UseRMS":
							mission.RMS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseKUBand":
							mission.KUbandAntenna = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseSTBDMPM":
							break;
						case "UseODS":
							mission.ODS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseExtAL":
							mission.ExtAL = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseTAA":
							break;
						case "AftTAA":
							break;
						case "HasBulkheadFloodlights":
							mission.FwdBulkDockLights = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "HasDragChute":
							mission.DragChute = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "Bridgerails":
							break;
						case "PayloadZPos":
							break;
						case "SILTS":
							mission.SILTS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "LogSSMEData":
							mission.LogSSMEData = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseCISS":
							break;
						case "CISS_GPrime":
							break;
						case "UseASE_IUS":
							break;
						case "ASE_IUS_AftLocation":
							break;

					}
				}
			}
			file.Close();

			return;
		}

		private void SaveMissionFile( string missionfile, Mission mission )
		{
			System.IO.StreamWriter file = new System.IO.StreamWriter( missionfile );
			
			file.WriteLine( "Name=" + mission.MissionName );
			file.WriteLine( "Orbiter=" + mission.OVname );
			file.WriteLine( "OrbiterTexture=" );
			file.WriteLine( "TargetInc=" );
			file.WriteLine( "MECOAlt=" );
			file.WriteLine( "MECOVel=" );
			file.WriteLine( "MECOFPA=" );
			file.WriteLine( "PerformRollToHeadsUp=" );
			file.WriteLine( "OMSAssistEnable=" );
			file.WriteLine( "OMSAssistDuration=" );
			file.WriteLine( "MaxSSMEThrust=" );
			file.WriteLine( "ThrottleDown=" );
			file.WriteLine( "ThrottleUp=" );
			file.WriteLine( "UseRMS=" );
			file.WriteLine( "UseKUBand=" );
			file.WriteLine( "UseSTBDMPM=" );
			file.WriteLine( "UseODS=" );
			file.WriteLine( "UseExtAL=" );
			file.WriteLine( "UseTAA=" );
			file.WriteLine( "AftTAA=" );
			file.WriteLine( "HasBulkheadFloodlights=" + (mission.FwdBulkDockLights ? "TRUE" : "FALSE") );
			file.WriteLine( "HasDragChute=" + (mission.DragChute ? "TRUE" : "FALSE") );
			file.WriteLine( "Bridgerails=" );
			file.WriteLine( "PayloadZPos=" );
			file.WriteLine( "SILTS=" + (mission.SILTS ? "TRUE" : "FALSE") );
			file.WriteLine( "LogSSMEData=" + (mission.LogSSMEData ? "TRUE" : "FALSE") );
			file.WriteLine( "UseCISS=" );
			file.WriteLine( "CISS_GPrime=" );
			file.WriteLine( "UseASE_IUS=" );
			file.WriteLine( "ASE_IUS_AftLocation=" );
			
			file.Close();
			return;
		}
	}
}
