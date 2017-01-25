using System;
using System.Collections.Generic;
using System.ComponentModel;
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
	/// Interaction logic for Orbiter.xaml
	/// </summary>
	public partial class Orbiter : UserControl
	{
		public Orbiter()
		{
			InitializeComponent();
		}

		private void cbOV_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			// enable/disable controls according to selected OV
			if (cbOV.SelectedIndex == 0)// OV-102
			{
				ckbSILTS.IsEnabled = true;
				ckbSILTS.IsChecked = true;
			}
			else
			{
				ckbSILTS.IsEnabled = false;
				ckbSILTS.IsChecked = false;
			}

			if ((cbOV.SelectedIndex == 2) || (cbOV.SelectedIndex == 4))// OV-103 || OV-105
			{
				ckbFwdBulkDockLights.IsEnabled = true;
			}
			else
			{
				ckbFwdBulkDockLights.IsEnabled = false;
				ckbFwdBulkDockLights.IsChecked = true;
			}

			return;
		}

		private void ckbExtAL_Click(object sender, RoutedEventArgs e)
		{
			if (ckbExtAL.IsChecked == true)
			{
				ckbODS.IsEnabled = true;
				ckbODS.IsChecked = true;
			}
			else
			{
				ckbODS.IsEnabled = false;
				ckbODS.IsChecked = false;
			}
		}

		private void ckbSTBDMPMs_Click(object sender, RoutedEventArgs e)
		{
			// TODO OBSS
		}
	}
}
