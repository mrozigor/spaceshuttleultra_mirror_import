#include "Aerodynamics.h"
#include "../SSUMath.h"

namespace Aerodynamics
{

TwoDLookup::TwoDLookup()
{
}

TwoDLookup::~TwoDLookup()
{
}

void TwoDLookup::AddDataRange(double x, const double* yData, const double* liftData, const double* dragData, const double* momentData, unsigned int size)
{
	xValues.push_back(x);
	yValues.push_back(std::vector<double>(yData, yData+size));
	liftLookupTable.push_back(std::vector<double>(liftData, liftData+size));
	dragLookupTable.push_back(std::vector<double>(dragData, dragData+size));
	momentLookupTable.push_back(std::vector<double>(momentData, momentData+size));
}

void TwoDLookup::GetValue(double x, double y, double* cl, double* cd, double* cm) const
{
	unsigned int x1 = GetLowerIndex(xValues, x);
	unsigned int x2 = x1+1;
	unsigned int y1 = GetLowerIndex(yValues[x1], y);
	unsigned int y2 = GetLowerIndex(yValues[x2], y);
	
	*cl = Interpolate(liftLookupTable, x1, y1, x2, y2, x, y);
	*cd = Interpolate(dragLookupTable, x1, y1, x2, y2, x, y);
	*cm = Interpolate(momentLookupTable, x1, y1, x2, y2, x, y);
}

/*unsigned int TwoDLookup::GetLowerBound(const std::vector<double> &data, double value)
{
	// array should be from smallest to largest value
	// look through array until we find value larger than target value
	unsigned int upperBound = data.size()-2;
	for(unsigned int i=1;i<upperBound;i++) {
		if(data[i] > value) return i-1;
	}
	return upperBound;
}*/

double TwoDLookup::Interpolate(const std::vector<std::vector<double> > &data, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, double x, double y) const
{
	double a = linterp(yValues[x1][y1], data[x1][y1], yValues[x1][y1+1], data[x1][y1+1], y);
	double b = linterp(yValues[x2][y2], data[x2][y2], yValues[x2][y2+1], data[x2][y2+1], y);
	return linterp(xValues[x1], a, xValues[x2], b, x);
}

ThreeDLookup::ThreeDLookup()
{
}

ThreeDLookup::~ThreeDLookup()
{
}

void ThreeDLookup::AddDataRange(double z, double x, const double *yData, const double *liftData, const double *dragData, const double *momentData, unsigned int size)
{
	if(zValues[zValues.size()-1] != z) {
		zValues.push_back(z);
		//lookupTables.push_back(TwoDLookup());
	}

	lookupTables[lookupTables.size()-1].AddDataRange(x, yData, liftData, dragData, momentData, size);
}


/*ControlSurfaceLookup::ControlSurfaceLookup()
{
}

ControlSurfaceLookup::~ControlSurfaceLookup()
{
}

void ControlSurfaceLookup::AddData(double mach, double aoa, double deflection, double *data, int arraySize) const
{
}

double ControlSurfaceLookup::GetValue(double mach, double aoa, double deflection)
{
	/*MachMap::iterator machLowerBound, machUpperBound;
	machLowerBound = lookupTable.lower_bound(mach);
	// if lower bound is at or past last element, use last 2 elements for interpolation
	if(machLowerBound >= lookupTable.rbegin()) {
		machUpperBound = machLowerBound;
		--machLowerBound;
	}
	else {
		machUpperBound = machLowerBound+1;
	}*
	MachMap::iterator machLowerBound, machUpperBound;
	FindBounds(lookupTable, machLowerBound, machUpperBound, mach);

	return 0.0;
}

template< class Iterator, class Map >
void ControlSurfaceLookup::FindBounds(Map& table, Iterator& lowerBound, Iterator& upperBound, double value)
{
	/*lowerBound = table.lower_bound(value);
	if(lowerBound == table.end() || lowerBound == table.end()-1) {
		upperBound = table.end()-1;
		lowerBound = upperBound-1;
	}
	else {
		upperBound = lowerBound+1;
	}*
	Iterator end = --table.end();
	for(lowerBound = table.begin(); lowerBound != end; ++lowerBound) {
		if(lowerBound->first <= value) break;
	}
	upperBound = lowerBound;
	++upperBound;
}*/

};