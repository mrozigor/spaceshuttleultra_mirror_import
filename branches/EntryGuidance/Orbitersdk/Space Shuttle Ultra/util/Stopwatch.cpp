#include "Stopwatch.h"

Stopwatch::Stopwatch()
{
	QueryPerformanceFrequency(&liFrequency);  
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::Start()
{
	QueryPerformanceCounter(&liLast);
}

double Stopwatch::Stop()
{
	QueryPerformanceCounter(&liCurrent);

	double fPerfElapsed =(((liCurrent.QuadPart - liLast.QuadPart) 
            * 1000000.0) / liFrequency.QuadPart);

	return fPerfElapsed;
}