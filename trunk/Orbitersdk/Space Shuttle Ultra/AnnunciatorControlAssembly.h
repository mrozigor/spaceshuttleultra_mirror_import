/****************************************************************************
  This file is part of Space Shuttle Ultra

  Annunciator Control Assembly definition



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
#ifndef __ACA_H_
#define __ACA_H_


#include "AtlantisSubsystem.h"
#include "DiscInPort.h"
#include "DiscOutPort.h"


using namespace discsignals;


class AnnunciatorControlAssembly:public AtlantisSubsystem
{
	private:
		short ID;
		DiscInPort power[2];
		DiscInPort odd[2];
		DiscInPort even[2];
		DiscInPort cmd[40];
		DiscOutPort output[40];
		
	public:
		AnnunciatorControlAssembly( AtlantisSubsystemDirector* _director, const string& _ident, short nID );
		~AnnunciatorControlAssembly( void );

		void Realize( void );
		void OnPreStep( double fSimT, double fDeltaT, double fMJD );
};


#endif// __ACA_H_