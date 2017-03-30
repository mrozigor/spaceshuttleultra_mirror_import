/****************************************************************************
  This file is part of Space Shuttle Ultra

  Standard light VC component



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

#include "BasicLight.h"
#include "DiscInPort.h"


namespace vc {

	using discsignals::DiscInPort;

	class StandardSingleLight : public BasicLight
	{
		protected:
			UINT panelmesh;
			UINT grpIndex;

			float offsetU[2];
			float offsetV[2];

		public:
			StandardSingleLight( Atlantis* _sts, const string& _ident );
			~StandardSingleLight();
			
			virtual void SetDefaultState( unsigned int _state );
			virtual void SetStateOffset( unsigned int _state, float _U, float _V );
			virtual bool ConnectLight( unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine );

			virtual void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			void UpdateUV( void );
			void DefineMeshGroup( UINT _panelmesh, UINT _grpIndex );
			void VisualCreated( void );

			DiscInPort input[2];
	};

	class StandardDoubleLight : public BasicLight
	{
		protected:
			UINT panelmesh;
			UINT grpIndex;

			float offsetU[4];
			float offsetV[4];

		public:
			StandardDoubleLight( Atlantis* _sts, const string& _ident );
			~StandardDoubleLight();
			
			virtual void SetDefaultState( unsigned int _state );
			virtual void SetStateOffset( unsigned int _state, float _U, float _V );
			virtual bool ConnectLight( unsigned short usPort, DiscreteBundle* pBundle, unsigned short usLine );

			virtual void OnPostStep( double fSimT, double fDeltaT, double fMJD );

			void UpdateUV( void );
			void DefineMeshGroup( UINT _panelmesh, UINT _grpIndex );
			void VisualCreated( void );

			DiscInPort input[4];
	};
}
