#include <fstream>
#include <string>
#include <sstream>
#include "AerodynamicLookupTables.h"
#include "SSUMath.h"

namespace Aerodynamics
{

TwoDLookup::TwoDLookup()
: xLowerIndex(0), yLowerIndex(0), xUpperIndex(1), yUpperIndex(1)
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

void TwoDLookup::AddAxialDataRange(double x, const std::vector<double>& yData, const std::vector<double>& normalData, const std::vector<double>& axialData, const std::vector<double>& momentData)
{
	xValues.push_back(x);
	yValues.push_back(yData);
	liftLookupTable.push_back(normalData);
	dragLookupTable.push_back(axialData);
	momentLookupTable.push_back(momentData);

	for(unsigned int i=0;i<normalData.size();i++) {
		VECTOR3 axial = _V(-axialData[i], normalData[i], 0.0);
		VECTOR3 ld = RotateVectorZ(axial, x);
		dragLookupTable[dragLookupTable.size()-1][i] = -ld.x;
		liftLookupTable[liftLookupTable.size()-1][i] = ld.y;
	}
}

void TwoDLookup::GetValues(double x, double y, double& cl, double& cd, double& cm)
{
	if(x<xValues[xLowerIndex] || x>xValues[xLowerIndex+1])
		xLowerIndex = GetLowerIndex(xValues, x);
	xUpperIndex = xLowerIndex+1;

	if(y<yValues[xLowerIndex][yLowerIndex] || y>yValues[xLowerIndex][yLowerIndex+1])
		yLowerIndex = GetLowerIndex(yValues[xLowerIndex], y);
	if(y<yValues[xUpperIndex][yUpperIndex] || y>yValues[xUpperIndex][yUpperIndex+1])
		yUpperIndex = GetLowerIndex(yValues[xUpperIndex], y);

	//unsigned int y2 = GetLowerIndex(yValues[x2], y);
	
	cl = Interpolate(liftLookupTable, xLowerIndex, yLowerIndex, xUpperIndex, yUpperIndex, x, y);
	cd = Interpolate(dragLookupTable, xLowerIndex, yLowerIndex, xUpperIndex, yUpperIndex, x, y);
	cm = Interpolate(momentLookupTable, xLowerIndex, yLowerIndex, xUpperIndex, yUpperIndex, x, y);
}

double TwoDLookup::Interpolate(const std::vector<std::vector<double> > &data, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, double x, double y) const
{
	double a = linterp(yValues[x1][y1], data[x1][y1], yValues[x1][y1+1], data[x1][y1+1], y);
	double b = linterp(yValues[x2][y2], data[x2][y2], yValues[x2][y2+1], data[x2][y2+1], y);
	return linterp(xValues[x1], a, xValues[x2], b, x);
}

bool TwoDLookup::Compare(TwoDLookup& l1, TwoDLookup& l2)
{
	for(unsigned int i=0;i<l1.liftLookupTable.size();i++) {
		for(unsigned int j=0;j<l1.liftLookupTable[i].size();j++) {
			if(!Eq(l1.liftLookupTable[i][j], l2.liftLookupTable[i][j])) return false;
			if(!Eq(l1.dragLookupTable[i][j], l2.dragLookupTable[i][j])) return false;
			if(!Eq(l1.momentLookupTable[i][j], l2.momentLookupTable[i][j])) return false;
		}
	}
	return true;
}

ThreeDLookup::ThreeDLookup()
: lowerZIndex(0)
{
}

ThreeDLookup::ThreeDLookup(const char* dataFile)
: lowerZIndex(0)
{
	std::ifstream dataIn(dataFile);
	if(!dataIn.fail()) { // file was opened; read data
		std::string line;
		// read deflection range
		std::vector<double> deflectionRange;
		std::getline(dataIn, line);
		ReadCSVLine(line, deflectionRange);

		unsigned int lineCount = 2; // counter in case file is invalid
		std::vector<double> normalData;
		std::vector<double> axialData;
		std::vector<double> momentData;
		std::vector<double> machAoaValues; 
		while(!dataIn.eof()) {
			std::getline(dataIn, line);
			ReadCSVLine(line, machAoaValues);
			std::getline(dataIn, line);
			ReadCSVLine(line, normalData);
			std::getline(dataIn, line);
			ReadCSVLine(line, axialData);
			std::getline(dataIn, line);
			ReadCSVLine(line, momentData);

			// make sure block of data is valid
			bool error = ( (machAoaValues.size() != 2) ||
				(normalData.size() != deflectionRange.size()) ||
				(axialData.size() != deflectionRange.size()) ||
				(momentData.size() != deflectionRange.size()) );
			if(error) {
				std::ostringstream message;
				message << "ERROR: Invalid aerodynamic data file " << dataFile << std::endl;
				message << "Line " << lineCount;
				oapiWriteLog((char*)message.str().c_str());
				break;
			}

			AddAxialDataRange(machAoaValues[0], machAoaValues[1], deflectionRange, normalData, axialData, momentData);

			lineCount+=4;
		}
		dataIn.close();
	}
	else {
		oapiWriteLog("ERROR: Could not find aerodynamic data file");
		oapiWriteLog((char*)dataFile);
	}

	/*for(unsigned int i=0;i<lookupTables[0].liftLookupTable.size();i++) {
		std::ostringstream stream1;
		std::ostringstream stream2;
		stream1 << zValues[0] << " " << lookupTables[0].xValues[i];
		stream2 << zValues[0] << " "<< lookupTables[0].xValues[i];
		for(unsigned int j=0;j<lookupTables[0].liftLookupTable[i].size();j++) {
			stream1 << " " << lookupTables[0].liftLookupTable[i][j];
			stream2 << " " << lookupTables[0].dragLookupTable[i][j];
		}
		oapiWriteLog((char*)stream1.str().c_str());
		oapiWriteLog((char*)stream2.str().c_str());
	}*/
}

ThreeDLookup::~ThreeDLookup()
{
}

void ThreeDLookup::GetValues(double mach, double aoa, double deflection, double &cl, double &cd, double &cm)
{
	double cl1, cl2;
	double cd1, cd2;
	double cm1, cm2;

	if(mach<zValues[lowerZIndex] || mach>zValues[lowerZIndex+1])
		lowerZIndex = GetLowerIndex(zValues, mach);
	unsigned int upperZIndex = lowerZIndex+1;

	lookupTables[lowerZIndex].GetValues(aoa, deflection, cl1, cd1, cm1);
	lookupTables[upperZIndex].GetValues(aoa, deflection, cl2, cd2, cm2);

	cl = linterp(zValues[lowerZIndex], cl1, zValues[upperZIndex], cl2, mach);
	cd = linterp(zValues[lowerZIndex], cd1, zValues[upperZIndex], cd2, mach);
	cm = linterp(zValues[lowerZIndex], cm1, zValues[upperZIndex], cm2, mach);
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

void ThreeDLookup::AddAxialDataRange(double mach, double aoa, std::vector<double> deflectionData, std::vector<double> normalData, std::vector<double> axialData,  std::vector<double> momentData)
{	
	if(zValues.size()==0 || zValues[zValues.size()-1]!=mach) {
		zValues.push_back(mach);
		lookupTables.push_back(TwoDLookup());
	}

	lookupTables[lookupTables.size()-1].AddAxialDataRange(aoa, deflectionData, normalData, axialData, momentData);
}

void ThreeDLookup::ReadCSVLine(const std::string &line, std::vector<double> &data) const
{
	data.clear(); // empty vector before adding data
	std::istringstream inputStream(line);
	while(!inputStream.eof()) {
		double value;
		inputStream>>value;
		data.push_back(value);
		inputStream.ignore(1, ',');
	}
}

bool ThreeDLookup::Compare(ThreeDLookup& l1, ThreeDLookup& l2)
{
	if(l1.zValues.size() != l2.zValues.size()) return false;
	
	for(unsigned int i=0;i<l1.lookupTables.size();i++) {
		if(!TwoDLookup::Compare(l1.lookupTables[i], l2.lookupTables[i]))
			return false;
	}
	return true;
}

};