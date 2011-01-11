#include "OMSBurnSoftware.h"
#include "OrbitDAP.h"
#include "../Atlantis.h"
#include "../ParameterValues.h"
#include <UltraMath.h>
#include "IDP.h"

namespace dps
{

OMSBurnSoftware::OMSBurnSoftware(SimpleGPCSystem* _gpc)
: SimpleGPCSoftware(_gpc, "OMSBurnSoftware"),
BurnInProg(false), BurnCompleted(false),
MnvrLoad(false), MnvrExecute(false), MnvrToBurnAtt(false),
lastUpdateSimTime(-10.0), pOrbitDAP(NULL)
{
	TIG[0]=TIG[1]=TIG[2]=TIG[3]=0.0;
	OMS = 0;
	TV_ROLL=0.0;
	PEG7 = _V(0.0, 0.0, 0.0);
	Trim = _V(0.0, 0.0, 0.0);
}

OMSBurnSoftware::~OMSBurnSoftware()
{
}

void OMSBurnSoftware::Realize()
{
	DiscreteBundle* pBundle = BundleManager()->CreateBundle("LOMS", 5);
	omsEngineCommand[LEFT].Connect(pBundle, 2);
	omsPitchGimbal[LEFT].Connect(pBundle, 3);
	omsPitchCommand[LEFT].Connect(pBundle, 3);
	omsYawGimbal[LEFT].Connect(pBundle, 4);
	omsYawCommand[LEFT].Connect(pBundle, 4);
	pBundle = BundleManager()->CreateBundle("ROMS", 5);
	omsEngineCommand[RIGHT].Connect(pBundle, 2);
	omsPitchGimbal[RIGHT].Connect(pBundle, 3);
	omsPitchCommand[RIGHT].Connect(pBundle, 3);
	omsYawGimbal[RIGHT].Connect(pBundle, 4);
	omsYawCommand[RIGHT].Connect(pBundle, 4);

	omsPitchCommand[LEFT].ResetLine();
	omsYawCommand[LEFT].ResetLine();
	omsPitchCommand[RIGHT].ResetLine();
	omsYawCommand[RIGHT].ResetLine();

	if(MnvrLoad) {
		LoadManeuver();
		//if(MnvrToBurnAtt) STS()->LoadBurnAttManeuver(BurnAtt);
		if(MnvrToBurnAtt) pOrbitDAP->ManeuverToLVLHAttitude(BurnAtt);
	}

	pOrbitDAP = static_cast<OrbitDAP*>(FindSoftware("OrbitDAP"));
}

void OMSBurnSoftware::OnPreStep(double SimT, double DeltaT, double MJD)
{
	if((SimT-lastUpdateSimTime) > 10.0) {
		STS()->GetElements(NULL, el, &oparam, 0, FRAME_EQU);
		lastUpdateSimTime = SimT;
	}
	else {
		oparam.ApT-=DeltaT;
		oparam.PeT-=DeltaT;
	}


	if(BurnCompleted || !MnvrLoad || !MnvrExecute) return; // no burn to perform

	double met = STS()->GetMET();
	sprintf_s(oapiDebugString(), 255, "Maneuver %f %f %f %f", tig, met, BurnTime, IgnitionTime);
	if(BurnInProg)
	{
		if(met>=(IgnitionTime+BurnTime)) {
			//sprintf(oapiDebugString(), "Shutdown");
			//SetThrusterGroupLevel(thg_main, 0.00);
			omsEngineCommand[0].ResetLine();
			omsEngineCommand[1].ResetLine();
			BurnCompleted=true;
			BurnInProg=false;
			pOrbitDAP->UseRCS();
		}
		else {
			VECTOR3 ThrustVector;
			STS()->GetThrustVector(ThrustVector); //inefficient
			//only shows errors caused by burn timing
			VGO.x-=((ThrustVector.z/STS()->GetMass())*DeltaT)*MPS2FPS;
			VGO.y-=((ThrustVector.x/STS()->GetMass())*DeltaT)*MPS2FPS;
			VGO.z+=((ThrustVector.y/STS()->GetMass())*DeltaT)*MPS2FPS;
		}
	}
	else if(met>=tig)
	{
		// start burn
		if(OMS<3) {
			//sprintf(oapiDebugString(), "Burning");
			BurnInProg=true;
			IgnitionTime=met;
			//ignite engines
			//if(OMS==0) SetThrusterGroupLevel(thg_main, 1.00);
			//else SetThrusterLevel(th_oms[OMS-1], 1.00);
			if(OMS==0) {
				omsEngineCommand[0].SetLine();
				omsEngineCommand[1].SetLine();
				pOrbitDAP->UseOMSTVC(OrbitDAP::BOTH_OMS, Trim);
			}
			else if(OMS==1) {
				omsEngineCommand[0].SetLine();
				pOrbitDAP->UseOMSTVC(OrbitDAP::LEFT_OMS, Trim);
			}
			else {
				omsEngineCommand[1].SetLine();
				pOrbitDAP->UseOMSTVC(OrbitDAP::RIGHT_OMS, Trim);
			}
		}
	}
}

bool OMSBurnSoftware::OnMajorModeChange(unsigned int newMajorMode)
{
	if(newMajorMode == 104 || newMajorMode == 105 || newMajorMode == 106 ||
		newMajorMode == 202 ||
		newMajorMode == 301 || newMajorMode == 302 || newMajorMode == 303)
	{
		WT=STS()->GetMass()*kg_to_pounds;
		BurnInProg=false;
		BurnCompleted=false;
		MnvrLoad=false;
		MnvrExecute=false;
		MnvrToBurnAtt=false;
		return true;
	}
	return false;
}

bool OMSBurnSoftware::ItemInput(int spec, int item, const char* Data)
{
	double dNew;
	if(item>=1 && item<=4) {
		OMS=item-1;
		return true;
	}
	if(item==5) {
		dNew=atof(Data);
		if(dNew>=0.0 && dNew<=359.99)
			TV_ROLL=dNew;
		return true;
	}
	else if(item>=6 && item<=8) {
		dNew=atof(Data);
		if(fabs(dNew)<=6.0) 
		{
			Trim.data[item-6]=dNew;
		}
		return true;
	}
	else if(item==9) {
		//dNew=atof(Data);
		WT=atof(Data);
		return true;
	}
	else if(item>=10 && item<=13) {
		if(item==13) dNew=atof(Data);
		else dNew=atoi(Data);
		//sprintf(oapiDebugString(), "%f", dNew);
		if((item==10 && dNew<365.0) || (item==11 && dNew<24.0) || (item>11 && dNew<60.0)) {
			TIG[item-10]=dNew;
		}
		return true;
	}
	else if(item==14 && GetMajorMode()!=202) {
		C1=atof(Data);
		return true;
	}
	else if(item==15 && GetMajorMode()!=202) {
		dNew=atof(Data);
		if(fabs(dNew)<10.0) {
			C2=dNew;
		}
		return true;
	}
	else if(item==16 && GetMajorMode()!=202) {
		HT=atof(Data);
		return true;
	}
	else if(item==17 && GetMajorMode()!=202) {
		ThetaT=atof(Data);
		return true;
	}
	else if(item>=19 && item<=21) {
		dNew=atof(Data);
		PEG7.data[item-19]=dNew;
		return true;
	}
	else if(item==22) {
		LoadManeuver();
		return true;
	}
	else if(item==27) {
		if(!MnvrToBurnAtt) {
			//STS()->LoadBurnAttManeuver(BurnAtt);
			pOrbitDAP->ManeuverToLVLHAttitude(BurnAtt);
		}
		else {
			STS()->TerminateManeuver();
			MnvrToBurnAtt=false;
		}
		return true;
	}

	return false;
}

bool OMSBurnSoftware::ExecPressed(int spec)
{
	if(MnvrLoad && !MnvrExecute && tig-STS()->GetMET()<=15.0) MnvrExecute=true;
	return true;
}

bool OMSBurnSoftware::OnPaint(int spec, vc::MDU* pMDU) const
{
	if(spec!=dps::MODE_UNDEFINED) return false;

	int minutes, seconds;
	int timeDiff;
	int TIMER[4];
	int TGO[2];
	char cbuf[255];

	switch(GetMajorMode()) {
	case 104:
		PrintCommonHeader("OMS 1 MNVR EXEC", pMDU);
		if((oparam.PeT)<(oparam.ApT)) {
			minutes=(int)(oparam.PeT/60);
			seconds=(int)(oparam.PeT-(60*minutes));
			sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
			pMDU->mvprint(20, 9, cbuf);
		}
		else {
			minutes=(int)(oparam.ApT/60);
			seconds=(int)(oparam.ApT-(60*minutes));
			sprintf_s(cbuf, 255, "TTA %.2d:%.2d", minutes, seconds);
			pMDU->mvprint(20, 9, cbuf);
		}
		break;
	case 105:
		PrintCommonHeader("OMS 2 MNVR EXEC", pMDU);
		if((oparam.PeT)<(oparam.ApT)) {
			minutes=(int)(oparam.PeT/60);
			seconds=(int)(oparam.PeT-(60*minutes));
			sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
			pMDU->mvprint(20, 9, cbuf);
		}
		else {
			minutes=(int)(oparam.ApT/60);
			seconds=(int)(oparam.ApT-(60*minutes));
			sprintf_s(cbuf, 255, "TTA %.2d:%.2d", minutes, seconds);
			pMDU->mvprint(20, 9, cbuf);
		}
		break;
	case 106:
		PrintCommonHeader("OMS 2 MNVR COAST", pMDU);
		if((oparam.PeT)<(oparam.ApT)) {
			minutes=(int)(oparam.PeT/60);
			seconds=(int)(oparam.PeT-(60*minutes));
			sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
			pMDU->mvprint(20, 9, cbuf);
		}
		else {
			minutes=(int)(oparam.ApT/60);
			seconds=(int)(oparam.ApT-(60*minutes));
			sprintf_s(cbuf, 255, "TTA %.2d:%.2d", minutes, seconds);
			pMDU->mvprint(20, 9, cbuf);
		}
		break;
	case 202:
		PrintCommonHeader("ORBIT MNVR EXEC", pMDU);
		if((oparam.PeT)<(oparam.ApT)) {
			minutes=(int)(oparam.PeT/60);
			seconds=(int)(oparam.PeT-(60*minutes));
			sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
			pMDU->mvprint(20, 9, cbuf);
		}
		else {
			minutes=(int)(oparam.ApT/60);
			seconds=(int)(oparam.ApT-(60*minutes));
			sprintf_s(cbuf, 255, "TTA %.2d:%.2d", minutes, seconds);
			pMDU->mvprint(20, 9, cbuf);
		}
		break;
	case 301:
		PrintCommonHeader("DEORB MNVR COAST", pMDU);
		if((oparam.PeT)<(oparam.ApT)) { // should show REI
			minutes=(int)(oparam.PeT/60);
			seconds=(int)(oparam.PeT-(60*minutes));
			sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
			pMDU->mvprint(20, 9, cbuf);
		}
		else {
			minutes=(int)(oparam.ApT/60);
			seconds=(int)(oparam.ApT-(60*minutes));
			sprintf_s(cbuf, 255, "TTA %.2d:%.2d", minutes, seconds);
			pMDU->mvprint(20, 9, cbuf);
		}
		break;
	case 302:
		PrintCommonHeader("DEORB MNVR EXEC", pMDU);
		if((oparam.PeT)<(oparam.ApT)) { // should show REI
			minutes=(int)(oparam.PeT/60);
			seconds=(int)(oparam.PeT-(60*minutes));
			sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
			pMDU->mvprint(20, 9, cbuf);
		}
		else {
			minutes=(int)(oparam.ApT/60);
			seconds=(int)(oparam.ApT-(60*minutes));
			sprintf_s(cbuf, 255, "TTA %.2d:%.2d", minutes, seconds);
			pMDU->mvprint(20, 9, cbuf);
		}
		break;
	case 303:
		PrintCommonHeader("DEORB MNVR COAST", pMDU);
		if((oparam.PeT)<(oparam.ApT)) { // should show REI/TFF
			minutes=(int)(oparam.PeT/60);
			seconds=(int)(oparam.PeT-(60*minutes));
			sprintf_s(cbuf, 255, "TTP %.2d:%.2d", minutes, seconds); 
			pMDU->mvprint(20, 9, cbuf);
		}
		else {
			minutes=(int)(oparam.ApT/60);
			seconds=(int)(oparam.ApT-(60*minutes));
			sprintf_s(cbuf, 255, "TTA %.2d:%.2d", minutes, seconds);
			pMDU->mvprint(20, 9, cbuf);
		}
		break;
	}

	timeDiff=static_cast<int>(tig-STS()->GetMET()+1);
	if(true) { //for the moment, timer will always be drawn; this will change next version
		TIMER[0]=timeDiff/86400;
		TIMER[1]=(timeDiff-TIMER[0]*86400)/3600;
		TIMER[2]=(timeDiff-TIMER[0]*86400-TIMER[1]*3600)/60;
		TIMER[3]=timeDiff-TIMER[0]*86400-TIMER[1]*3600-TIMER[2]*60;
		sprintf_s(cbuf, 255, "%.0d/%02d:%02d:%02d", abs(TIMER[0]), abs(TIMER[1]), abs(TIMER[2]), abs(TIMER[3]));
		pMDU->mvprint(38, 1, cbuf);
	}

	pMDU->mvprint(1, 1, "OMS BOTH 1");
	pMDU->mvprint(8, 2, "L 2");
	pMDU->mvprint(8, 3, "R 3");
	pMDU->mvprint(1, 4, "RCS SEL  4");
	pMDU->mvprint(11, OMS+1, "*");

	sprintf_s(cbuf, 255, "5 TV ROLL %d", static_cast<int>(TV_ROLL));
	pMDU->mvprint(1, 5, cbuf);
	pMDU->mvprint(1, 6, "TRIM LOAD");
	sprintf(cbuf, "6 P  %+2.1f", Trim.data[0]);
	pMDU->mvprint(2, 7, cbuf);
	sprintf(cbuf, "7 LY %+2.1f", Trim.data[1]);
	pMDU->mvprint(2, 8, cbuf);
	sprintf(cbuf, "8 RY %+2.1f", Trim.data[2]);
	pMDU->mvprint(2, 9, cbuf);
	sprintf(cbuf, "9 WT %6.0f", WT);
	pMDU->mvprint(1, 10, cbuf);
	pMDU->mvprint(0, 11, "10 TIG");
	sprintf(cbuf, "%03.0f/%02.0f:%02.0f:%04.1f", TIG[0], TIG[1], TIG[2], TIG[3]);
	pMDU->mvprint(3, 12, cbuf);

	pMDU->mvprint(0, 13, "TGT PEG 4");
	pMDU->mvprint(1, 14, "14 C1");
	pMDU->mvprint(1, 15, "15 C2");
	pMDU->mvprint(1, 16, "16 HT");
	pMDU->mvprint(1, 17, "17  T");
	pMDU->mvprint(1, 18, "18 PRPLT");

	pMDU->mvprint(0, 19, "TGT PEG 7");
	pMDU->mvprint(1, 20, "19  VX");
	pMDU->mvprint(1, 21, "20  VY");
	pMDU->mvprint(1, 22, "21  VZ");
	if(PEG7.x!=0.0 || PEG7.y!=0.0 || PEG7.z!=0.0) {
		sprintf(cbuf, "%+7.1f", PEG7.x);
		pMDU->mvprint(8, 20, cbuf);
		sprintf(cbuf, "%+6.1f", PEG7.y);
		pMDU->mvprint(8, 21, cbuf);
		sprintf(cbuf, "%+6.1f", PEG7.z);
		pMDU->mvprint(8, 22, cbuf);
	}

	if(MnvrLoad) {
		pMDU->mvprint(0, 23, "LOAD 22/TIMER 23");
		sprintf(cbuf, "24 R %-3.0f", BurnAtt.data[ROLL]);
		pMDU->mvprint(21, 3, cbuf);
		sprintf(cbuf, "25 P %-3.0f", BurnAtt.data[PITCH]);
		pMDU->mvprint(21, 4, cbuf);
		sprintf(cbuf, "26 Y %-3.0f", BurnAtt.data[YAW]);
		pMDU->mvprint(21, 5, cbuf);
		if(!MnvrExecute && timeDiff<=15.0) pMDU->mvprint(46, 2, "EXEC", dps::DEUATT_FLASHING);
	}
	else {
		pMDU->mvprint(0, 23, "     22/TIMER 23");
		pMDU->mvprint(21, 3, "24 R");
		pMDU->mvprint(21, 4, "25 P");
		pMDU->mvprint(21, 5, "26 Y");
	}

	pMDU->mvprint(20, 2, "BURN ATT");
	if(!MnvrToBurnAtt) pMDU->mvprint(20, 6, "MNVR 27");
	else pMDU->mvprint(20, 6, "MNVR 27*");

	pMDU->mvprint(20, 8, "REI");
	pMDU->mvprint(25, 10, "GMBL");
	pMDU->mvprint(24, 11, "L");
	pMDU->mvprint(30, 11, "R");
	sprintf(cbuf, "P %+02.1f %+02.1f", omsPitchGimbal[LEFT].GetVoltage()*OMS_PITCH_RANGE, omsPitchGimbal[RIGHT].GetVoltage()*OMS_PITCH_RANGE);
	pMDU->mvprint(20, 12, cbuf);
	sprintf(cbuf, "Y %+02.1f %+02.1f", omsYawGimbal[LEFT].GetVoltage()*OMS_YAW_RANGE, omsYawGimbal[RIGHT].GetVoltage()*OMS_YAW_RANGE);
	pMDU->mvprint(20, 13, cbuf);

	pMDU->mvprint(20, 15, "PRI 28   29");
	pMDU->mvprint(20, 16, "SEC 30   31");
	pMDU->mvprint(20, 17, "OFF 32   33");
	pMDU->mvprint(20, 18, "GMBL CK  34");

	if(!BurnInProg && !BurnCompleted) {
		TGO[0]=(int)(BurnTime/60);
		TGO[1]=(int)(BurnTime-(TGO[0]*60));
	}
	else if(!BurnCompleted) {
		double btRemaining=IgnitionTime+BurnTime-STS()->GetMET();
		TGO[0]=(int)btRemaining/60;
		TGO[1]=(int)btRemaining%60;
	}
	else TGO[0]=TGO[1]=0;
	sprintf(cbuf, "VTOT   %6.2f", DeltaVTot);
	pMDU->mvprint(37, 3, cbuf);
	sprintf(cbuf, "TGO %.2d:%.2d", TGO[0], TGO[1]);
	pMDU->mvprint(36, 4, cbuf);
	sprintf(cbuf, "VGO X %+8.2f", VGO.x);
	pMDU->mvprint(36, 6, cbuf);
	sprintf(cbuf, "Y  %+7.2f", VGO.y);
	pMDU->mvprint(40, 7, cbuf);
	sprintf(cbuf, "Z  %+7.2f", VGO.z);
	pMDU->mvprint(40, 8, cbuf);
	pMDU->mvprint(40, 10, "HA     HP");
	pMDU->mvprint(36, 11, "TGT");
	pMDU->mvprint(36, 12, "CUR");

	pMDU->mvprint(35, 15, "35 ABORT TGT");

	return true;
}

bool OMSBurnSoftware::OnParseLine(const char* keyword, const char* value)
{
	if(!_strnicmp(keyword, "PEG7", 4)) {
		sscanf(value, "%lf%lf%lf", &PEG7.x, &PEG7.y, &PEG7.z);
		return true;
	}
	else if(!_strnicmp(keyword, "WT", 2)) {
		sscanf(value, "%lf", &WT);
		return true;
	}
	else if(!_strnicmp(keyword, "TIG", 3)) {
		sscanf(value, "%lf%lf%lf%lf", &TIG[0], &TIG[1], &TIG[2], &TIG[3]);
		return true;
	}
	else if(!_strnicmp(keyword, "TV_ROLL", 7)) {
		sscanf_s(value, "%lf", &TV_ROLL);
		return true;
	}
	else if(!_strnicmp(keyword, "MNVR", 4)) {
		sscanf_s(value, "%d %d", &MnvrLoad, &MnvrToBurnAtt);
		return true;
	}
	return false;
}

void OMSBurnSoftware::OnSaveState(FILEHANDLE scn) const
{
	char cbuf[255];
	oapiWriteScenario_vec(scn, "PEG7", PEG7);
	oapiWriteScenario_float(scn, "WT", WT);
	sprintf_s(cbuf, 255, "%0.0f %0.0f %0.0f %0.1f", TIG[0], TIG[1], TIG[2], TIG[3]);
	oapiWriteScenario_string(scn, "TIG", cbuf);
	oapiWriteScenario_float(scn, "TV_ROLL", TV_ROLL);
	sprintf_s(cbuf, 255, "%d %d", MnvrLoad, MnvrToBurnAtt);
	oapiWriteScenario_string(scn, "MNVR", cbuf);
}

void OMSBurnSoftware::LoadManeuver()
{
	int i;
	double StartWeight, EndWeight, EndWeightLast=0.0, FuelRate, ThrustFactor=1.0;
	//VECTOR3 ThrustVector;
	bool bDone=false;
	MnvrLoad=true;
	tig=TIG[0]*86400+TIG[1]*3600+TIG[2]*60+TIG[3];
	
	//dV
	for(i=0;i<3;i++) {
		DeltaV.data[i]=PEG7.data[i]*fps_to_ms;
	}
	DeltaVTot=length(PEG7);
	double DeltaVTotms=length(DeltaV);
	//GimbalOMS(Trim);

	//OMSTVCControlP.Reset();
	//OMSTVCControlY.Reset();

	VECTOR3 ThrustDir; //direction of net thrust (in Orbiter frame)
	if(OMS==0) {
		omsPitchCommand[0].SetLine(static_cast<float>(Trim.data[0]/OMS_PITCH_RANGE));
		omsPitchCommand[1].SetLine(static_cast<float>(Trim.data[0]/OMS_PITCH_RANGE));
		omsYawCommand[0].SetLine(static_cast<float>(Trim.data[1]/OMS_YAW_RANGE));
		omsYawCommand[1].SetLine(static_cast<float>(Trim.data[2]/OMS_YAW_RANGE));
		VECTOR3 temp1, temp2;
		//temp1 = STS()->GimbalOMS(0, Trim.data[0], Trim.data[1]);
		//temp2 = STS()->GimbalOMS(1, Trim.data[0], Trim.data[2]);
		temp1 = CalcOMSThrustDir(0, Trim.data[0], Trim.data[1]);
		temp2 = CalcOMSThrustDir(1, Trim.data[0], Trim.data[2]);
		ThrustDir=temp1+temp2;
		ThrustFactor=length(ThrustDir); //get thrust produced by engines
		ThrustDir=ThrustDir/ThrustFactor; //normalize vector
	}
	else if(OMS==1 || OMS==2) {
		omsPitchCommand[OMS-1].SetLine(Trim.data[0]);
		omsYawCommand[OMS-1].SetLine(Trim.data[OMS]);
		//ThrustDir = STS()->GimbalOMS(OMS-1, Trim.data[0], Trim.data[OMS]);
		ThrustDir = CalcOMSThrustDir(OMS, Trim.data[0], Trim.data[OMS]);
	}
	else {
		ThrustDir=_V(0.0, 0.0, 1.0); //+X RCS
	}
	ThrustDir=RotateVectorZ(ThrustDir, TV_ROLL);

	sprintf_s(oapiDebugString(), 255, "Thrust Dir: %f %f %f %f", ThrustDir.x, ThrustDir.y, ThrustDir.z, TV_ROLL);
	BurnAtt.data[PITCH]=-asin(ThrustDir.y)*DEG;
	BurnAtt.data[YAW]=-asin(ThrustDir.x)*DEG; //check signs here
	// compensate for roll
	/*if(TV_ROLL!=0.0) {
		double dTemp=BurnAtt.data[PITCH];
		BurnAtt.data[PITCH]-=BurnAtt.data[PITCH]*(1.0-cos(TV_ROLL*RAD))+BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD));
		BurnAtt.data[YAW]+=BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD))-dTemp*(1.0-abs(cos(TV_ROLL*RAD)));
	}*/
	
	//Burn Attitude
	if(DeltaV.x!=0.0 || DeltaV.z!=0.0) {
		if(DeltaV.z<=0) BurnAtt.data[PITCH]+=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
		else BurnAtt.data[PITCH]-=DEG*acos(DeltaV.x/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.z, 2))));
	}
	if(DeltaV.x!=0.0 || DeltaV.y!=0.0) BurnAtt.data[YAW]+=DEG*asin(DeltaV.y/sqrt((pow(DeltaV.x, 2)+pow(DeltaV.y, 2))));
	BurnAtt.data[ROLL]=TV_ROLL;
	/*if(TV_ROLL!=0.0) {
		double dTemp=BurnAtt.data[PITCH];
		BurnAtt.data[PITCH]-=BurnAtt.data[PITCH]*(1.0-cos(TV_ROLL*RAD))+BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD));
		BurnAtt.data[YAW]+=BurnAtt.data[YAW]*(1.0-sin(TV_ROLL*RAD))-dTemp*(1.0-cos(TV_ROLL*RAD));
	}*/

	//use rocket equation (TODO: Check math/formulas here)
	// NOTE: assume vacuum ISP and 1.0 efficiency for tank
	StartWeight=WT/kg_to_pounds;
	EndWeight=StartWeight/exp(DeltaVTotms/ORBITER_OMS_ISP0);
	FuelRate=ORBITER_OMS_THRUST/ORBITER_OMS_ISP0;
	FuelRate*=ThrustFactor; //if two-engine burn, compensate for # of engines; TODO: Make sure this is valid; we're calculating fuel, not thrust (SiameseCat)
	BurnTime=(StartWeight-EndWeight)/FuelRate;
	//TGO[0]=BurnTime/60;
	//TGO[1]=BurnTime-(TGO[0]*60);
	VGO.x=DeltaVTot*ThrustDir.z;
	VGO.y=DeltaVTot*ThrustDir.x;
	VGO.z=-DeltaVTot*ThrustDir.y;
}
};