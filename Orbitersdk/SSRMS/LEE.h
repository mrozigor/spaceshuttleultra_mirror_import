#ifndef __LEE_H
#define __LEE_H
#pragma once

#include "Latch.h"

class LEE : public LatchSystem
{
public:
	LEE(SSRMS* _ssrms, const std::string& _ident);
	virtual ~LEE();

	virtual void OnPreStep(double SimT, double DeltaT, double MJD);
	virtual bool OnParseLine(const char* line);
	virtual void OnSaveState(FILEHANDLE scn) const;

	virtual void CreateAttachment();
protected:
	virtual void OnAttach();
	virtual void OnDetach();
private:
};

#endif //__LEE_H