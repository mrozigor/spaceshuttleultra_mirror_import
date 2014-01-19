#include "SolenoidValve.h"
#include "assert.h"


SolenoidValve::SolenoidValve( double initpos, double rate, bool NormallyClosed, PressureSource* psource, PressureSource* psourceinvent )
{
	assert( (initpos >= 0) && (initpos <= 1) && "SolenoidValve::SolenoidValve.initpos" );
	pos = initpos;
	this->rate = rate / 100;
	this->NormallyClosed = NormallyClosed;
	this->psource = psource;
	this->psourceinvent = psourceinvent;
}

SolenoidValve::~SolenoidValve( void )
{
	return;
}

void SolenoidValve::Connect( int input, DiscreteBundle* pBundle, int iLine )
{
	assert( (input >= 0) && (input <= 1) && "SolenoidValve::Connect.input" );
	dipInput[input].Connect( pBundle, iLine );
	return;
}

double SolenoidValve::GetPos( void ) const
{
	return pos;
}

void SolenoidValve::tmestp( double dt )
{
	double mpos = 0;

	if ((dipInput[0].IsSet() == true) || (dipInput[1].IsSet() == true)) mpos = NormallyClosed;
	else mpos = !NormallyClosed;

	if (mpos != pos)
	{
		if (mpos > pos)
		{
			pos += (rate * dt);
			if (pos > 1) pos = 1;
		}
		else
		{
			pos -= (rate * dt);
			if (pos < 0) pos = 0;
		}
	}
	return;
}

void SolenoidValve::_backdoor( double ipos )
{
	pos = ipos;
	return;
}

double SolenoidValve::Use( double flow )
{
	if (pos == 0)// TODO improve?
	{
		if (psourceinvent != nullptr) return psourceinvent->Use( flow );
		else return 0;
	}
	else return pos * psource->Use( flow );
}