using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class SSUVessel : OrbiterVessel
	{
		public SSUVessel( string orbiterpath )
		{
			osfspath = orbiterpath;

			majormode = 101;

			mission = new Mission();
		}

		protected override void LoadSpecificParams( string line, System.IO.StreamReader scn )
		{
			do
			{
				line = line.TrimStart( ' ' );
				if (line == "END") break;
				else if (line.StartsWith( "MISSION " ))
				{
					missionname = line.Substring( 8, line.Length - 8 );
				}
				else if (line.StartsWith( "CONFIGURATION " ))
				{
					config = Convert.ToInt32( line.Substring( 14, line.Length - 14 ) );
				}
				else if (line.StartsWith( "VC_POS " ))
				{
					vcpos = Convert.ToInt32( line.Substring( 7, line.Length - 7 ) );
				}
				else if (line.StartsWith( "SBTC " ))
				{
					//
				}
				else if (line.StartsWith( "SPEEDBRAKE " ))
				{
					//
				}
				else if (line.StartsWith( "GEAR " ))
				{
					//
				}
				else if (line.StartsWith( "OPS " ))
				{
					majormode = Convert.ToInt32( line.Substring( 4, line.Length - 4 ) );
				}
				else if (line.StartsWith( "PLB_CAM " ))
				{
					//
				}
				else if (line.StartsWith( "@SUBSYSTEM " ))
				{
					//
				}
				else if (line.StartsWith( "  @PANEL " ))
				{
					//
				}
			} while ((line = scn.ReadLine()) != null);

			// TODO check if mission file exists
			LoadMissionFile( osfspath + "Missions\\SSU\\" + missionname + ".cfg" );
			return;
		}

		protected override void SaveSpecificParams( System.IO.StreamWriter scn )
		{
			scn.WriteLine( "  MISSION " + missionname );

			scn.WriteLine( "  CONFIGURATION " + config );

			scn.WriteLine( "  VC_POS " + vcpos );

			scn.WriteLine( "  OPS " + majormode );

			// TODO

			SaveMissionFile( osfspath + "Missions\\SSU\\" + missionname + ".cfg" );
			return;
		}

		private void LoadMissionFile( string missionfile )
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
							mission.OVtex = val;
							break;
						case "TargetInc":
							mission.MECO_Inc = Convert.ToDouble( val );
							break;
						case "TargetLAN":
							mission.MECO_LAN = Convert.ToDouble( val );
							break;
						case "MECOAlt":
							mission.MECO_Alt = Convert.ToDouble( val );
							break;
						case "MECOVel":
							mission.MECO_Vel = Convert.ToDouble( val );
							break;
						case "MECOFPA":
							mission.MECO_FPA = Convert.ToDouble( val );
							break;
						case "PerformRollToHeadsUp":
							mission.RTHU = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "OMSAssistEnable":
							mission.OMSAssistEnable = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "OMSAssistDuration":
							mission.OMSAssistDuration = Convert.ToDouble( val );
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

		private void SaveMissionFile( string missionfile )
		{
			System.IO.StreamWriter file = new System.IO.StreamWriter( missionfile );
			
			file.WriteLine( "Name=" + mission.MissionName );
			file.WriteLine( "Orbiter=" + mission.OVname );
			file.WriteLine( "OrbiterTexture=" + mission.OVtex );
			file.WriteLine( "TargetInc=" + string.Format( "{0:f6}", mission.MECO_Inc ) );
			file.WriteLine( "TargetLAN=" + string.Format( "{0:f6}", mission.MECO_LAN ) );
			file.WriteLine( "MECOAlt=" + string.Format( "{0:f6}", mission.MECO_Alt ) );
			file.WriteLine( "MECOVel=" + string.Format( "{0:f6}", mission.MECO_Vel ) );
			file.WriteLine( "MECOFPA=" + string.Format( "{0:f6}", mission.MECO_FPA ) );
			file.WriteLine( "PerformRollToHeadsUp=" + (mission.RTHU ? "TRUE" : "FALSE") );
			file.WriteLine( "OMSAssistEnable=" + (mission.OMSAssistEnable ? "TRUE" : "FALSE") );
			file.WriteLine( "OMSAssistDuration=" +  string.Format( "{0:f6}", mission.OMSAssistDuration ) );
			file.WriteLine( "MaxSSMEThrust=" );
			file.WriteLine( "ThrottleDown=" );
			file.WriteLine( "ThrottleUp=" );
			file.WriteLine( "UseRMS=" + (mission.RMS ? "TRUE" : "FALSE") );
			file.WriteLine( "UseKUBand=" + (mission.KUbandAntenna ? "TRUE" : "FALSE") );
			file.WriteLine( "UseSTBDMPM=" );
			file.WriteLine( "UseODS=" + (mission.ODS ? "TRUE" : "FALSE") );
			file.WriteLine( "UseExtAL=" + (mission.ExtAL ? "TRUE" : "FALSE") );
			file.WriteLine( "UseTAA=" );
			file.WriteLine( "AftTAA=" );
			file.WriteLine( "HasBulkheadFloodlights=" + (mission.FwdBulkDockLights ? "TRUE" : "FALSE") );
			file.WriteLine( "HasDragChute=" + (mission.DragChute ? "TRUE" : "FALSE") );
			file.WriteLine( "Bridgerails=" );
			file.WriteLine( "PayloadZPos=" );
			if (mission.OVname == "Columbia") file.WriteLine( "SILTS=" + (mission.SILTS ? "TRUE" : "FALSE") );
			file.WriteLine( "LogSSMEData=" + (mission.LogSSMEData ? "TRUE" : "FALSE") );
			file.WriteLine( "UseCISS=" );
			file.WriteLine( "CISS_GPrime=" );
			file.WriteLine( "UseASE_IUS=" );
			file.WriteLine( "ASE_IUS_AftLocation=" );
			
			file.Close();
			return;
		}

		protected string missionname;

		protected int config;

		protected int vcpos;

		protected int majormode;

		protected string osfspath;
		
		protected Mission mission;
	}
}
