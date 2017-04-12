/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel L2 definition



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
#ifndef __VC_PANELL2_H
#define __VC_PANELL2_H


#include "AtlantisPanel.h"


namespace vc
{
	class PanelL2:public AtlantisPanel
	{
			MESHHANDLE hPanelMesh;
			UINT mesh_index;

		public:
			PanelL2( Atlantis* _sts );
			virtual ~PanelL2();

			virtual void AddMeshes( const VECTOR3& ofs );
			virtual void SetMeshVisibility( bool visible );
			virtual UINT GetVCMeshIndex( void ) const;
			virtual void DefineVC();
			virtual void RegisterVC();
			void DefineVCAnimations( UINT vcidx );
			virtual void Realize();
	};
}

#endif// __VC_PANELL2_H
