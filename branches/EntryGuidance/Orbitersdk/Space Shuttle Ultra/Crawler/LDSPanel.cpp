#include "LDSPanel.h"
#include "Crawler.h"
#include "meshres_crawler_vc.h"
#include <list>

namespace vc
{

	LDS::LDS(Crawler *_v, const std::string &_ident, vc::CRAWLER_CAB _cab)
		:CrawlerPanel(_v ,_ident, _cab)
	{
		//SOMETHING
		oapiWriteLog("LDS: CLEAR TARGET");
		target.clear();
		target = "NONE";
		//target = "LC39A";
	}


	LDS::~LDS()
	{
		// clean up animations
		for(unsigned int i=0;i<vpAnimations.size();i++) delete vpAnimations.at(i);
		for(unsigned int i=0;i<LDSAnimation.size();i++) delete LDSAnimation.at(i);
	}

	void LDS::ShowAlignmentError(double lateral)
	{
		//sprintf(oapiDebugString(),"%lf",lateral);
	}

	double LDS::CalculateDistanceBetweenAttachments()
	{

		VECTOR3 pos, dir, rot;
		VECTOR3 gpos, gattach, horizon_attach;
		VECTOR3 ldist;

		
		std::vector<ATTACHMENTHANDLE> attachments;
		std::vector<VESSEL*> vessels;
		bool found = false;
		OBJHANDLE hv = NULL;
		VESSEL *pv = NULL;
		std::string name;
		name.clear();

		V()->Local2Global(MLP_ATTACH_POS, gattach);
		V()->HorizonRot(MLP_ATTACH_POS, horizon_attach);

		for(DWORD i=0; i<oapiGetVesselCount(); ++i)
		{
			hv = oapiGetVesselByIndex(i);
			pv = oapiGetVesselInterface(hv);

			name = pv->GetClassNameA();
			if(name == "SSU_Pad" || name == "SSU_VAB")
			{
				for(DWORD j=0; j<pv->AttachmentCount(false); j++)
				{
					ATTACHMENTHANDLE attach = pv->GetAttachmentHandle(false,j);
					if(!_strnicmp(pv->GetAttachmentId(attach),"XMLP",4))
					{
						attachments.push_back(attach);
						vessels.push_back(pv);
					}
				}
			}
		}

		//sprintf(oapiDebugString(),"FOUND %lf ATTACHMENTS",attachments.end());

		//ALL XMLP ATTACHMENTS COMPLETED
		//DETERMINING SHORTEST DISTANCE

		std::vector<VESSEL*>::iterator vi= vessels.begin();
		std::vector<VESSEL*>::iterator tvi;
		std::vector<ATTACHMENTHANDLE>::iterator ai= attachments.begin();
		std::vector<ATTACHMENTHANDLE>::iterator tai;
		double distance = -1;
		
		for(vi = vessels.begin(); vi != vessels.end(); vi++)
		{
			(*vi)->GetAttachmentParams((*ai),pos,dir,rot);
			(*vi)->Local2Global(pos,gpos);
			double temp = dist(gpos,gattach);

			if(distance == -1)
			{
				distance = temp;
				tvi = vi;
				tai = ai;
				++ai;
			}

			if(temp<distance)
			{
				distance = temp;
				tvi = vi;
				tai = ai;
				++ai;
			}

		}
		//sprintf(oapiDebugString(),"%lf",distance);

		//FINAL CALCULATION
		(*tvi)->GetAttachmentParams((*tai),pos,dir,rot);
		(*tvi)->Local2Global(pos,gpos);
		VECTOR3 gdist = gpos - gattach;
		MATRIX3 RotMatrix;
		V()->GetRotationMatrix(RotMatrix);
		ldist = tmul(RotMatrix,gdist);

		sprintf(oapiDebugString(),"%lf",-ldist.x);

		if(!attachments.empty())
			return -ldist.x;
		else return 0;


		


		/*
		for(DWORD i=0; i<oapiGetVesselCount(); i++)
		{
			hv = oapiGetVesselByIndex(i);
			pv = oapiGetVesselInterface(hv);
			name = pv->GetClassNameA();
			//oapiWriteLog("PRZED WEJSCIEM W IFA");
			if(target == "LC39A" || target == "LC39B")
			{
				if(name == "SSU_Pad") //match
				{
					oapiWriteLog("FOUND PAD");
					ATTACHMENTHANDLE attach = pv->GetAttachmentHandle(false,0);

					//TESTING ATTACHMENT
					oapiWriteLog("START ATTACHMENT TEST");
					if(_strnicmp(pv->GetAttachmentId(attach),"XMLP",4)) continue;
					oapiWriteLog("ATTACHMENT TEST COMPLETED");

					pv->GetAttachmentParams(attach,pos,dir,rot);
					pv->Local2Global(pos,gpos);

					VECTOR3 gdist = gpos-gattach;
					MATRIX3 RotMatrix;
					V()->GetRotationMatrix(RotMatrix);

					ldist = tmul(RotMatrix,gdist);
					return -ldist.x;
				}	
			}
			else if(target == "VAB_HBAY1")
			{
				if(name == "SSU_VAB") //match
				{
					pv->Local2Global(VAB_HIGHBAY1,gpos);
					VECTOR3 gdist = gpos - gattach;
					MATRIX3 RotMatrix;
					V()->GetRotationMatrix(RotMatrix);
					ldist = tmul(RotMatrix,gdist);
					return -ldist.x;
				}
			}
			else if(target == "VAB_HBAY2")
			{
				if(name == "SSU_VAB")
				{
					pv->Local2Global(VAB_HIGHBAY2,gpos);
					VECTOR3 gdist = gpos - gattach;
					MATRIX3 RotMatrix;
					V()->GetRotationMatrix(RotMatrix);
					ldist = tmul(RotMatrix,gdist);
					return -ldist.x;
				}
			}
		}
		return 0; //NO TARGET OR STH
		*/
	}


	void LDS::OnPostStep(double fSimT, double fDeltaT, double fMJD)
	{
		if(LeftKnobState == ON)
			V()->SetAnimation(LDSBarAnim,CalculateOffset(1,CalculateDistanceBetweenAttachments()));
		if(CenterKnobState == ON)
			V()->SetAnimation(LDSBarAnim,CalculateOffset(0.1,CalculateDistanceBetweenAttachments()));
		if(RightKnobState == ON)
			V()->SetAnimation(LDSBarAnim,CalculateOffset(0.01,CalculateDistanceBetweenAttachments()));
		//sprintf(oapiDebugString(),"%lf",CalculateDistanceBetweenAttachments(MLP_ATTACH_POS));
		
	}

	double LDS::CalculateOffset(double accuracy, double distance)
	{
		double offset;
		offset = ((0.05/accuracy)*distance) + 0.5;
		//sprintf(oapiDebugString(),"%lf",offset);


		if(offset >= 1)
			return 1;
		else if(offset <= 0)
			return 0;
		else return offset;
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


		VECTOR3 LP1,LP2,CP1,CP2,RP1,RP2,AB1,AB2;
		LP1 = TransformVector(_V(-0.448,0.350,-0.073));
		LP2 = TransformVector(_V(-0.404,0.350,-0.047));
		CP1 = TransformVector(_V(-0.463,0.350,-0.047));
		CP2 = TransformVector(_V(-0.421,0.350,-0.022));
		RP1 = TransformVector(_V(-0.478,0.350,-0.021));
		RP2 = TransformVector(_V(-0.434,0.350,0.005));
		AB1 = TransformVector(_V(-0.4746225178,0.4015539587,-0.0527803004));
		AB2 = TransformVector(_V(-0.4253329933,0.4015539587,-0.1361487955));


		//LEFT KNOB ANIMATION
		static UINT LeftKnobGrp[1] = {GRP_LDS_sense_1_knob_VC};
		VECTOR3 LResult = _V(LP2.x-LP1.x,LP2.y-LP1.y,LP2.z-LP1.z);
		VECTOR3 LNormalised = LResult/dist(LP1,LP2);	
		MGROUP_ROTATE* pRotLeftKnob = new MGROUP_ROTATE(vcidx,LeftKnobGrp,1,LP1,LNormalised,120*RAD); //TRANSFORMED VECTOR
		LeftKnobAnim = V()->CreateAnimation(0.0);
		V()->AddAnimationComponent(LeftKnobAnim,0,1,pRotLeftKnob);
		vpAnimations.push_back(pRotLeftKnob);

		//CENTER KNOB ANIMATION
		static UINT CenterKnobGrp[1] = {GRP_LDS_sense_10_knob_VC};
		VECTOR3 CResult = _V(CP2.x-CP1.x,CP2.y-CP1.y,CP2.z-CP1.z);
		VECTOR3 CNormalised = CResult/dist(CP1,CP2);
		MGROUP_ROTATE* pRotCenterKnob = new MGROUP_ROTATE(vcidx,CenterKnobGrp,1,CP1,CNormalised,120*RAD);
		CenterKnobAnim = V()->CreateAnimation(0.0);
		V()->AddAnimationComponent(CenterKnobAnim,0,1,pRotCenterKnob);
		vpAnimations.push_back(pRotCenterKnob);

		//RIGHT KNOB ANIMATION
		static UINT RightKnobGrp[1] = {GRP_LDS_sense_01_knob_VC};
		VECTOR3 RResult = _V(RP2.x-RP1.x,RP2.y-RP1.y,RP2.z-RP1.z);
		VECTOR3 RNormalised = RResult/dist(RP1,RP2);
		MGROUP_ROTATE* pRotRightKnob = new MGROUP_ROTATE(vcidx,RightKnobGrp,1,RP1,RNormalised,120*RAD);
		RightKnobAnim = V()->CreateAnimation(0.0);
		V()->AddAnimationComponent(RightKnobAnim,0,1,pRotRightKnob);
		vpAnimations.push_back(pRotRightKnob);

		//LDS ALIGNMENT BAR ANIMATION
		static UINT LDSBarGrp[1] = {GRP_Alignment_bar_VC};
		VECTOR3 BResult = _V(AB2.x-AB1.x,AB2.y-AB1.y,AB2.z-AB1.z);
		VECTOR3 BNormalised = BResult/dist(AB1,AB2);
		MGROUP_TRANSLATE *pLDSTranslate = new MGROUP_TRANSLATE(vcidx,LDSBarGrp,1,BResult);
		LDSBarAnim = V()->CreateAnimation(0.5);
		V()->AddAnimationComponent(LDSBarAnim,0,1,pLDSTranslate);
		LDSAnimation.push_back(pLDSTranslate);
		
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