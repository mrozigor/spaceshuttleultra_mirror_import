/****************************************************************************
  This file is part of Space Shuttle Ultra

  Drag Chute definition



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
#ifndef __DC_H
#define __DC_H


#include "discsignals.h"
#include "AtlantisSubsystem.h"
#include "Atlantis.h"


using namespace discsignals;


class DragChute:public AtlantisSubsystem
{
	private:
		enum {STOWED, DEPLOYING, REEFED, INFLATED, JETTISONED} DragChuteState;
		UINT mesh_index;
		MESHHANDLE hMesh;

		UINT anim_deploy;
		UINT anim_spin;
		AnimState as_spin;

		double DragChuteDeployTime;
		double DragChuteSize;// 0 (Stowed/Jettisoned) or 0.4 (Reefed) or 1.0 (Deployed)

		DiscOutPort DragChuteARMLT;
		DiscOutPort DragChuteDPYLT;
		DiscOutPort DragChuteJETTLT;
		DiscInPort DragChuteARM[2];
		DiscInPort DragChuteDPY[2];
		DiscInPort DragChuteJETT[2];

		void SpinChute( double DeltaT );

	public:
		DragChute( AtlantisSubsystemDirector* _director );
		virtual ~DragChute();

		virtual void Realize();
		virtual void OnPreStep( double SimT, double DeltaT, double MJD );
		virtual bool OnParseLine( const char* line );
		virtual void OnSaveState( FILEHANDLE scn ) const;
		bool SingleParamParseLine() const {return true;};
		void AddMesh();
		void DefineAnimations();

		void AddAero( void ) const;
};

#endif //__DC_H
