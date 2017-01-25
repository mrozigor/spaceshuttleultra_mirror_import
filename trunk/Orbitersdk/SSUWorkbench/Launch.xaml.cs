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

		private void ckbOMSAssist_Click(object sender, RoutedEventArgs e)
		{
			if (ckbOMSAssist.IsChecked == true) tbOMSAssist.IsEnabled = true;
			else tbOMSAssist.IsEnabled = false;
			return;
		}
	}
}
