#pragma once

#include "../SimpleGPCSystem.h"
#include "../GPC.h"

namespace dps {
	namespace simple {
		class FCOSServiceProvider;
		class Partition;
		class VirtualIOP;
		class GPCProcess;
		class MemoryOverlayProvider;

		/**
		 * \brief Provides equivalents to the FCOS operating system calls of the PASS operating system
		 */
		class FCOSServiceProvider {
			Partition &host;
		protected:
			void beginSVC();
		public:
			/**
			 * Create a new FCOS Service Provider for this partition
			 */
			FCOSServiceProvider(Partition &_host);
			/**
			 * \brief SVC #1: SCHEDULE
			 *
			 * Schedule a new process for execution. Contrary to the real hardware,the SimpleDPSSystem
			 * implementation will execute C++ objects and not AP-101S instructions. Concepts of the real 
			 * implementation will get C++ equivalents. The only data that such processes can store 
			 * themselves is process execution state (program block address) or temporary variables.
			 * All other data is stored in the compools, which can be accessed by the processes. 
			 * (FCOS compool, major function overlay, ops overlay)
			 *
			 * \param tscdPrio The priority of the process, 0 = lowest, idle
			 * \param tscdFlgs The process flags with the execution conditions
			 * \param tscdPDE The process directory element of the process, in this implemention
			 *  described as GPCProcess object, which can return its own PDE
			 * \param tscdExp1 The event on which the process should get executed
			 * \param tscdExp2 The event on which the process should get cancelled
			 * \param T_at_or_in The time in seconds, at which a process should get executed (AT, GMT) 
			 * or the delta in seconds (IN), when a process should get executed
			 * \param T_repeat The time in seconds for the REPEAT EVERY or REPEAT AFTER conditions
			 * \param T_until The time in seconds GMT, until a process should get executed
			 */
			void Schedule(unsigned char tscdPrio, unsigned short tscdFlgs, dps::simple::GPCProcess *tscdPDE, 
				dps::EVENT *tscdExp1, dps::EVENT* tscdExp2, double T_at_or_in, double T_repeat, double T_until);
			/**
			 * \brief SVC #19: OVERLAY
			 * 
			 * Load a memory configuration into application memory. 
			 * 
			 * The SimpleDPSSystem implementation will override MMU or databus activity regarding the program loading 
			 * and will simply manipulate data structures in memory created that way by mission constants. The main 
			 * process of the OPS or major function will be replaced by C++ equivalents executed by the current partition.
			 *
			 * Loading and saving of application data is delegated to translating objects, that load and save data from or into 
			 * scenario files in a human-readable form. 
			 *
			 * \param toplSorc The source of the memory overlay: 0 = MMU, 1-5 other GPC
			 * \param toplFnBs The phase number of the major function base. 0 if no function base is requested
			 * \param toplPOvl The phase number of the OPS overlay. 
			 * \param toplEvnt The address of an event variable to signal that the overlay operation has completed
			 * \param toplStat The address of a full word to store the I/O status
			 */
			void Overlay(unsigned char toplSorc, unsigned short toplFnBs, unsigned short toplPOvl, dps::EVENT *toplEvnt, unsigned long *toplStat);
		};

		class VirtualIOP {
			Partition &host;
		public:
			VirtualIOP(Partition &_host);
		};

		/**
		 * \brief Describes a partition of the GPCs. Can be a single GPC in simplex or common set operation, or multiple GPCs in a 
		 * redundant set with the same memory configuration
		 */
		class Partition {
		protected:
			FCOSServiceProvider fcos;
			VirtualIOP iop;
			std::vector<GPC*> partitionGPCs;

			void checkAssignedGPCs();
		public:
			Partition(dps::SimpleGPCSystem* host);

			void includeGPC(dps::GPC* gpc);
			void excludeGPC(dps::GPC* gpc);
			bool isEmpty() const;
			void execute(double fSimT, double fSimDT);
			void saveState(FILEHANDLE scn);
			bool parseLine(char* line);
		};
	}
}