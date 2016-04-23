using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
    /// <summary>
    /// Describes the correct configuration of a payload bay during a mission.
    /// </summary>
    class PayloadBayState
    {
        /// <summary>
        /// Signals, that this payload bay state only contains elements that are fixed during a mission and can't be released or exchanged
        /// </summary>
        private bool isConstant;
        /// <summary>
        /// List of the installed payloads
        /// </summary>
        /// <remarks>
        /// Can be fixed and variable payloads.
        /// </remarks>
        private List<PLBPayload> payloads;
        /// <summary>
        /// Extension of the pressurized volume of the cabin.
        /// </summary>
        private List<PLBPayload> cabinExtension;
    }
}
