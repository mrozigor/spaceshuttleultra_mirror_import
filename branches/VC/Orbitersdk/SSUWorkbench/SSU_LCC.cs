using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class SSU_LCC : OrbiterVessel
	{
		public SSU_LCC( Mission mission )
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

			_class = "SSU_LCC";
			name = "LCC";

			int ms = Convert.ToInt32( 1000 * (mission.T0Second - (int)mission.T0Second) );
			t0 = new DateTime( mission.T0Year, mission.T0Month, mission.T0Day, mission.T0Hour, mission.T0Minute, (int)mission.T0Second, ms );

			if (mission.LaunchPad == 0) pad_name = "LC39A";
			else pad_name = "LC39B";

			shuttle_name = mission.OVname;

			statuslanded = true;
			statusplanet = "Earth";
			poslon = -80.6493245;
			poslat = 28.5852249;
			heading = 270.0;
			alt = 2;
			afcmode = 7;
			return;
		}

		protected override void SaveSpecificParams( System.IO.StreamWriter scn )
		{
			scn.WriteLine( "  LAUNCH_MJD " + string.Format( "{0:f10}", t0.ToOADate() + 15018.0 ) );

			scn.WriteLine( "  PAD_NAME " + pad_name );

			scn.WriteLine( "  SHUTTLE_NAME " + shuttle_name );
			return;
		}


		Mission mission;

		DateTime t0;

		protected string shuttle_name;
		protected string pad_name;
	}
}
