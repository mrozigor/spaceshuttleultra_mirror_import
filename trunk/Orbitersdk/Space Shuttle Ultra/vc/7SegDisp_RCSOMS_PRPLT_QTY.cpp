#include "7SegDisp_RCSOMS_PRPLT_QTY.h"
#include "..\Atlantis.h"


namespace vc
{
	_7SegDisp_RCSOMS_PRPLT_QTY::_7SegDisp_RCSOMS_PRPLT_QTY( Atlantis* _sts, const std::string& _ident ):_7SegDisp( _sts, _ident )
	{
	}

	_7SegDisp_RCSOMS_PRPLT_QTY::~_7SegDisp_RCSOMS_PRPLT_QTY()
	{
	}

	void _7SegDisp_RCSOMS_PRPLT_QTY::Realize( void )
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "O3_Timer_OMSRCS", 10 );
		pRCSOMS_PRPLT_QTY_RCSOXID.Connect( pBundle, 5 );
		pRCSOMS_PRPLT_QTY_RCSFUEL.Connect( pBundle, 6 );
		pRCSOMS_PRPLT_QTY_LOWEST.Connect( pBundle, 7 );
		pRCSOMS_PRPLT_QTY_OMSOXID.Connect( pBundle, 8 );
		pRCSOMS_PRPLT_QTY_OMSFUEL.Connect( pBundle, 9 );
		return;
	}

	void _7SegDisp_RCSOMS_PRPLT_QTY::Controller( void )
	{
		short qtyL = 0;
		short qtyF = 0;
		short qtyR = 0;

		if (pRCSOMS_PRPLT_QTY_RCSOXID.IsSet())
		{
			qtyL = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_lrcs )) );
			qtyF = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_frcs )) );
			qtyR = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_rrcs )) );
		}
		else if (pRCSOMS_PRPLT_QTY_RCSFUEL.IsSet())
		{
			qtyL = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_lrcs )) );
			qtyF = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_frcs )) );
			qtyR = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_rrcs )) );
		}
		else if (pRCSOMS_PRPLT_QTY_LOWEST.IsSet())
		{
			qtyL = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_lrcs )) );
			qtyF = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_frcs )) );
			qtyR = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_rrcs )) );
		}
		else if (pRCSOMS_PRPLT_QTY_OMSOXID.IsSet())
		{
			qtyL = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_oms )) );
			qtyF = 0;
			qtyR = qtyL;
		}
		else if (pRCSOMS_PRPLT_QTY_OMSFUEL.IsSet())
		{
			qtyL = min( 99, static_cast<short>(STS()->GetPropellantLevel( STS()->ph_oms )) );
			qtyF = 0;
			qtyR = qtyL;
		}

		next_state[5] = qtyR / 10;
		next_state[4] = qtyR % 10;
		next_state[3] = qtyF / 10;
		next_state[2] = qtyF % 10;
		next_state[1] = qtyL / 10;
		next_state[0] = qtyL % 10;
		return;
	}
};