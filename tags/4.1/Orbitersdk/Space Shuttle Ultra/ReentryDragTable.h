#include <fstream>
#include <map>
#include <vector>
#include "stdio.h"
#include <string>

class DragTable
{
public:
	DragTable();
	~DragTable();

private:
	std::vector<std::pair<double,double>> dTable;

public:
	void LoadFromFile();
	double Interpolate(double rho);
	double GetAirDensity(double altitude);
	double TargetAltitude(double target_drag, double speed, double AOA, double mass, double cd);
	double TargetDrag(double range, double speed);
};