#ifndef ISSUMLP_H_C5C7035F_B9D3_4918_A62F_259860813EE7
#define ISSUMLP_H_C5C7035F_B9D3_4918_A62F_259860813EE7
#pragma once

class ISSUMLP
{
public:
	virtual void SignalGSEStart() = 0;
	virtual void OnT0() = 0;
	virtual void TriggerHDP() = 0;
	virtual void TriggerROFIs() = 0;
	virtual void ActivateSSS() = 0;
};

#endif //ISSUMLP_H_C5C7035F_B9D3_4918_A62F_259860813EE7