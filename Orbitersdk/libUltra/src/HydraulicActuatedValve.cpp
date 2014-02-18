#include "HydraulicActuatedValve.h"
#include "PressureSource.h"
#include "assert.h"
//#include "orbitersdk.h"


HydraulicActuatedValve::HydraulicActuatedValve( double initpos, double rate, PressureSource* PneumaticClose )// TODO complete with hyd press
{
	assert( (initpos >= 0) && (initpos <= 1) && "HydraulicActuatedValve::HydraulicActuatedValve.initpos" );
	assert( (rate > 0) && "HydraulicActuatedValve::HydraulicActuatedValve.rate" );
	pos = initpos;
	this->rate = rate / 100;
	this->PneumaticClose = PneumaticClose;
}

HydraulicActuatedValve::~HydraulicActuatedValve( void )
{
	return;
}

void HydraulicActuatedValve::Connect( DiscreteBundle* pBundle )
{
	dipServovalve[0].Connect( pBundle, 0 );
	dipServovalve[1].Connect( pBundle, 1 );
	dipFailOperationalServoswitch[0].Connect( pBundle, 2 );
	dipFailOperationalServoswitch[1].Connect( pBundle, 3 );
	dipFailSafeServoswitch[0].Connect( pBundle, 4 );
	dipFailSafeServoswitch[1].Connect( pBundle, 5 );
	return;
}

double HydraulicActuatedValve::GetPos( void ) const// TODO use DiscreteBundle
{
	return pos;
}

void HydraulicActuatedValve::tmestp( double dt )
{
	if ((dipFailSafeServoswitch[0].IsSet() == true) || (dipFailSafeServoswitch[1].IsSet() == true))
	{
		// hyd
		int SV = 0;// default SV A
		if ((dipFailOperationalServoswitch[0].IsSet() == false) && (dipFailOperationalServoswitch[1].IsSet() == false)) SV = 1;// SV B

		double mpos = dipServovalve[SV].GetVoltage() / 5;

		if (mpos != pos)
		{
			if (mpos > pos)
			{
				pos += (rate * dt);
				if (pos > mpos) pos = mpos;
			}
			else
			{
				pos -= (rate * dt);
				if (pos < mpos) pos = mpos;
			}
		}
	}
	else
	{
		// pneu
		if (PneumaticClose->Use( 0 ) >= MIN_PRESS_CLOSE_HAV)// just get press
		{
			// close
			PneumaticClose->Use( HE_USE_CLOSE_HAV );// use now
			pos -= (rate * dt);// TODO use correct rate and create pneumatic sequence valve
			if (pos < 0) pos = 0;
		}
	}
	return;
}

void HydraulicActuatedValve::_backdoor( double ipos )
{
	assert( (ipos >= 0) && (ipos <= 1) && "HydraulicActuatedValve::_backdoor.ipos" );
	pos = ipos;
	return;
}