using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SSUWorkbench.model
{
    /// <summary>
    /// Description of an orbiter resource, that is taking part in a program or mission
    /// </summary>
    /// <remarks>
    /// Describes the properties of an Orbiter that is available in a program/storyline. An 
    /// orbiter can be set to enter service or be removed from service at a specific date.
    /// </remarks>
    class Orbiter
    {
        /// <summary>
        /// The registration (OV-) or similar of the orbiter
        /// </summary>
        private string registrationNumber;
        /// <summary>
        /// The given name/call sign of the orbiter
        /// </summary>
        private string name;
        /// <summary>
        /// The date at which the orbiter became available for a launch campaign
        /// </summary>
        private DateTime availableDate;
        /// <summary>
        /// The date at which the orbiter is taken out of service
        /// </summary>
        /// Can be null, for signaling that this orbiter stays active until the end of the program.
        private DateTime outOfServiceDate;
        /// <summary>
        /// List of missions in the program, which are refering to this orbiter.
        /// </summary>
        private List<Mission> missions;
    }
}
