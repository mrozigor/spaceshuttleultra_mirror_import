/****************************************************************************
  This file is part of Space Shuttle Ultra

  General Purpose Computer definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#pragma once 

#include "../AtlantisSubsystem.h"
#include "../discsignals/DiscInPort.h"
#include "../discsignals/DiscOutPort.h"
#include "BIU.h"

namespace dps {

	using class discsignals::DiscDemultiplex16;
	using class discsignals::DiscMultiplex16;
	using class discsignals::DiscInPort;

	class GPCSoftware;

#pragma pack()

	typedef struct __psw {
		unsigned int uIA:24;
		unsigned int uPM:4;
		unsigned int uCC:2;
		unsigned int uILC:2;
		unsigned int uIntCode:16;
		unsigned int bProblemState:1;
		unsigned int bWaitState:1;
		unsigned int bMachineCheckMask:1;
		unsigned int bASCIIMode:1;
		unsigned int bProtKey:4;
		unsigned int SystemMask:8;
	} AP101PSW;

	/**
	 * base class for all GPC subsystem units. Must implement IOP and CPU 
	 * behavior. 
	 */
	class GPC:public AtlantisSubsystem {
	protected:
		unsigned short usGPCID;

		long lGPR[16];
		float fFPR[8];

		AP101PSW psw;
		
		unsigned short usMajorCount;
		unsigned short usMinorCount;

		virtual void OnIOPPrestep(double fSimT, double fDeltaT, double fMJD);
		virtual void MinorCycle(double fSimT, double fDeltaT, double fMJD);
		virtual void MajorCycle(double fSimT, double fDeltaT, double fMJD);

		/**
		 * Fill the first memory page (4096 byte) by PROM data with 
		 * information about how to find the IPL.
		 */
		virtual void GeneratePROMData() = 0;
	public:
		GPC(SubsystemDirector* _direct, const string& _ident, unsigned short usGPCID);
		virtual ~GPC();
		virtual void IPL();
		virtual void LoadMemoryConfiguration(unsigned short usMemConfig);

		//Functions for handling data blocks in GPC memory
		virtual void* AllocateGPCMemory(unsigned long usSize);
		virtual void FreeGPCMemory(void* pMemBlock);
		//clear the whole memory
		virtual void ClearGPCMemory();
		/**
		 * Clear the whole memory reserved for applications
		 */
		virtual void FreeApplicationMemory();
		
	
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
	public:
		//@todo: Number is not unknown, needs research!
		DiscDemultiplex16 disc_in[4];
		DiscMultiplex16 disc_out;
		BIU channel[24];
		DiscInPort power_on;
		DiscInPort ipl;
		DiscInPort ipl_source1;
		DiscInPort ipl_source2;
		DiscInPort run;
		DiscInPort halt;
	};

};