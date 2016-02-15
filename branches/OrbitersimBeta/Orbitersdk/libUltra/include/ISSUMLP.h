#ifndef ISSUMLP_H_C5C7035F_B9D3_4918_A62F_259860813EE7
#define ISSUMLP_H_C5C7035F_B9D3_4918_A62F_259860813EE7
#pragma once

class ISSUMLP
{
public:
	virtual void GLSAutoSeqStart() = 0; // called at T-31 seconds
	virtual void OnT0() = 0;
	virtual void TriggerROFIs() = 0;
	virtual void ActivateSSS() = 0;
};

#endif //ISSUMLP_H_C5C7035F_B9D3_4918_A62F_259860813EE7