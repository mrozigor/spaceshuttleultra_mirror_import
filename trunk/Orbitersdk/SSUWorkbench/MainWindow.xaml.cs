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

		private void OpenScenario()
		{
			OpenFileDialog openfiledialog = new OpenFileDialog();

			openfiledialog.Title = "Open Scenario File...";
			openfiledialog.Filter = "Scenario files (*.scn)|*.scn|All files (*.*)|*.*";
			openfiledialog.InitialDirectory = orbiterpath + "Scenarios\\Space Shuttle Ultra";

			if (openfiledialog.ShowDialog() == true)
			{
				scenario = new model.Scenario();
				mission = new Mission();
				scenario.Load( openfiledialog.FileName, mission, orbiterpath );
				DataContext = new// load to screen
				{
					scenario = this.scenario,
					mission = this.mission,
				};
				MessageBox.Show( "done!" );
			}
			return;
		}

		private void SaveScenario()
		{
			SaveFileDialog savefiledialog = new SaveFileDialog();

			savefiledialog.Title = "Save Scenario File...";
			savefiledialog.Filter = "Scenario files (*.scn)|*.scn";

			if (savefiledialog.ShowDialog() == true)
			{
				scenario.Save( savefiledialog.FileName, orbiterpath );
				MessageBox.Show( "done!" );
			}
			return;
		}

		private void LaunchSite_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (LaunchSite.SelectedIndex == 0)
			{
				// KSC
				LaunchPad.IsEnabled = true;
				LaunchPad.Items.Clear();
				LaunchPad.Items.Add( "LC-39A" );
				LaunchPad.Items.Add( "LC-39B" );
				LaunchPad.SelectedIndex = 0;

				MLP.IsEnabled = true;
				MLP.Items.Clear();
				MLP.Items.Add( "MLP-1" );
				MLP.Items.Add( "MLP-2" );
				MLP.Items.Add( "MLP-3" );
				MLP.SelectedIndex = 0;
			}
			else
			{
				// VAFB
				LaunchPad.IsEnabled = false;
				LaunchPad.Items.Clear();
				LaunchPad.Items.Add( "SLC-6" );
				LaunchPad.SelectedIndex = 0;

				MLP.IsEnabled = false;
				MLP.Items.Clear();
			}
			return;
		}

		private void ribbonOpen_Click(object sender, RoutedEventArgs e)
		{
			OpenScenario();
			return;
		}

		private void ribbonSave_Click(object sender, RoutedEventArgs e)
		{
			SaveScenario();
			return;
		}

		private void RibbonButton_Click(object sender, RoutedEventArgs e)
		{
			System.Environment.Exit( 0 );//System.Windows.Forms.Application.Exit();
			return;
		}

		protected override void OnPreviewKeyDown(System.Windows.Input.KeyEventArgs e)
		{
			if ((e.Key == Key.O) && (Keyboard.IsKeyDown( Key.LeftCtrl ) || Keyboard.IsKeyDown( Key.RightCtrl )))
			{
				OpenScenario();
				e.Handled = true;
			}
			else if ((e.Key == Key.S) && (Keyboard.IsKeyDown( Key.LeftCtrl ) || Keyboard.IsKeyDown( Key.RightCtrl )))
			{
				SaveScenario();
				e.Handled = true;
			}
			
			base.OnPreviewKeyDown(e);
			return;
		}
	}
}
