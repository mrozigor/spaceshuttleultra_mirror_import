#include "ReentryDragTable.h"
#include "ParameterValues.h"

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
	file.open("Config/SSU_ReentryDrag.csv",std::ios::in);
	if(file.is_open())
	{
		std::string line;
		while(std::getline(file,line))
		{
			double vr, h;
			sscanf(line.c_str(),"%lf%lf",&vr,&h);
			dTable.push_back(std::pair<double,double>(vr,h));
			line.clear();
			vr = h = 0;
		}
		
	}

}

double DragTable::Interpolate(double relVel)
{
	std::vector<std::pair<double,double>>::iterator it;
	
	for(it = dTable.begin(); it != dTable.end(); it++)
	{
		if(it<dTable.end()-1)
		{
			if((it->first > relVel && (it+1)->first < relVel )||(it->first < relVel && (it+1)->first > relVel ) )
				return it->second + ((it+1)->second - it->second)*(relVel-it->first)/((it+1)->first - it->first);

			else if(it->first == relVel)
				return it->second;
		}
		else return it->second;
	}
	
	return 0;
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
	double eah = density/2.04959;
	return log(fabs(eah))/-0.000156935;

}

double DragTable::TargetDrag(double range, double speed)
{
	return ((speed-300)*(speed+300))/(2*range);
}