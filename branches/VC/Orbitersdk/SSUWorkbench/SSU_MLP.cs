using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class SSU_MLP : OrbiterVessel
	{
		public SSU_MLP( Mission mission )
		{
			this.mission = mission;
		}

		public override void PreSave( MissionPhase missionphase )
		{
			if ((mission.LaunchSite == 1) || (missionphase > MissionPhase.LaunchT31s))
			{
				save = false;
				return;
			}

			_class = "SSU_MLP";
			if (mission.MLP == 0) name = "MLP1";
			else if (mission.MLP == 1) name = "MLP2";
			else name = "MLP3";

			umb_st = 0;
			umb_pos = 0.0;

			statuslanded = true;
			statusplanet = "Earth";
			poslon = 0.0;
			poslat = 0.0;
			heading = 0.0;
			alt = 30.685;
			afcmode = 7;
			if (mission.LaunchPad == 0) attached.Add( Tuple.Create( 0, 0, "LC39A" ) );
			else attached.Add( Tuple.Create( 0, 0, "LC39B" ) );
			return;
		}

		protected override void SaveSpecificParams( System.IO.StreamWriter scn )
		{
			scn.WriteLine( "  T0_UMB " + umb_st + " " + string.Format( "{0:f4}", umb_pos ) );
			return;
		}


		Mission mission;

		protected int umb_st;
		protected double umb_pos;
	}
}
