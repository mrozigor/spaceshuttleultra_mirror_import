using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class SSUVessel : OrbiterVessel
	{
		public SSUVessel( Mission mission, string orbiterpath )
		{
			osfspath = orbiterpath;

			plmass = -1.0;
			majormode = 101;

			this.mission = mission;
		}

		protected override void LoadSpecificParams( string line, System.IO.StreamReader scn )
		{
			do
			{
				line = line.TrimStart( ' ' );
				if (line.IndexOf( ';' ) != -1) line = line.Substring( 0, line.IndexOf( ';' ) );
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
					string tmp = line.Substring( 5, line.Length - 5 );
					double []num = Array.ConvertAll( tmp.Split( ' ' ), Double.Parse );
					if (num.Count() == 2)
					{
						sbtc_l = num[0];
						sbtc_r = num[1];
					}
				}
				else if (line.StartsWith( "SPEEDBRAKE " ))
				{
					string tmp = line.Substring( 11, line.Length - 11 );
					double []num = Array.ConvertAll( tmp.Split( ' ' ), Double.Parse );
					if (num.Count() == 3)
					{
						sb_st = (int)num[0];
						sb_cur = num[1];
						sb_tgt = num[2];
					}
				}
				else if (line.StartsWith( "GEAR " ))
				{
					string tmp = line.Substring( 5, line.Length - 5 );
					double []num = Array.ConvertAll( tmp.Split( ' ' ), Double.Parse );
					if (num.Count() == 2)
					{
						gear_st = (int)num[0];
						gear_pos = num[1];
					}
				}
				else if (line.StartsWith( "CARGO_STATIC_MESH " ))
				{
					staticcargo_mesh = line.Substring( 18, line.Length - 18 );
				}
				else if (line.StartsWith( "CARGO_STATIC_OFS " ))
				{
					string tmp = line.Substring( 17, line.Length - 17 );
					double []num = Array.ConvertAll( tmp.Split( ' ' ), Double.Parse );
					if (num.Count() == 3)
					{
						staticcargo_ofsx = num[0];
						staticcargo_ofsy = num[1];
						staticcargo_ofsz = num[2];
					}
				}
				else if (line.StartsWith( "PAYLOAD_MASS " ))
				{
					plmass = Convert.ToDouble( line.Substring( 13, line.Length - 13 ) );
				}
				else if (line.StartsWith( "OPS " ))
				{
					majormode = Convert.ToInt32( line.Substring( 4, line.Length - 4 ) );
				}
				else if (line.StartsWith( "PLB_CAM " ))
				{
					string tmp = line.Substring( 8, line.Length - 8 );
					double []num = Array.ConvertAll( tmp.Split( ' ' ), Double.Parse );
					if (num.Count() == 8)
					{
						cameraA_p = num[0];
						cameraA_y = num[1];
						cameraB_p = num[2];
						cameraB_y = num[3];
						cameraC_p = num[4];
						cameraC_y = num[5];
						cameraD_p = num[6];
						cameraD_y = num[7];
					}
				}
				else if (line.StartsWith( "@SUBSYSTEM " ))
				{
					// TODO
				}
				else if (line.StartsWith( "  @PANEL " ))
				{
					// TODO
				}
			} while ((line = scn.ReadLine()) != null);

			// TODO check if mission file exists (what to do if it doesn't?)
			LoadMissionFile( osfspath + "Missions\\SSU\\" + missionname + ".cfg" );
			return;
		}

		protected override void SaveSpecificParams( System.IO.StreamWriter scn )
		{
			scn.WriteLine( "  MISSION " + missionname );

			scn.WriteLine( "  CONFIGURATION " + config );

			scn.WriteLine( "  VC_POS " + vcpos );

			scn.WriteLine( "  SBTC " + string.Format( "{0:f2} {1:f2}", sbtc_l, sbtc_r ) );

			scn.WriteLine( "  SPEEDBRAKE " + sb_st + " " + string.Format( "{0:f4} {1:f4}", sb_cur, sb_tgt ) );

			scn.WriteLine( "  GEAR " + gear_st + " " + string.Format( "{0:f4}", gear_pos ) );

			if (!String.IsNullOrEmpty( staticcargo_mesh ))
			{
				scn.WriteLine( "  CARGO_STATIC_MESH " + staticcargo_mesh );
				if ((staticcargo_ofsx != 0) && (staticcargo_ofsy != 0) && (staticcargo_ofsz != 0)) scn.WriteLine( "  CARGO_STATIC_OFS " + staticcargo_ofsx + " " + staticcargo_ofsy + " " + staticcargo_ofsz );
			}
			if (plmass > 0) scn.WriteLine( "  PAYLOAD_MASS " + plmass );

			scn.WriteLine( "  OPS " + majormode );

			scn.WriteLine( "  PLB_CAM " + string.Format( "{0:f4} {1:f4} {2:f4} {3:f4} {4:f4} {5:f4} {6:f4} {7:f4}", cameraA_p, cameraA_y, cameraB_p, cameraB_y, cameraC_p, cameraC_y, cameraD_p , cameraD_y) );

			// TODO subsystems
			// TODO panels

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
							mission.MaxThrust = Convert.ToDouble( val );
							break;
						case "ThrottleDown":
							mission.QPOLY3 = Convert.ToDouble( val );
							break;
						case "ThrottleUp":
							mission.QPOLY4 = Convert.ToDouble( val );
							break;
						case "UseRMS":
							mission.RMS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseKUBand":
							mission.KUbandAntenna = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseSTBDMPM":
							mission.STBDMPMs = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseODS":
							mission.ODS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseExtAL":
							mission.ExtAL = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseTAA":
							mission.TTA = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "AftTAA":
							mission.AftTTA = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "HasBulkheadFloodlights":
							mission.FwdBulkDockLights = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "HasDragChute":
							mission.DragChute = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "Bridgerails":
							// TODO
							break;
						case "PayloadZPos":
							// TODO
							break;
						case "SILTS":
							mission.SILTS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "LogSSMEData":
							mission.LogSSMEData = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseCISS":
							mission.CISS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "CISS_GPrime":
							mission.CISSGPrime = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "UseASE_IUS":
							mission.ASE_IUS = (val.ToUpperInvariant() == "TRUE" ? true : false);
							break;
						case "ASE_IUS_AftLocation":
							mission.ASE_IUSAft = (val.ToUpperInvariant() == "TRUE" ? true : false);
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
			file.WriteLine( "OMSAssistDuration=" + string.Format( "{0:f6}", mission.OMSAssistDuration ) );
			file.WriteLine( "MaxSSMEThrust=" + mission.MaxThrust );
			file.WriteLine( "ThrottleDown=" + string.Format( "{0:f3}", mission.QPOLY3 ) );
			file.WriteLine( "ThrottleUp=" + string.Format( "{0:f3}", mission.QPOLY4 ) );
			file.WriteLine( "UseRMS=" + (mission.RMS ? "TRUE" : "FALSE") );
			file.WriteLine( "UseKUBand=" + (mission.KUbandAntenna ? "TRUE" : "FALSE") );
			file.WriteLine( "UseSTBDMPM=" + (mission.STBDMPMs ? "TRUE" : "FALSE") );
			file.WriteLine( "UseODS=" + (mission.ODS ? "TRUE" : "FALSE") );
			file.WriteLine( "UseExtAL=" + (mission.ExtAL ? "TRUE" : "FALSE") );
			file.WriteLine( "UseTAA=" + (mission.TTA ? "TRUE" : "FALSE") );
			file.WriteLine( "AftTAA=" + (mission.AftTTA ? "TRUE" : "FALSE") );
			file.WriteLine( "HasBulkheadFloodlights=" + (mission.FwdBulkDockLights ? "TRUE" : "FALSE") );
			file.WriteLine( "HasDragChute=" + (mission.DragChute ? "TRUE" : "FALSE") );
			file.WriteLine( "Bridgerails=" );// TODO
			file.WriteLine( "PayloadZPos=" );// TODO
			file.WriteLine( "SILTS=" + (mission.SILTS ? "TRUE" : "FALSE") );
			file.WriteLine( "LogSSMEData=" + (mission.LogSSMEData ? "TRUE" : "FALSE") );
			file.WriteLine( "UseCISS=" + (mission.CISS ? "TRUE" : "FALSE") );
			file.WriteLine( "CISS_GPrime=" + (mission.CISSGPrime ? "TRUE" : "FALSE") );
			file.WriteLine( "UseASE_IUS=" + (mission.ASE_IUS ? "TRUE" : "FALSE") );
			file.WriteLine( "ASE_IUS_AftLocation=" + (mission.ASE_IUSAft ? "TRUE" : "FALSE") );
			
			file.Close();
			return;
		}

		protected string missionname;

		protected int config;

		protected int vcpos;

		protected double sbtc_l;
		protected double sbtc_r;

		protected int sb_st;
		protected double sb_cur;
		protected double sb_tgt;

		protected int gear_st;
		protected double gear_pos;

		protected string staticcargo_mesh;

		protected double staticcargo_ofsx;
		protected double staticcargo_ofsy;
		protected double staticcargo_ofsz;
		
		protected double plmass;

		protected int majormode;

		protected double cameraA_p;
		protected double cameraA_y;
		protected double cameraB_p;
		protected double cameraB_y;
		protected double cameraC_p;
		protected double cameraC_y;
		protected double cameraD_p;
		protected double cameraD_y;

		protected string osfspath;
		
		protected Mission mission;
	}
}
