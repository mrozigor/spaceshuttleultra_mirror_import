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
			// TODO missing scenario parameters:
			// Date JD
			// Date JE
			// Help
			// Script
			// TRACKMODE (partially done)
			// GROUNDLOCATION
			// GROUNDDIRECTION
			// REF (HUD)

			Mission mission = new Mission();
			
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
							if (mission.Description == null) mission.Description = line;
							else mission.Description += "\r\n" + line;
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
							mission.scnSystem = line.Substring( 7, line.Length - 7 );
						}
						else if (line.StartsWith( "Date MJD " ))
						{
							double num = Convert.ToDouble( line.Substring( 9, line.Length - 9 ) ) - 15018.0;
							mission.scnDatetime = DateTime.FromOADate( num );
						}
						else if (line.StartsWith( "Context " ))
						{
							mission.scnContext = line.Substring( 8, line.Length - 8 );
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
							mission.scnShip = line.Substring( 5, line.Length - 5 );
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
							mission.scnCameraTarget = line.Substring( 7, line.Length - 7 );
						}
						else if (line.StartsWith( "MODE " ))
						{
							if (line.Substring( 5, line.Length - 5 ) == "Extern") mission.scnCameraMode = 1;
						}
						else if (line.StartsWith( "POS " ))
						{
							string tmp = line.Substring( 4, line.Length - 4 );
							double []num = Array.ConvertAll( tmp.Split( ' ' ), Double.Parse );
							if (num.Count() == 3)
							{
								mission.scnCameraPosX = num[0];
								mission.scnCameraPosY = num[1];
								mission.scnCameraPosZ = num[2];
							}
						}
						else if (line.StartsWith( "TRACKMODE " ))
						{
							mission.scnCameraTrackMode = line.Substring( 10, line.Length - 10 );
						}
						else if (line.StartsWith( "FOV " ))
						{
							mission.scnCameraFOV = Convert.ToDouble( line.Substring( 4, line.Length - 4 ) );
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
							if (tmp == "Orbit") mission.scnHUDType = 1;
							else if (tmp == "Surface") mission.scnHUDType = 2;
							else mission.scnHUDType = 3;
						}
					}
				}
				else if (line.StartsWith( "BEGIN_MFD " ))
				{
					string mfdid = line.Substring( 10, line.Length - 10 );
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_MFD") break;
						else
						{
							// TODO
						}
					}
				}
				else if (line.StartsWith( "BEGIN_PANEL" ))
				{
					mission.scnCockpitType = 1;
					while ((line = file.ReadLine()) != null)
					{
						line = line.TrimStart( ' ' );
						if (line == "END_PANEL") break;
					}
				}
				else if (line.StartsWith( "BEGIN_VC" ))
				{
					mission.scnCockpitType = 2;
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
						else
						{
							// TODO
						}
					}
				}

				// TODO get mission file name into mission.MissionName
				mission.MissionName = "STS-101";
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
			file.WriteLine( mission.Description );
			file.WriteLine( "END_DESC" );
			file.WriteLine( "" );

			////////////////// environment //////////////////
			file.WriteLine( "BEGIN_ENVIRONMENT" );
			file.WriteLine( "  System " + mission.scnSystem );
			file.WriteLine( "  Date MJD " + string.Format( "{0:f6}", mission.scnDatetime.ToOADate() + 15018.0 ) );
			if (mission.scnContext != null) file.WriteLine( "  Context " + mission.scnContext );
			file.WriteLine( "END_ENVIRONMENT" );
			file.WriteLine( "" );

			////////////////// focus //////////////////
			file.WriteLine( "BEGIN_FOCUS" );
			file.WriteLine( "  Ship " + mission.scnShip );
			file.WriteLine( "END_FOCUS\r\n" );

			////////////////// camera //////////////////
			file.WriteLine( "BEGIN_CAMERA" );
			if (mission.scnCameraMode == 1) file.WriteLine( "  TARGET " + mission.scnCameraTarget );
			if (mission.scnCameraMode == 0) file.WriteLine( "  MODE Cockpit" );
			else file.WriteLine( "  MODE Extern" );
			if (mission.scnCameraMode == 1) file.WriteLine( "  POS " + string.Format( "{0:f2} {1:f2} {2:f2}", mission.scnCameraPosX, mission.scnCameraPosY, mission.scnCameraPosZ ) );
			file.WriteLine( "  TRACKMODE " + mission.scnCameraTrackMode );
			file.WriteLine( "  FOV " + string.Format( "{0:f2}", mission.scnCameraFOV ) );
			file.WriteLine( "END_CAMERA" );
			file.WriteLine( "" );

			////////////////// hud //////////////////
			if (mission.scnHUDType != 0)
			{
				file.WriteLine( "BEGIN_HUD" );
				if (mission.scnHUDType == 1) file.WriteLine( "  TYPE Orbit" );
				else if (mission.scnHUDType == 2) file.WriteLine( "  TYPE Surface" );
				else file.WriteLine( "  TYPE Docking" );
				file.WriteLine( "END_HUD" );
				file.WriteLine( "" );
			}

			////////////////// mfd //////////////////
			string[] MFDname = {"Left", "Right", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
			for (int i = 0; i < 12; i++)
			{
				// TODO add condition to output only if mfd is on
				file.WriteLine( "BEGIN_MFD " + MFDname[i] );
				// TODO mfd
				file.WriteLine( "END_MFD" );
				file.WriteLine( "" );
			}

			////////////////// panel //////////////////
			if (mission.scnCockpitType == 1)
			{
				file.WriteLine( "BEGIN_PANEL" );
				file.WriteLine( "END_PANEL" );
				file.WriteLine( "" );
			}

			////////////////// vc //////////////////
			if (mission.scnCockpitType == 2)
			{
				file.WriteLine( "BEGIN_VC" );
				file.WriteLine( "END_VC" );
				file.WriteLine( "" );
			}

			////////////////// ships //////////////////
			file.WriteLine( "BEGIN_SHIPS" );
			// TODO ships
			file.WriteLine( "END_SHIPS" );
			//file.WriteLine( "" );

			////////////////// extmfd //////////////////
			/*file.WriteLine( "BEGIN_DX9ExtMFD" );
			file.WriteLine( "END" );
			//file.WriteLine( "" );

			////////////////// extmfd //////////////////
			file.WriteLine( "BEGIN_ExtMFD" );
			file.WriteLine( "END" );
			//file.WriteLine( "" );*/

			file.Close();

			//SaveMissionFile( orbiterpath + "Missions\\SSU\\" + mission.MissionName + ".cfg", mission );
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
