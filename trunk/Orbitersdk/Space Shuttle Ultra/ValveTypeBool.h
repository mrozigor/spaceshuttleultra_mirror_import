#ifndef _g_ValveTypeBool_H_
#define _g_ValveTypeBool_H_


#include "BasicValve.h"


class ValveTypeBool
{
private:
	BasicValve* vlv;
	bool pos;
public:
	/**
	 * Opens valve
	 */
	bool Open( void );

	/**
	 * Closes valve
	 */
	bool Close( void );

	/**
	 * Returns valve position
	 * @return valve position (ON/OFF)
	 */
	bool GetPos( void );

	/**
	 * Updates valve position (call from time step functions)
	 * @param dtme sim dt
	 */
	void tmestp( double dtme );

	/**
	 * Use from .scn loading function to set valve position
	 * @param ipos valve position
	 */
	void _backdoor( bool ipos );

	/**
	 * Create a new valve (only shows ON/OFF states)
	 * @param initpos initial valve position
	 * @param imaxrate maximum valve motion rate
	 */
	ValveTypeBool( bool initpos, double imaxrate );
	~ValveTypeBool( void );
};


#endif// _g_ValveTypeBool_H_
