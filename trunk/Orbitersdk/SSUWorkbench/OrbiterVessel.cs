using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
	class OrbiterVessel
	{
		public OrbiterVessel()
		{
			poslat = -1.0;
			poslon = -1.0;
			heading = -1.0;
			rposx = -1.0;
			rposy = -1.0;
			rposz = -1.0;
			rvelx = -1.0;
			rvely = -1.0;
			rvelz = -1.0;
			elements_date = -1.0;
			arotx = -1.0;
			aroty = -1.0;
			arotz = -1.0;
			vrotx = -1.0;
			vroty = -1.0;
			vrotz = -1.0;
			fuel = -1.0;
			alt = -9999.0;
			rcsmode = -1;
			afcmode = -1;
			xpdr = -1;
			prplevel = new List<Tuple<int,double>>();
			thlevel = new List<Tuple<int,double>>();
			dockinfo = new List<Tuple<int,int,string>>();
			attached = new List<Tuple<int,int,string>>();
			otherparams = new List<string>();

			save = true;
		}

		public void Save( System.IO.StreamWriter scn )
		{
			if (save == false) return;
			// header
			scn.WriteLine( name + ":" + _class );

			// standard params
			scn.WriteLine( "  STATUS " + (statuslanded ? "Landed" : "Orbiting") +  " " + statusplanet );
			
			if (statuslanded)
			{
				if (!String.IsNullOrEmpty( _base )) scn.WriteLine( "  BASE " + _base );
				else if ((poslon != -1.0) && (poslat != -1.0)) scn.WriteLine( "  POS " + string.Format( "{0:f7} {1:f7}", poslon, poslat ) );
				if (heading != -1.0) scn.WriteLine( "  HEADING " + string.Format( "{0:f2}", heading ) );
				if (alt != -9999.0) scn.WriteLine( "  ALT " + string.Format( "{0:f3}", alt ) );
				if ((arotx != -1.0) && (aroty != -1.0) && (arotz != -1.0)) scn.WriteLine( "  AROT " + string.Format( "{0:f3} {1:f3} {2:f3}", arotx, aroty, arotz ) );
			}
			else
			{
				if ((rposx != -1.0) && (rposy != -1.0) && (rposz != -1.0)) scn.WriteLine( "  RPOS " + string.Format( "{0:f3} {1:f3} {2:f3}", rposx, rposy, rposz ) );
				if ((rvelx != -1.0) && (rvely != -1.0) && (rvelz != -1.0)) scn.WriteLine( "  RVEL " + string.Format( "{0:f4} {1:f4} {2:f4}", rvelx, rvely, rvelz ) );
				if (elements_date != -1.0) scn.WriteLine( "  ELEMENTS " + elements_a + " " + elements_e + " " + elements_i + " " + elements_lan + " " + elements_lp + " " + elements_ml + " " + elements_date  );
				if ((arotx != -1.0) && (aroty != -1.0) && (arotz != -1.0)) scn.WriteLine( "  AROT " + string.Format( "{0:f3} {1:f3} {2:f3}", arotx, aroty, arotz ) );
				if ((vrotx != -1.0) && (vroty != -1.0) && (vrotz != -1.0)) scn.WriteLine( "  VROT " + vrotx + " " + vroty + " " + vrotz );
			}

			string attch = "";
			for (int j = 0; j < attached.Count; j++)
			{
				attch = attch + " " + attached[j].Item1 + ":" + attached[j].Item2 + "," + attached[j].Item3;
			}
			if (attached.Count > 0) scn.WriteLine( "  ATTACHED" + attch );

			if (rcsmode != -1) scn.WriteLine( "  RCSMODE " + rcsmode );
			
			if (afcmode != -1) scn.WriteLine( "  AFCMODE " + afcmode );

			if (fuel != -1.0) scn.WriteLine( "  FUEL " + fuel );

			string prplvl = "";
			for (int j = 0; j < prplevel.Count; j++)
			{
				prplvl = prplvl + " " + prplevel[j].Item1 + ":" + string.Format( "{0:f6}", prplevel[j].Item2 );
			}
			if (prplevel.Count > 0) scn.WriteLine( "  PRPLEVEL" + prplvl );

			string thlvl = "";
			for (int j = 0; j < thlevel.Count; j++)
			{
				thlvl = thlvl + " " + thlevel[j].Item1 + ":" + string.Format( "{0:f6}", thlevel[j].Item2 );
			}
			if (thlevel.Count > 0) scn.WriteLine( "  THLEVEL" + thlvl );

			string dcknf = "";
			for (int j = 0; j < dockinfo.Count; j++)
			{
				dcknf = dcknf + " " + dockinfo[j].Item1 + ":" + dockinfo[j].Item2 + "," + dockinfo[j].Item3;
			}
			if (dockinfo.Count > 0) scn.WriteLine( "  DOCKINFO" + dcknf );
			
			if (!String.IsNullOrEmpty( ids )) scn.WriteLine( "  IDS " + ids );
			
			if (!String.IsNullOrEmpty( navfreq )) scn.WriteLine( "  NAVFREQ " + navfreq );

			if (xpdr != -1) scn.WriteLine( "  XPDR " + xpdr );

			// other params
			SaveSpecificParams( scn );
			scn.WriteLine( "END" );
			return;
		}

		public virtual void PreSave( MissionPhase missionphase )
		{
			return;
		}

		protected virtual void SaveSpecificParams( System.IO.StreamWriter scn )
		{
			for (int i = 0; i < otherparams.Count; i++)
			{
				scn.WriteLine( "  " + otherparams[i] );
			}
			return;
		}

		protected string name;
		public string Name
		{
			get { return name; }
			set { name = value; }
		}

		protected string _class;
		public string Class
		{
			get { return _class; }
			set { _class = value; }
		}

		protected bool statuslanded;
		protected string statusplanet;

		protected string _base;

		protected double heading;

		protected double rposx;
		protected double rposy;
		protected double rposz;

		protected double rvelx;
		protected double rvely;
		protected double rvelz;

		protected double elements_a;
		protected double elements_e;
		protected double elements_i;
		protected double elements_lan;
		protected double elements_lp;
		protected double elements_ml;
		protected double elements_date;

		protected double arotx;
		protected double aroty;
		protected double arotz;

		protected double vrotx;
		protected double vroty;
		protected double vrotz;

		protected double fuel;

		protected double poslat;

		protected double poslon;

		protected double alt;

		protected int rcsmode;

		protected int afcmode;

		protected string ids;// HACK

		protected string navfreq;// HACK

		protected int xpdr;

		protected List<Tuple<int,double>> prplevel;
		
		protected List<Tuple<int,double>> thlevel;

		protected List<Tuple<int,int,string>> dockinfo;

		protected List<Tuple<int,int,string>> attached;

		protected List<string> otherparams;

		protected bool save;
	}
}
