#ifndef _g_BasicValve_H_
#define _g_BasicValve_H_


class BasicValve
{
private:
	double pos;
	double maxrate;

	// motion
	double mpos;
	double mrate;
public:
	/**
	 * Opens valve using specific rate
  	 * @param rate rate
	 */
	bool Open( double rate );

	/**
	 * Opens valve using maximum rate
	 */
	bool Open( void );

	/**
	 * Closes valve using specific rate
  	 * @param rate rate
	 */
	bool Close( double rate );

	/**
	 * Closes valve using maximum rate
	 */
	bool Close( void );

	/**
	 * Moves valve to target position using specific rate
 	 * @param tpos target position
  	 * @param rate rate
	 */
	bool Move( double tpos, double rate );

	/**
	 * Moves valve to target position using maximum rate
 	 * @param tpos target position
	 */
	bool Move( double tpos );

	/**
	 * Returns valve position
	 * @return valve position (range: 0 - closed, 1 - open)
	 */
	double GetPos( void );

	/**
	 * Updates valve position (call from time step functions)
	 * @param dtme sim dt
	 */
	void tmestp( double dtme );

	/**
	 * Use from .scn loading function to set valve position
	 * @param ipos valve position
	 */
	void _backdoor( double ipos );

	/**
	 * Create a new valve
	 * @param initpos initial valve position
	 * @param imaxrate maximum valve motion rate
	 */
	BasicValve( double initpos, double imaxrate );
	~BasicValve( void );
};


#endif// _g_BasicValve_H_


/*
// 1->open, 0->closed
// rate -> %/s
*/
