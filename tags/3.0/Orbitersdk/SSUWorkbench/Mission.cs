using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SSUWorkbench.model
{
    class Mission
    {
        private string missionName;
        private string launchSiteName;

        public string LaunchSiteName
        {
            get { return launchSiteName; }
        }

        public string MissionName
        {
            get { return missionName; }
            set { missionName = value; }
        }
    }
}
