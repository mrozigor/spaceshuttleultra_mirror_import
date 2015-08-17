/****************************************************************************
  This file is part of Space Shuttle Ultra

  Tunnel Adapter Assembly System definition



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
#ifndef __TAA_H_
#define __TAA_H_


#include "..\AtlantisSubsystem.h"


namespace eva_docking
{
	const static char* DEFAULT_MESHNAME_TAA_FORWARD = "SSU\\TAA";
	const static char* DEFAULT_MESHNAME_TAA_AFT = "SSU\\TAA";// TODO uses the same for now (should have struts)

	const VECTOR3 TAA_POS = _V( 0.0, -1.03, 0.0 );// Only X and Y axes used. Z pos is set in Mission.cpp (GetTunnelAdapterAssemblyZPos)

	const double TAA_MASS = 200;// Kg


	class TunnelAdapterAssembly:public AtlantisSubsystem
	{
		private:
			MESHHANDLE hMesh;
			UINT mesh_idx;
		public:
			TunnelAdapterAssembly( AtlantisSubsystemDirector* _director, bool aftTAA = false );
			~TunnelAdapterAssembly( void );

			double GetSubsystemEmptyMass( void ) const {return TAA_MASS;};

			void AddMeshes( const VECTOR3 &ofs );
			void DefineAnimations( const VECTOR3& ofs );
	};
};


#endif// __TAA_H_