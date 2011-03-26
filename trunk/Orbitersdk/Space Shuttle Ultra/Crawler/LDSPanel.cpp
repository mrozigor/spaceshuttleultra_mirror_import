#include "LDSPanel.h"
#include "Crawler.h"

namespace vc
{

	LDS::LDS(Crawler *_v, const std::string &_ident, vc::CRAWLER_CAB _cab)
		:CrawlerPanel(_v ,_ident, _cab)
	{
		//SOMETHING
	}


	LDS::~LDS()
	{
		// clean up animations
		for(unsigned int i=0;i<vpAnimations.size();i++) delete vpAnimations.at(i);
	}

	void LDS::ShowAlignmentError(double lateral)
	{
		//sprintf(oapiDebugString(),"%lf",lateral);
	}

	void LDS::CalculateDistanceBetweenAttachments(VECTOR3 MLP_ATTACH_GLOBAL, MATRIX3 RotationMatrix)
	{
		bool found = false;
		OBJHANDLE hv = NULL;
		VESSEL *pv = NULL;
		std::string name;
		name.clear();

		for(DWORD i=0; i<oapiGetVesselCount(); i++)
		{
			hv = oapiGetVesselByIndex(i);
			pv = oapiGetVesselInterface(hv);
			name = pv->GetClassNameA();
			if(name.c_str() == "SSU_Pad")
			{
				oapiWriteLog((char*)name.c_str());
				oapiWriteLog("ZNALAZLEM PAD");
			}
			else
			{
				//COKOLWIEK
			}
		}
	}

	void LDS::RegisterVC()
	{
		BasicPanel::RegisterVC();

		VECTOR3 ofs;
		int aid_ofs;
		if(cabID==FWD)
		{
			ofs = CRAWLER_FWD_VC_OFFSET;
			aid_ofs = 0;
		}
		else 
		{
			ofs = CRAWLER_REAR_VC_OFFSET;
			aid_ofs = AID_REAR_OFFSET;
		}


		oapiVCRegisterArea(AID_LDS_AREA+aid_ofs,PANEL_REDRAW_MOUSE,PANEL_MOUSE_LBDOWN);
		oapiVCSetAreaClickmode_Quadrilateral(AID_LDS_AREA+aid_ofs,
			TransformVector(_V(-0.451, 0.367, -0.091))+ofs,
			TransformVector(_V(-0.507, 0.367, 0.002))+ofs, 
			TransformVector(_V(-0.451, 0.334, -0.091))+ofs,
			TransformVector(_V(-0.507, 0.334, 0.002))+ofs);


	}


	void LDS::DefineVC()
	{
		//STH
		int aid_ofs;
		if(cabID==FWD) aid_ofs = 0;
		else  aid_ofs = AID_REAR_OFFSET;

		AddAIDToMouseEventList(AID_LDS_AREA+aid_ofs);
	}


		

	void LDS::Realize()
	{
		
	}

	
	void LDS::DefineVCAnimations(UINT vcidx)
	{

		
		/*int rot_ofs;
		if(cabID == FWD)
		{
			rot_ofs = 1;
			BasicPanel::DefineVCAnimations(5);//PASS FWD LDS MESH NUMBER
		}

		else if(cabID == REAR)
		{
			rot_ofs = -1;
			BasicPanel::DefineVCAnimations(6); //PASS REAR LDS MESH NUMBER
		}*/
		//BasicPanel::DefineVCAnimations(vcidx);
		
		//BasicPanel::DefineVCAnimations(mshnbr);


		VECTOR3 LP1,LP2,CP1,CP2,RP1,RP2;
		LP1 = TransformVector(_V(-0.448,0.350,-0.073));
		LP2 = TransformVector(_V(-0.404,0.350,-0.047));
		CP1 = TransformVector(_V(-0.463,0.350,-0.047));
		CP2 = TransformVector(_V(-0.421,0.350,-0.022));
		RP1 = TransformVector(_V(-0.478,0.350,-0.021));
		RP2 = TransformVector(_V(-0.434,0.350,0.005));

		//LEFT KNOB ANIMATION
		static UINT LeftKnobGrp[1] = {29};
		VECTOR3 LResult = _V(LP2.x-LP1.x,LP2.y-LP1.y,LP2.z-LP1.z);
		VECTOR3 LNormalised = LResult/dist(LP1,LP2);	
		MGROUP_ROTATE* pRotLeftKnob = new MGROUP_ROTATE(vcidx,LeftKnobGrp,1,LP1,LNormalised,120*RAD); //TRANSFORMED VECTOR
		LeftKnobAnim = V()->CreateAnimation(0.0);
		V()->AddAnimationComponent(LeftKnobAnim,0,1,pRotLeftKnob);
		vpAnimations.push_back(pRotLeftKnob);

		//CENTER KNOB ANIMATION
		static UINT CenterKnobGrp[1] = {28};
		VECTOR3 CResult = _V(CP2.x-CP1.x,CP2.y-CP1.y,CP2.z-CP1.z);
		VECTOR3 CNormalised = CResult/dist(CP1,CP2);
		MGROUP_ROTATE* pRotCenterKnob = new MGROUP_ROTATE(vcidx,CenterKnobGrp,1,CP1,CNormalised,120*RAD);
		CenterKnobAnim = V()->CreateAnimation(0.0);
		V()->AddAnimationComponent(CenterKnobAnim,0,1,pRotCenterKnob);
		vpAnimations.push_back(pRotCenterKnob);

		//RIGHT KNOB ANIMATION
		static UINT RightKnobGrp[1] = {27};
		VECTOR3 RResult = _V(RP2.x-RP1.x,RP2.y-RP1.y,RP2.z-RP1.z);
		VECTOR3 RNormalised = RResult/dist(RP1,RP2);
		MGROUP_ROTATE* pRotRightKnob = new MGROUP_ROTATE(vcidx,RightKnobGrp,1,RP1,RNormalised,120*RAD);
		RightKnobAnim = V()->CreateAnimation(0.0);
		V()->AddAnimationComponent(RightKnobAnim,0,1,pRotRightKnob);
		vpAnimations.push_back(pRotRightKnob);
	}
		

	
	bool LDS::OnVCMouseEvent(int id, int _event, VECTOR3 &p)
	{

		if((id == AID_LDS_AREA) || (id==AID_LDS_AREA+AID_REAR_OFFSET))
		{
			//sprintf(oapiDebugString(),"%f%s%f",p.x," ",p.y);


			if(p.x >= 0 && p.x < 0.26) //LEFT KNOB
			{
				if(p.y <= 0.5) //SET LEFT KNOB TO OFF
				{
					V()->SetAnimation(LeftKnobAnim,OFF);
					LeftKnobState = OFF;
					//sprintf(oapiDebugString(),"%s","LEFT KNOB OFF");
				}
				else //SET KNOB TO ON
				{	
					V()->SetAnimation(LeftKnobAnim,ON);
					LeftKnobState = ON;
					//sprintf(oapiDebugString(),"%s","LEFT KNOB ON");
				}
			}

			if(p.x >= 0.26 && p.x < 0.53) //CENTER KNOB
			{
				if(p.y <= 0.5) //SET CENTER KNOB TO OFF
				{
					V()->SetAnimation(CenterKnobAnim,OFF);
					CenterKnobState = OFF;
				}
				else //SET CENTER KNOB TO ON
				{
					V()->SetAnimation(CenterKnobAnim,ON);
					CenterKnobState = ON;
				}
			}

			if(p.x >= 0.53 && p.x < 1) //RIGHT KNOB
			{
				if(p.y <= 0.5) //SET RIGHT KNOB TO OFF
				{
					V()->SetAnimation(RightKnobAnim,OFF);
					RightKnobState = OFF;
				}
				else //SET RIGHT KNOB TO ON
				{
					V()->SetAnimation(RightKnobAnim,ON);
					RightKnobState = ON;
				}
			}

			return true;
		}
		else return false;
	}

};