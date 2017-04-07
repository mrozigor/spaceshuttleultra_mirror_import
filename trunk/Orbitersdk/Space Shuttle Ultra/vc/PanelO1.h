/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel O1 definition



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
#ifndef __VC_PANELO1_H
#define __VC_PANELO1_H


#include "AtlantisPanel.h"
#include "StandardLight.h"
#include "discsignals.h"


namespace vc
{
	class PanelO1 : public AtlantisPanel
	{
		private:
			MESHHANDLE hPanelMesh;
			UINT mesh_index;

			StandardSingleLight* pGPCLight[25];

			DiscInPort lighttest[2];
			DiscInPort lightcmd[25];
			DiscOutPort lightoutput[25];

		public:
			PanelO1( Atlantis* _sts );
			virtual ~PanelO1();

			virtual void AddMeshes( const VECTOR3& ofs );
			virtual void SetMeshVisibility( bool visible );
			virtual UINT GetVCMeshIndex( void ) const;
			virtual void DefineVC( void );
			virtual void RegisterVC( void );
			virtual void DefineVCAnimations( UINT vcidx );
			virtual void Realize( void );
			virtual void OnPostStep( double SimT, double DeltaT, double MJD );
	};
}

#endif// __VC_PANELO1_H