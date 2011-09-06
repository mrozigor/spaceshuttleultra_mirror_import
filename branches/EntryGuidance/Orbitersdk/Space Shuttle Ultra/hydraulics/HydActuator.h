/****************************************************************************
  This file is part of Space Shuttle Ultra

  Hydraulic Actuator definition



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

#include "DiscreteBundleManager.h"
#include "DiscInPort.h"
#include "DiscOutPort.h"

namespace hydraulics {

	typedef enum _HydAct
	{
		_1P,
		_1Y,
		_2P,
		_2Y,
		_3P,
		_3Y,
		_LHSRBT,
		_LHSRBR,
		_RHSRBT,
		_RHSRBR
	} HydAct;

	using namespace discsignals;

	class HydActuator {
		/**
		 * Command input port A
		 */
	public:
		HydAct ID;
		DiscreteBundleManager* bundle;

		DiscInPort in_servovalve_ATVC1;
		DiscInPort in_servovalve_ATVC2;
		DiscInPort in_servovalve_ATVC3;
		DiscInPort in_servovalve_ATVC4;

		DiscInPort in_isolationvalve_ATVC1;
		DiscInPort in_isolationvalve_ATVC2;
		DiscInPort in_isolationvalve_ATVC3;
		DiscInPort in_isolationvalve_ATVC4;

		DiscOutPort out_deltapressure_ATVC1;
		DiscOutPort out_deltapressure_ATVC2;
		DiscOutPort out_deltapressure_ATVC3;
		DiscOutPort out_deltapressure_ATVC4;

		DiscOutPort out_positiontransducer_ATVC1;
		DiscOutPort out_positiontransducer_ATVC2;
		DiscOutPort out_positiontransducer_ATVC3;
		DiscOutPort out_positiontransducer_ATVC4;

		HydActuator( HydAct nID, DiscreteBundleManager* BundleManager );
		virtual ~HydActuator();

		void Realize( void );
		void OnPostStep( double fSimT, double fDeltaT, double fMJD );
	};
};