#ifndef ___URWUMPES_AUX_CLASS_STOPWATCH_H___
#define ___URWUMPES_AUX_CLASS_STOPWATCH_H___

#include "windows.h"

class Stopwatch
{
   LARGE_INTEGER liFrequency;
   LARGE_INTEGER liCurrent;
   LARGE_INTEGER liLast;
public:
	Stopwatch();
	~Stopwatch();
	void Start();
	double Stop();
};

#endif

