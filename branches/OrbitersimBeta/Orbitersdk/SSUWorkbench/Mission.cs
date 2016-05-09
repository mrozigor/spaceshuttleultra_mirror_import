using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
    /// <summary>
    /// Description of a mission in a program.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Describes the resources used in a mission, the launch and landing parameters and the important milestone dates.
    /// For historic reproductions of missions and events, its possible to define additional launch attempts and anomalies.
    /// For free-style missions, it will be possible to define launch window constraints.
    /// </para>
    /// <para>
    /// A mission that is created independent of a program, will get created with a minimal stub program to define its resources.
    /// </para>
    /// </remarks>
    /// <seealso cref="SSUWorkbench.model.STSProgram"/>
    class Mission
    {
        /// <summary>
        /// The program, to which the mission belongs.
        /// </summary>
        private STSProgram program; 
        /// <summary>
        /// The name of the mission, as by the used naming scheme. 
        /// </summary>
        private string missionName;
        /// <summary>
        /// The name of the launch site
        /// </summary>
        private string launchSiteName;

        /// <summary>
        /// The name of the launch site
        /// </summary>
        public string LaunchSiteName
        {
            get { return launchSiteName; }
        }
        /// <summary>
        /// The name of the mission
        /// </summary>
        public string MissionName
        {
            get { return missionName; }
            set { missionName = value; }
        }
    }
}
