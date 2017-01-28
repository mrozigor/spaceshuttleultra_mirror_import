using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class SSUVessel : OrbiterVessel
	{
		public SSUVessel( Mission mission )
		{
			this.mission = mission;
		}

		public override void PreSave( MissionPhase missionphase )
		{
			_class = "SpaceShuttleUltra";
			name = mission.OVname;

			// TODO build state from "mission" and "missionphase"
			switch (missionphase)
			{
				case MissionPhase.LaunchT31s:
					config = 0;
					vcpos = 0;
					sbtc_l = 0.0;
					sbtc_r = 0.0;
					sb_st = 0;
					sb_cur = 0.0;
					sb_tgt = 0.0;
					gear_st = 0;
					gear_pos = 0.0;
					majormode = 101;
					cameraA_p = 0.0;
					cameraA_y = -90.0;
					cameraB_p = 0.0;
					cameraB_y = 90.0;
					cameraC_p = 0.0;
					cameraC_y = -90.0;
					cameraD_p = 0.0;
					cameraD_y = 90.0;
					// TODO state vector, etc
					statuslanded = true;
					statusplanet = "Earth";
					poslon = 0.0;
					poslat = 0.0;
					heading = 0.0;
					alt = -1.345;
					afcmode = 7;
					if (mission.MLP == 0) attached.Add( Tuple.Create( 0, 0, "MLP1" ) );
					else if (mission.MLP == 1) attached.Add( Tuple.Create( 0, 0, "MLP2" ) );
					else attached.Add( Tuple.Create( 0, 0, "MLP3" ) );
					break;
				case MissionPhase.Orbit:
					config = 2;
					vcpos = 0;
					sbtc_l = 0.0;
					sbtc_r = 0.0;
					sb_st = 0;
					sb_cur = 0.0;
					sb_tgt = 0.0;
					gear_st = 0;
					gear_pos = 0.0;
					majormode = 201;
					cameraA_p = 0.0;
					cameraA_y = -90.0;
					cameraB_p = 0.0;
					cameraB_y = 90.0;
					cameraC_p = 0.0;
					cameraC_y = -90.0;
					cameraD_p = 0.0;
					cameraD_y = 90.0;
					// TODO state vector, etc
					break;
			}
			return;
		}

		protected override void SaveSpecificParams( System.IO.StreamWriter scn )
		{
			scn.WriteLine( "  MISSION " + mission.MissionName );

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
			return;
		}


		Mission mission;

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
	}
}
