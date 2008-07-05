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

	bool Open( double );
	bool Open( void );// use max rate
	bool Close( double );
	bool Close( void );// use max rate
	bool Move( double );
	bool Move( double, double );
	double GetPos( void );
	void tmestp( double );
	void _backdoor( double );

	BasicValve( double, double );
	~BasicValve( void );
};

#endif// _g_BasicValve_H_

/*
// 1->open, 0->closed
// rate -> %/s
*/
