#include "ValveTypeBool.h"


ValveTypeBool::ValveTypeBool( bool initpos, double imaxrate )
{
	vlv = new BasicValve( (double)initpos, imaxrate );
	pos = initpos;
}

ValveTypeBool::~ValveTypeBool( void )
{
	delete vlv;
}

bool ValveTypeBool::Open( void )
{
	return vlv->Open();
}

bool ValveTypeBool::Close( void )
{
	return vlv->Close();
}

bool ValveTypeBool::GetPos( void )
{
	return pos;
}

void ValveTypeBool::tmestp( double dtme )
{
	vlv->tmestp( dtme );
	double vlvpos = vlv->GetPos();

	if (pos == true)
	{
		if (vlvpos == 0) pos = false;
	}
	else
	{
		if (vlvpos == 1) pos = true;
	}
	return;
}

void ValveTypeBool::_backdoor( bool ipos )
{
	vlv->_backdoor( (double)ipos );
	return;
}