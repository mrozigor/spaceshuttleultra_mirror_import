#include "ReentryDragTable.h"
#include "ParameterValues.h"
#include "UltraMath.h"

DragTable::DragTable()
{
	LoadFromFile();
}

DragTable::~DragTable()
{
	dTable.clear();
}

void DragTable::LoadFromFile()
{
	std::fstream file;
	file.open("Config/SSU_Density.csv",std::ios::in);
	if(file.is_open())
	{
		std::string line;
		while(std::getline(file,line))
		{
			double alt, rho;
			sscanf(line.c_str(),"%lf%lf",&alt,&rho);
			dTable.push_back(std::pair<double,double>(alt,rho));
			line.clear();
			alt = rho = 0;
		}
		
	}

}

double DragTable::Interpolate(double rho)
{
	std::vector<std::pair<double,double>>::iterator it;

	/*if(rho >= 1.16696)
		return 500;

	else if(rho <= 5.98467e-007)
		return 100000;*/
	if(rho >= dTable.front().second)
		return dTable.front().first;
	else if(rho <= dTable.back().second)
		return dTable.back().first;

	for(it = dTable.begin(); it != dTable.end(); ++it)
	{
		if(it<dTable.end()-1)
		{
			if((rho>it->second && rho < (it+1)->second) || (rho<it->second && rho > (it+1)->second))
				return linterp(it->second,it->first,(it+1)->second,(it+1)->first,rho);
		}
	}
	// shouldn't reach this point
	it = dTable.end()-2;
	return linterp(it->second,it->first,(it+1)->second,(it+1)->first,rho);
}

double DragTable::GetAirDensity(double altitude)
{
	return  2.04959*exp(-0.000156935*altitude);
}

double DragTable::TargetAltitude(double target_drag, double speed, double AOA, double mass, double cd)
{
	double cd1 = -0.07854;
	double cd2 = -6.15920/1000*AOA;
	double cd3 = -6.21408/10000*pow(AOA,2);
	//double drag = 0.5*GetAirDensity(alt)*pow(speed,2)*(cd1+cd2+cd3)*250/2*mass;
	double density = (target_drag*2*mass)/(pow(speed,2)*cd*ORBITER_WING_AREA);
	return Interpolate(density);

}

double DragTable::TargetDrag(double range, double speed)
{
	return ((speed-300)*(speed+300))/(2*(range-100000));
}