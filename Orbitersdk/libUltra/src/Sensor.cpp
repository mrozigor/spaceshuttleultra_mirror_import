#include "Sensor.h"
#include "assert.h"


Sensor::Sensor( double MaxValue, double MinValue )
{
	assert( (MinValue < MaxValue) && "Sensor::Sensor.(MinValue < MaxValue)" );
	this->MaxValue = MaxValue;
	this->MinValue = MinValue;
	conversor = 5 / (MaxValue - MinValue);
	return;
}

Sensor::~Sensor( void )
{
	return;
}

void Sensor::Connect( DiscreteBundle* pBundle, int iLine )
{
	dipOutput.Connect( pBundle, iLine );
	return;
}

void Sensor::SetValue( double value )
{
	if (value > MaxValue) value = MaxValue;
	else if (value < MinValue) value = MinValue;

	dipOutput.SetLine( value * conversor );
	return;
}