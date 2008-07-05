#include "BasicValve.h"


BasicValve::BasicValve( double initpos, double imaxrate )
{
	pos = initpos;
	maxrate = imaxrate / 100;

	mpos = pos;
	mrate = maxrate;
}

BasicValve::~BasicValve( void )
{
	// the end
}

bool BasicValve::Open( double rate )
{
	if ((rate > (maxrate * 100)) || (rate <= 0)) return false;
	mpos = 1;
	mrate = rate / 100;
	return true;
}

bool BasicValve::Open( void )
{
	mpos = 1;
	mrate = maxrate;
	return true;
}

bool BasicValve::Close( double rate )
{
	if ((rate > (maxrate * 100)) || (rate <= 0)) return false;
	mpos = 0;
	mrate = rate / 100;
	return true;
}

bool BasicValve::Close( void )
{
	mpos = 0;
	mrate = maxrate;
	return true;
}

bool BasicValve::Move( double tpos, double rate )
{
	if ((rate > (maxrate * 100)) || (rate <= 0)) return false;
	if ((tpos < 0) || (tpos > 1)) return false;
	mpos = tpos;
	mrate = rate / 100;
	return true;
}

bool BasicValve::Move( double tpos )
{
	if ((tpos < 0) || (tpos > 1)) return false;
	mpos = tpos;
	mrate = maxrate;
	return true;
}

double BasicValve::GetPos( void )
{
	return pos;
}

void BasicValve::tmestp( double dtme )
{
	if (mpos != pos)
	{
		// MOVE IT!!!!!!!!!!!
		if (mpos > pos)
		{
			pos = pos + (mrate * dtme);
			if (pos > mpos) pos = mpos;
		}
		else
		{
			pos = pos - (mrate * dtme);
			if (pos < mpos) pos = mpos;
		}
	}
	return;
}
void BasicValve::_backdoor( double ipos )
{
	mpos = ipos;
	pos = ipos;
	return;
}
