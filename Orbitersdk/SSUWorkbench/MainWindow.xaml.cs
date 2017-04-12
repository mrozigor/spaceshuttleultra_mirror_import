using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Ribbon;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using SSUWorkbench.model;


namespace SSUWorkbench
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : RibbonWindow
	{
		private string orbiterpath;
		internal model.Scenario scenario { get; private set; }
		internal Mission mission { get; private set; }


		public MainWindow()
		{
			InitializeComponent();
			
			if (GetOrbiterPath() == false) System.Environment.Exit( 0 );//System.Windows.Forms.Application.Exit();
		}

		private bool GetOrbiterPath()
		{
			// first check if anything has been saved, and if it still exists
			string tmp = SSUWorkbench.Properties.Settings.Default.orbiterexepath;
			if (tmp.Length  > 0)
			{
				if (System.IO.File.Exists( tmp + "orbiter.exe" ))
				{
					orbiterpath = tmp;
					return true;
				}
			}

			OpenFileDialog openfiledialog = new OpenFileDialog();

			openfiledialog.Title = "Locate orbiter.exe...";
			openfiledialog.Filter = "Orbiter executable (orbiter.exe)|orbiter.exe";

			if (openfiledialog.ShowDialog() == true)
			{
				orbiterpath = openfiledialog.FileName.Substring( 0, openfiledialog.FileName.LastIndexOf( "\\" ) + 1 );
				// save
				SSUWorkbench.Properties.Settings.Default.orbiterexepath = orbiterpath;
				SSUWorkbench.Properties.Settings.Default.Save();
				return true;
			}
			return false;
		}

		private void NewMission()
		{
			// load STS-101 mission at launch phase
			mission = new Mission();
			scenario = new model.Scenario( mission );
			DataContext = new// load to screen
			{
				scenario = this.scenario,
				mission = this.mission,
			};
			return;
		}

		private void OpenMission()
		{
			OpenFileDialog openfiledialog = new OpenFileDialog();

			openfiledialog.Title = "Open Missino File...";
			openfiledialog.Filter = "Mission files (*.cfg)|*.cfg|All files (*.*)|*.*";
			openfiledialog.InitialDirectory = orbiterpath + "Missions\\SSU";

			if (openfiledialog.ShowDialog() == true)
			{
				mission = new Mission();
				scenario = new model.Scenario( mission );
				mission.Load( openfiledialog.FileName );
				DataContext = new// load to screen
				{
					scenario = this.scenario,
					mission = this.mission,
				};
				MessageBox.Show( "done!" );
			}
			return;
		}

		private void SaveMission()
		{
			SaveFileDialog savefiledialog = new SaveFileDialog();

			savefiledialog.Title = "Save Mission File...";
			savefiledialog.Filter = "Mission files (*.cfg)|*.cfg";
			savefiledialog.InitialDirectory = orbiterpath + "Missions\\SSU";
			savefiledialog.FileName = mission.MissionName;

			if (savefiledialog.ShowDialog() == true)
			{
				mission.Save( savefiledialog.FileName );
				MessageBox.Show( "done!" );
			}
			return;
		}

		private void ExportScenario()
		{
			SaveFileDialog savefiledialog = new SaveFileDialog();

			savefiledialog.Title = "Export Scenario File...";
			savefiledialog.Filter = "Scenario files (*.scn)|*.scn";
			savefiledialog.InitialDirectory = orbiterpath + "Scenarios\\Space Shuttle Ultra";

			if (savefiledialog.ShowDialog() == true)
			{
				scenario.Save( savefiledialog.FileName );
				MessageBox.Show( "done!" );
			}
			return;
		}

		protected override void OnPreviewKeyDown(System.Windows.Input.KeyEventArgs e)
		{
			if ((e.Key == Key.N) && (Keyboard.IsKeyDown( Key.LeftCtrl ) || Keyboard.IsKeyDown( Key.RightCtrl )))
			{
				NewMission();
				e.Handled = true;
			}
			else if ((e.Key == Key.O) && (Keyboard.IsKeyDown( Key.LeftCtrl ) || Keyboard.IsKeyDown( Key.RightCtrl )))
			{
				OpenMission();
				e.Handled = true;
			}
			else if ((e.Key == Key.S) && (Keyboard.IsKeyDown( Key.LeftCtrl ) || Keyboard.IsKeyDown( Key.RightCtrl )))
			{
				SaveMission();
				e.Handled = true;
			}
			else if ((e.Key == Key.E) && (Keyboard.IsKeyDown( Key.LeftCtrl ) || Keyboard.IsKeyDown( Key.RightCtrl )))
			{
				ExportScenario();
				e.Handled = true;
			}
			
			base.OnPreviewKeyDown(e);
			return;
		}

		private void ribbonNew_Click(object sender, RoutedEventArgs e)
		{
			NewMission();
			return;
		}

		private void ribbonOpen_Click(object sender, RoutedEventArgs e)
		{
			OpenMission();
			return;
		}

		private void ribbonSave_Click(object sender, RoutedEventArgs e)
		{
			SaveMission();
			return;
		}

		private void ribbonExport_Click(object sender, RoutedEventArgs e)
		{
			ExportScenario();
			return;
		}

		private void ribbonOptions_Click(object sender, RoutedEventArgs e)
		{
			// TODO options window to change orbiter.exe path
			return;
		}

		private void ribbonAbout_Click(object sender, RoutedEventArgs e)
		{
			AboutBox box = new AboutBox();
			box.ShowDialog();
			return;
		}

		private void ribbonClose_Click(object sender, RoutedEventArgs e)
		{
			System.Environment.Exit( 0 );//System.Windows.Forms.Application.Exit();
			return;
		}

		private void rbtnCreate_Click(object sender, RoutedEventArgs e)
		{
			scenario.Create();
			return;
		}
	}
}
