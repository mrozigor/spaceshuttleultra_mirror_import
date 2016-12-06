using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench
{
    /// <summary>
    /// Describes a payload that is supposed to be installed into the payload bay at any point during the mission
    /// A payload that is fixed can not be removed from the payload bay during the mission.
    /// </summary>
    class PLBPayload
    {
        private bool isFixed;
    }
}
