#ifndef _g_ValveTypeBool_H_
#define _g_ValveTypeBool_H_

#include "BasicValve.h"


class ValveTypeBool
{
private:
	BasicValve* vlv;
	bool pos;
public:
	bool Open( void );
	bool Close( void );
	bool GetPos( void );
	void tmestp( double );
	void _backdoor( bool );

	ValveTypeBool( bool, double );
	~ValveTypeBool( void );
};

#endif// _g_ValveTypeBool_H_
