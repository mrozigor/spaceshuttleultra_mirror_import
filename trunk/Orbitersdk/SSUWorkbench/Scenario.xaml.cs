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
	/// Interaction logic for Scenario.xaml
	/// </summary>
	public partial class Scenario : UserControl
	{
		public Scenario()
		{
			InitializeComponent();
		}

		private void cbCameraMode_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (cbCameraMode.SelectedIndex == 0)// Cockpit
			{
				cbCameraTrackMode.IsEnabled = false;
				tbCameraTrackModeTarget.IsEnabled = false;
				tbCameraPosX.IsEnabled = false;
				tbCameraPosY.IsEnabled = false;
				tbCameraPosZ.IsEnabled = false;
			}
			else// Extern
			{
				cbCameraTrackMode.IsEnabled = true;
				tbCameraTrackModeTarget.IsEnabled = true;
				tbCameraPosX.IsEnabled = true;
				tbCameraPosY.IsEnabled = true;
				tbCameraPosZ.IsEnabled = true;
			}
			return;
		}

		private void cbCameraTrackMode_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if ((cbCameraTrackMode.SelectedIndex == 0) || // TargetRelative
				(cbCameraTrackMode.SelectedIndex == 1) || // AbsoluteDirection
				(cbCameraTrackMode.SelectedIndex == 2))// GlobalFrame
			{
				tbCameraTrackModeTarget.IsEnabled = false;
			}
			else// TargetTo, TargetFrom, Ground
			{
				tbCameraTrackModeTarget.IsEnabled = true;
			}
		}
	}
}
