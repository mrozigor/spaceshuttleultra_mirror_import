/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel F3 definition



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
#ifndef __PANELF3_H
#define __PANELF3_H


#include "AtlantisPanel.h"
#include "PushButtonIndicator.h"
#include "StandardSwitchCover.h"


namespace vc
{
	class PanelF3:public AtlantisPanel
	{
		StandardSwitchCover* pDragChuteARMCover;
		StandardSwitchCover* pDragChuteDPYCover;
		StandardSwitchCover* pDragChuteJETTCover;

		PushButtonIndicator* pDragChuteARM;
		PushButtonIndicator* pDragChuteDPY;
		PushButtonIndicator* pDragChuteJETT;
	public:
		PanelF3( Atlantis* _sts );
		~PanelF3();

		virtual void Realize();

		virtual void DefineVC();
		virtual void RegisterVC();
	};
};

#endif //__PANELF3_H