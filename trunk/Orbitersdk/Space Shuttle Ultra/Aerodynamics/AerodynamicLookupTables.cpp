#include "AerodynamicLookupTables.h"
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

void TwoDLookup::AddAxialDataRange(double x, const double *yData, const double *normalData, const double *axialData, const double *momentData, unsigned int size)
{
	xValues.push_back(x);
	yValues.push_back(std::vector<double>(yData, yData+size));
	liftLookupTable.push_back(std::vector<double>());
	dragLookupTable.push_back(std::vector<double>());
	momentLookupTable.push_back(std::vector<double>(momentData, momentData+size));

	dragLookupTable[dragLookupTable.size()-1].reserve(size);
	liftLookupTable[liftLookupTable.size()-1].reserve(size);
	for(unsigned int i=0;i<size;i++) {
		VECTOR3 axial = _V(-axialData[i], normalData[i], 0.0);
		VECTOR3 ld = RotateVectorZ(axial, x);
		dragLookupTable[dragLookupTable.size()-1].push_back(-ld.x);
		liftLookupTable[liftLookupTable.size()-1].push_back(ld.y);
	}
}

void TwoDLookup::GetValues(double x, double y, double& cl, double& cd, double& cm) const
{
	unsigned int x1 = GetLowerIndex(xValues, x);
	unsigned int x2 = x1+1;
	unsigned int y1 = GetLowerIndex(yValues[x1], y);
	unsigned int y2 = GetLowerIndex(yValues[x2], y);
	
	cl = Interpolate(liftLookupTable, x1, y1, x2, y2, x, y);
	cd = Interpolate(dragLookupTable, x1, y1, x2, y2, x, y);
	cm = Interpolate(momentLookupTable, x1, y1, x2, y2, x, y);
}

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

void ThreeDLookup::GetValues(double z, double x, double y, double &cl, double &cd, double &cm) const
{
	double cl1, cl2;
	double cd1, cd2;
	double cm1, cm2;

	unsigned int z1 = GetLowerIndex(zValues, z);
	unsigned int z2 = z1+1;
	lookupTables[z1].GetValues(x, y, cl1, cd1, cm1);
	lookupTables[z2].GetValues(x, y, cl2, cd2, cm2);

	cl = linterp(zValues[z1], cl1, zValues[z2], cl2, z);
	cd = linterp(zValues[z1], cd1, zValues[z2], cd2, z);
	cm = linterp(zValues[z1], cm1, zValues[z2], cm2, z);
}

void ThreeDLookup::AddDataRange(double z, double x, const double *yData, const double *liftData, const double *dragData, const double *momentData, unsigned int size)
{
	if(zValues.size()==0 || zValues[zValues.size()-1]!=z) {
		zValues.push_back(z);
		lookupTables.push_back(TwoDLookup());
	}

	lookupTables[lookupTables.size()-1].AddDataRange(x, yData, liftData, dragData, momentData, size);
}

void ThreeDLookup::AddAxialDataRange(double z, double x, const double *yData, const double *normalData, const double *axialData, const double *momentData, unsigned int size)
{
	/*double* liftData = new double[size];
	double* dragData = new double[size];

	for(unsigned int i=0;i<size;i++) {
		VECTOR3 axial = _V(-axialData[i], normalData[i], 0.0);
		VECTOR3 ld = RotateVectorZ(axial, x);
		dragData[i] = -ld.x;
		liftData[i] = ld.y;
	}

	AddDataRange(z, x, yData, liftData, dragData, momentData, size);

	delete[] liftData;
	delete[] dragData;*/
	
	if(zValues.size()==0 || zValues[zValues.size()-1]!=z) {
		zValues.push_back(z);
		lookupTables.push_back(TwoDLookup());
	}

	lookupTables[lookupTables.size()-1].AddAxialDataRange(x, yData, normalData, axialData, momentData, size);
}

};