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
	friend class SpeedbrakeVerticalLookup;

	std::vector<double> xValues;
	std::vector< std::vector<double> > yValues;

	std::vector< std::vector<double> > liftLookupTable;
	std::vector< std::vector<double> > dragLookupTable;
	std::vector< std::vector<double> > momentLookupTable;
public:
	TwoDLookup();
	virtual ~TwoDLookup();

	void GetValues(double x, double y, double& cl, double& cd, double& cm) const;
protected:
	/**
	 * Adds data to lookup tables
	 * Note that this MUST be called in order of increasing x-value (i.e. 0.5 to 25.0)
	 */
	void AddDataRange(double x, const double* yData, const double* liftData, const double* dragData, const double* momentData, unsigned int size);
	/**
	 * Similar to AddDataRange, but for normal/axial values in stead of lift/drag
	 * Converts values to lift/drag coefficients
	 */
	void AddAxialDataRange(double x, const double* yData, const double* normalData, const double * axialData, const double* momentData, unsigned int size);
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
protected:
	std::vector<double> zValues;
	
	std::vector<TwoDLookup> lookupTables;
public:
	ThreeDLookup();
	virtual ~ThreeDLookup();
	
	void GetValues(double z, double x, double y, double& cl, double& cd, double& cm) const;
protected:
	/**
	 * Adds data to lookup tables
	 * Note that this MUST be called in order of increasing z-value, then increasing x-value
	 */
	void AddDataRange(double mach, double aoa, const double* deflectionData, const double* liftData, const double * dragData, const double* momentData, unsigned int size);
	/**
	 * Similar to AddDataRange, but for normal/axial values in stead of lift/drag
	 * Converts values to lift/drag coefficients
	 */
	void AddAxialDataRange(double mach, double aoa, const double* deflectionData, const double* normalData, const double * axialData, const double* momentData, unsigned int size);
	/**
	 *
	 * @param data table of values
	 * @param z1 index of lower z value
	 */
	//double Interpolate(const std::vector< std::vector<TwoDLookup> >& data, unsigned int z1) const;
};

};

#endif