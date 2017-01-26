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
	/// Interaction logic for ETSRB.xaml
	/// </summary>
	public partial class ETSRB : UserControl
	{
		public ETSRB()
		{
			InitializeComponent();
		}

		private void cbET_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (cbET.SelectedIndex == 0) ckbFRL.IsEnabled = true;
			else ckbFRL.IsEnabled = false;
		}
	}
}
