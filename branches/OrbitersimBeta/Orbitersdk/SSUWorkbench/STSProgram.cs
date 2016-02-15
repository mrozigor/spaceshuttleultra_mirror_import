using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SSUWorkbench;

namespace SSUWorkbench.model
{
    class STSProgram
    {
        private string name;
        private string description;

        private DateTime beginning;
        private DateTime ending;

        private List<Orbiter> availableOrbiters;
        private List<Orbiter> allOrbiters;
    }
}
