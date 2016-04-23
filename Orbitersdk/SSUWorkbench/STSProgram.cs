using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SSUWorkbench;

namespace SSUWorkbench.model
{
    ///<summary>Description of a Space Shuttle program.</summary>
    ///<remarks>
    ///Defines the resources available to the program and provides links to 
    ///later extensions of the program. 
    ///</remarks>
    ///
    class STSProgram
    {
        /// <summary>
        /// The name of the program
        /// </summary>
        private string name;
        /// <summary>
        /// A description of what the synopsis of this program should be.
        /// </summary>
        private string description;
        /// <summary>
        /// The beginning date of the program. Must be earlier than the beginning of the first launch campaign.
        /// </summary>
        private DateTime beginning;
        /// <summary>
        /// The ending date of the program, if available. Must be later than the end of the final post-launch campaign.
        /// </summary>
        private DateTime ending;
        /// <summary>
        /// A list of the available orbiters in the program. 
        /// </summary>
        private List<Orbiter> availableOrbiters;
        /// <summary>
        /// The list of Missions, that make up the program
        /// </summary>
        private List<Mission> missions;

        /// <summary>
        /// The beginning of the program
        /// </summary>
        public DateTime Beginning
        {
            get { return beginning; }
            set { beginning = value; }
        }

        /// <summary>
        /// The end of the program
        /// </summary>
        public DateTime Ending
        {
            get { return ending; }
            set { ending = value; }
        }
    }
}
