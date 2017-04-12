using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SSUWorkbench
{
	/// <summary>
	/// Interaction logic for Launch.xaml
	/// </summary>
	public partial class Launch : UserControl
	{
		public Launch()
		{
			InitializeComponent();
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

		private void ckbOMSAssist_Click(object sender, RoutedEventArgs e)
		{
			if (ckbOMSAssist.IsChecked == true) tbOMSAssist.IsEnabled = true;
			else tbOMSAssist.IsEnabled = false;
			return;
		}
	}
}
