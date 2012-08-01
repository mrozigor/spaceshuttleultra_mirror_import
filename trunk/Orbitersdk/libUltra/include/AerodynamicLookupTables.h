#ifndef AERODYNAMICLOOKUPTABLES_H_F18DDB2B_9538_4f87_83D4_159A0E2E3D58
#define AERODYNAMICLOOKUPTABLES_H_F18DDB2B_9538_4f87_83D4_159A0E2E3D58
#pragma once

#include <vector>

namespace Aerodynamics
{

/**
 * 2D lookup tables for cl, cd and cm values.
 * For wing data, x corresponds to mach and y corresponds to AoA
 * For control surface data, x corresponds to AoA and y corresponds to deflection
 */
class TwoDLookup
{
	friend class ThreeDLookup;

	std::vector<double> xValues;
	std::vector< std::vector<double> > yValues;

	std::vector< std::vector<double> > liftLookupTable;
	std::vector< std::vector<double> > dragLookupTable;
	std::vector< std::vector<double> > momentLookupTable;

	unsigned int xLowerIndex, xUpperIndex, yLowerIndex, yUpperIndex;
public:
	TwoDLookup();
	virtual ~TwoDLookup();

	void GetValues(double x, double y, double& cl, double& cd, double& cm);

	static bool Compare(TwoDLookup& l1, TwoDLookup& l2);
protected:
	/**
	 * Adds data to lookup tables.
	 * Should only be used for horizontal aerodynamic data (side force instead of lift/drag coefficients)
	 * Note that this MUST be called in order of increasing mach value (i.e. 0.5 to 25.0)
	 */
	void AddHorizontalDataRange(double beta, const std::vector<double>& deflectionData, const std::vector<double>& sideForceData, const std::vector<double>& yawMomentData, const std::vector<double>& rollMomentData);
	/**
	 * Adds vertical data to lookup tables.
	 * Converts axial values to lift/drag coefficients
	 * Note that this MUST be called in order of increasing mach value (i.e. 0.5 to 25.0)
	 */
	void AddVerticalDataRange(double aoa, const std::vector<double>& deflectionData, const std::vector<double>& normalData, const std::vector<double>& axialData, const std::vector<double>& momentData);
private:
	/**
	 *
	 * @param data table of values
	 * @param x1 index of lower x value
	 * @param y1 index of lower y value corresponding to x1
	 * @param x2 index of upper x value; equal to x1+1
	 * @param y2 index of lower y value corresponding to x2; almost always equal to y1
	 */
	double Interpolate(const std::vector< std::vector<double> >& data, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, double x, double y) const;
};


/**
 * 3D lookup table for cl, cm and cd values
 * x corresponds to aoa, y corresponds to deflection, z corresponds to mach
 */
class ThreeDLookup
{
	std::vector<double> zValues;
	
	std::vector<TwoDLookup> lookupTables;

	unsigned int lowerZIndex;
public:
	ThreeDLookup();
	ThreeDLookup(const char* dataFile, bool isHorizontalData = false);
	virtual ~ThreeDLookup();

	void Init(const char* dataFile, bool isHorizontalData = false);
	
	void GetValues(double mach, double aoa, double deflection, double& cl, double& cd, double& cm);

	static bool Compare(ThreeDLookup& l1, ThreeDLookup& l2);
protected:
	void AddHorizontalDataRange(double mach, double aoa, const std::vector<double>& deflectionData, const std::vector<double>& sideForceData, const std::vector<double>& yawMomentData, const std::vector<double>& rollMomentData);
	/**
	 * Adds vertical data to lookup table
	 * Converts axial values to lift/drag coefficients
	 */
	void AddVerticalDataRange(double mach, double aoa, const std::vector<double>& deflectionData, const std::vector<double>& normalData, const std::vector<double>& axialData, const std::vector<double>& momentData);
	/**
	 * Reads data in single line of csv file into vector
	 * @param line line containing double values delimited by commas
	 */
	void ReadCSVLine(const std::string& line, std::vector<double>& data) const;
};

};

#endif
