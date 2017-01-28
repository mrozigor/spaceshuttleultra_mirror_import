using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class SSU_Pad : OrbiterVessel
	{
		public SSU_Pad( Mission mission )
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

			_class = "SSU_Pad";
			if (mission.LaunchPad == 0) name = "LC39A";
			else name = "LC39B";

			shuttle = mission.OVname;

			statuslanded = true;
			statusplanet = "Earth";
			if (mission.LaunchPad == 0)
			{
				poslon = -80.6040720;
				poslat = 28.6083850;
			}
			else
			{
				poslon = -80.6208620;
				poslat = 28.6272150;
			}
			heading = 270.0;
			alt = 2;
			afcmode = 7;
			
			// TODO build state from "mission" and "missionphase"
			switch (missionphase)
			{
				case MissionPhase.LaunchT9m:
					oaa_st = 1;
					oaa_pos = 1.0;
					gva_st = 0;
					gva_pos = 0.0;
					venthood_st = 0;
					venthood_pos = 0.0;
					owpfss_st = 0;
					owpfss_pos = 0.0;
					owprss_st = 0;
					owprss_pos = 0.0;
					rss_st = 1;
					rss_pos = 1.0;
					gh2_st = 0;
					gh2_pos = 0.0;
					iaa_st = 0;
					iaa_pos = 0.0;
					eastsfd_st = 1;
					eastsfd_pos = 1.0;
					westsfd_st = 1;
					westsfd_pos = 1.0;
					break;
				case MissionPhase.LaunchT31s:
					oaa_st = 0;
					oaa_pos = 0.0;
					gva_st = 0;
					gva_pos = 0.0;
					venthood_st = 0;
					venthood_pos = 0.0;
					owpfss_st = 0;
					owpfss_pos = 0.0;
					owprss_st = 0;
					owprss_pos = 0.0;
					rss_st = 1;
					rss_pos = 1.0;
					gh2_st = 0;
					gh2_pos = 0.0;
					iaa_st = 0;
					iaa_pos = 0.0;
					eastsfd_st = 1;
					eastsfd_pos = 1.0;
					westsfd_st = 1;
					westsfd_pos = 1.0;
					break;
			}
			return;
		}

		protected override void SaveSpecificParams( System.IO.StreamWriter scn )
		{
			scn.WriteLine( "  ACCESS_ARM " + oaa_st + " " + string.Format( "{0:f4}", oaa_pos ) );

			scn.WriteLine( "  GVA " + gva_st + " " + string.Format( "{0:f4}", gva_pos ) );

			scn.WriteLine( "  VENTHOOD " + venthood_st + " " + string.Format( "{0:f4}", venthood_pos ) );

			scn.WriteLine( "  FSS_OWP " + owpfss_st + " " + string.Format( "{0:f4}", owpfss_pos ) );

			scn.WriteLine( "  RSS_OWP " + owprss_st + " " + string.Format( "{0:f4}", owprss_pos ) );

			scn.WriteLine( "  RSS " + rss_st + " " + string.Format( "{0:f4}", rss_pos ) );

			scn.WriteLine( "  FSS_GH2 " + gh2_st + " " + string.Format( "{0:f4}", gh2_pos ) );

			scn.WriteLine( "  FSS_IAA " + iaa_st + " " + string.Format( "{0:f4}", iaa_pos ) );

			scn.WriteLine( "  EAST_SRB_SFD " + eastsfd_st + " " + string.Format( "{0:f4}", eastsfd_pos ) );

			scn.WriteLine( "  WEST_SRB_SFD " + westsfd_st + " " + string.Format( "{0:f4}", westsfd_pos ) );

			scn.WriteLine( "  SHUTTLE " + shuttle );
			return;
		}


		Mission mission;

		protected int oaa_st;
		protected double oaa_pos;

		protected int gva_st;
		protected double gva_pos;

		protected int venthood_st;
		protected double venthood_pos;

		protected int owpfss_st;
		protected double owpfss_pos;

		protected int owprss_st;
		protected double owprss_pos;

		protected int rss_st;
		protected double rss_pos;

		protected int gh2_st;
		protected double gh2_pos;

		protected int iaa_st;
		protected double iaa_pos;

		protected int eastsfd_st;
		protected double eastsfd_pos;

		protected int westsfd_st;
		protected double westsfd_pos;

		protected string shuttle;
	}
}
