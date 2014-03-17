using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace SSUToolbox.Forms
{
    public partial class MainForm : Form
    {
        private Mission mission;
        private MECO meco;
        private Kits kits;
        private Landing landing;
        private Crew crew;
        private FlightData flightData;

        public MainForm()
        {
            InitializeComponent();

            if(!Program.standalone)
                this.Text = "SSU Mission Control";

            tabControl.TabPages.RemoveAt(0);

            TabPage myTabPage;
            myTabPage = new TabPage("General");
            mission = new Mission();
            mission.Dock = DockStyle.Fill;
            myTabPage.Controls.Add(mission);
            tabControl.TabPages.Add(myTabPage);

            myTabPage = new TabPage("MECO");
            meco = new MECO();
            mission.Dock = DockStyle.Fill;
            myTabPage.Controls.Add(meco);
            tabControl.TabPages.Add(myTabPage);

            myTabPage = new TabPage("Mission Kits");
            kits = new Kits();
            mission.Dock = DockStyle.Fill;
            myTabPage.Controls.Add(kits);
            tabControl.TabPages.Add(myTabPage);

            myTabPage = new TabPage("Landing");
            landing = new Landing();
            mission.Dock = DockStyle.Fill;
            myTabPage.Controls.Add(landing);
            tabControl.TabPages.Add(myTabPage);

            myTabPage = new TabPage("Crew");
            crew = new Crew();
            mission.Dock = DockStyle.Fill;
            myTabPage.Controls.Add(crew);
            tabControl.TabPages.Add(myTabPage);

            myTabPage = new TabPage("Flight Data Files");
            flightData = new FlightData();
            mission.Dock = DockStyle.Fill;
            myTabPage.Controls.Add(flightData);
            tabControl.TabPages.Add(myTabPage);

            //orbiter path
            orbiterPath = Directory.GetCurrentDirectory();
            if (Program.standalone)
                orbiterPath = orbiterPath.Remove(orbiterPath.LastIndexOf('\\'));
            orbiterPath += "\\";
            string exePath = orbiterPath + "orbiter.exe";
            if(!File.Exists(exePath))
            {
                MessageBox.Show("Please put this application in the \"SSUToolbox\" directory inside your Orbiter folder.", "Error",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                orbiterPath = "";
            }
        }

        private string orbiterPath;

        string boolString(bool val)
        {
            if (val) return "TRUE";
            else return "FALSE";
        }

        public void callGenerateFiles(){ generateFiles(); }

        private void generateFiles()
        {
            if(orbiterPath=="")
            {
                MessageBox.Show("Please put this application in the \"SSUToolbox\" directory inside your Orbiter folder.", "Error",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            mission.updateParameters();
            meco.updateParameters();
            //kits.updateParameters();

            string missionFile = orbiterPath;
            missionFile += "/Missions/SSU/" + mission.m_missionName + ".cfg";

            string content =
                "Name=" + mission.m_missionName + "\n"
                + "Orbiter=" + mission.orbiterNames[(int)mission.m_orbiter] + "\n"
                //+ "OrbiterTexture=" + ? + "\n"
                + "TargetINC=" + String.Format("{0}", meco.dfTargetInc) + "\n"
                + "TargetLAN=" + String.Format("{0}", meco.dfTargetLAN) + "\n"
                + "MECOAlt=" + String.Format("{0}", meco.dfMECOAlt) + "\n"
                + "MECOVel=" + String.Format("{0}", meco.dfMECOVel) + "\n"
                + "MECOFPA=" + String.Format("{0}", meco.dfMECOFPA) + "\n"
                //+ "UseExtAl=" + "FALSE" + "\n"
                + "UseRMS=" + "FALSE" + "\n"
                + "UseODS=" + "FALSE" + "\n"
                + "UseKUBand=" + "FALSE" + "\n"
                //+ "PerformRollToHeadsUp=" + "FALSE" + "\n"
                //+ "MaxSSMEThrust=" + "100.0" + "\n"
                //+ "HasBulkheadFloodlights=" + "TRUE" + "\n"
                ;

            System.IO.File.WriteAllText(missionFile, content);

            if (!File.Exists(missionFile))
            {   MessageBox.Show("Failed creating the file; watch for special characters on the mission name.", "Error",
                                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            MessageBox.Show("Mission file created.", "Done", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
    }
}
