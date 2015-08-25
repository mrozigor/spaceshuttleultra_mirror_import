using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SSUWorkbench.model
{
    class Orbiter
    {
        private string registrationNumber;
        private string name;

        private DateTime availableDate;
        private DateTime outOfServiceDate;

        private List<Mission> missions;
    }
}
