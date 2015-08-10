/****************************************************************************
  This file is part of Space Shuttle Ultra

  Centaur Integrated Support Structure definition



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
#ifndef __SSUCISS_H
#define __SSUCISS_H


#include "orbitersdk.h"


const static char* G_MESHNAME = "SSU\\CISS_CentaurG";
const static char* GPRIME_MESHNAME = "SSU\\CISS_CentaurG_Prime";

const double G_EMPTY_MASS = 3832.856;// Kg
const double GPRIME_EMPTY_MASS = 4036.972;// Kg

const double CISS_ROTATION_RATE = (1/58.0); // rate at which table rotates to deploy position

class SSU_CISS: public VESSEL2
{
	public:
		SSU_CISS( OBJHANDLE hVessel );
		~SSU_CISS( void );

		void clbkSetClassCaps( FILEHANDLE cfg );
		int clbkConsumeBufferedKey( DWORD key, bool down, char* kstate );

		void clbkPostStep(double simT, double simDT, double mjd);

		void clbkSaveState (FILEHANDLE scn);
		void clbkLoadStateEx (FILEHANDLE scn, void *vs);
	private:
		void DefineCentaurGPrimeAnimations();
		void DefineCentaurGAnimations();

		MESHHANDLE hMesh;
		UINT mesh_idx;
		ATTACHMENTHANDLE ahToOV;
		ATTACHMENTHANDLE ahToCentaur;

		UINT anim_rotate;
		VECTOR3 centaurAttachment[3];
		AnimState rotateAnimState;

		bool bFirstStep;
};

#endif// __SSUCISS_H