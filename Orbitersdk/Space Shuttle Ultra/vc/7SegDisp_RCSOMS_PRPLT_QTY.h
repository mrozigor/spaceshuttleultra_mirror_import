/****************************************************************************
  This file is part of Space Shuttle Ultra

  RCS/OMS PRPLT QTY 7-Segment Display Controller VC Component



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
#ifndef __7SD_RCSOMS_H
#define __7SD_RCSOMS_H


#include "7SegDisp.h"
#include "DiscInPort.h"


namespace vc
{
	using namespace discsignals;

	class _7SegDisp_RCSOMS_PRPLT_QTY:public _7SegDisp
	{
		private:
			DiscInPort pRCSOMS_PRPLT_QTY_RCSOXID;
			DiscInPort pRCSOMS_PRPLT_QTY_RCSFUEL;
			DiscInPort pRCSOMS_PRPLT_QTY_LOWEST;
			DiscInPort pRCSOMS_PRPLT_QTY_OMSOXID;
			DiscInPort pRCSOMS_PRPLT_QTY_OMSFUEL;

		public:
			_7SegDisp_RCSOMS_PRPLT_QTY( Atlantis* _sts, const std::string& _ident );
			virtual ~_7SegDisp_RCSOMS_PRPLT_QTY();

			void Realize( void );

			void Controller( void );
	};
};

#endif// __7SD_RCSOMS_H