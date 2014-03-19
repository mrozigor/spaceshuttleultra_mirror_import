using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SSUToolbox.Forms
{
    public partial class Mission : UserControl
    {
        public Mission()
        {
            InitializeComponent();

            if(!Program.standalone)
            {
                generateFiles.Hide();
                missionName.Enabled = false;
                orbiterUsed.Enabled = false;
            }

            orbiterUsed.Items.Add(orbiterNames[1]);
            orbiterUsed.Items.Add(orbiterNames[2]);
            orbiterUsed.Items.Add(orbiterNames[3]);
            orbiterUsed.Items.Add(orbiterNames[4]);
            orbiterUsed.Items.Add(orbiterNames[5]);
            orbiterUsed.Items.Add(orbiterNames[6]);
            orbiterUsed.SelectedItem = orbiterUsed.Items[0];
        }

        public enum Orbiter
        {
            NotSSUOrbiter = 0, Atlantis, Challenger, Columbia, Discovery, Enterprise, Endeavour
        };

        public string[] orbiterNames = { "NotSSUOrbiter", "Atlantis", "Challenger", "Columbia", "Discovery", "Enterprise", "Endeavour" };

        public string m_missionName;
        public Orbiter m_orbiter;

        public void updateParameters()
        {
            m_missionName = missionName.Text;
            m_orbiter = (Orbiter)orbiterUsed.SelectedIndex + 1;
        }

        private void generateFiles_Click(object sender, EventArgs e)
        {
            Control activeForm = this.FindForm();
            ((MainForm)activeForm).callGenerateFiles();
        }
    }
}
