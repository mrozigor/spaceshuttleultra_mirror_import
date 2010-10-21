#ifndef AERODYNAMICS_H_5519FCA4_C25D_465c_8D27_0008219CA925
#define AERODYNAMICS_H_5519FCA4_C25D_465c_8D27_0008219CA925
#pragma once

#include "AerodynamicLookupTables.h"

namespace Aerodynamics
{

class VerticalAerodynamicsLookup : public ThreeDLookup			 
{																 
public:															 
	VerticalAerodynamicsLookup();								 
	virtual ~VerticalAerodynamicsLookup();
};

class ElevonVerticalLookup : public ThreeDLookup
{
public:
	ElevonVerticalLookup();
	virtual ~ElevonVerticalLookup();
};

};

#endif