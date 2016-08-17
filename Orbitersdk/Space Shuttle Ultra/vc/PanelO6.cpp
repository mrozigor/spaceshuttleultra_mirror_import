#include "../Atlantis.h"
#include "PanelO6.h"
#include "../meshres_vc.h"
#include "../Atlantis_defs.h"

namespace vc {

	PanelO6::PanelO6(Atlantis* _sts) 
		: AtlantisPanel(_sts, "O6") {

		const VECTOR3 SWITCH_VERTICAL = _V(1.0, 0.0, 0.0);

		oapiWriteLog("(PanelO6::PanelO6) Enter constructor.");

		Add(pLGlareShieldFlood	= new StdSwitch3(_sts, "L GLRSHLD FLOOD"));	
		pLGlareShieldFlood->SetLabel(0, "OFF");
		pLGlareShieldFlood->SetLabel(1, "VAR");
		pLGlareShieldFlood->SetLabel(2, "BRIGHT");

		Add(pSTYDoorPosition = new StandardTalkback_2(_sts, "STAR TRACKER -Y DOOR POSITION (tb)", 2));
		Add(pSTZDoorPosition = new StandardTalkback_2(_sts, "STAR TRACKER -Z DOOR POSITION (tb)", 2));

		Add( pSTRKDoorControlSys1Cover = new StandardSwitchCover( _sts, "S TRK DR CNTL SYS1 Cover" ) );
		pSTRKDoorControlSys1Cover->SetMouseRegion( 0, 0.656091f, 0.175973f, 0.700474f, 0.227679f );
		pSTRKDoorControlSys1Cover->SetMouseRegion( 1, 0.656091f, 0.236236f, 0.700474f, 0.251363f );
		pSTRKDoorControlSys1Cover->DefineCoverGroup( GRP_O6SCOVER11_VC );
		pSTRKDoorControlSys1Cover->SetReference( _V( -0.3814, 3.1445, 13.7718 ), -SWITCH_VERTICAL );

		Add( pSTRKDoorControlSys2Cover = new StandardSwitchCover( _sts, "S TRK DR CNTL SYS2 Cover" ) );
		pSTRKDoorControlSys2Cover->SetMouseRegion( 0, 0.724671f, 0.175973f, 0.764189f, 0.227679f );
		pSTRKDoorControlSys2Cover->SetMouseRegion( 1, 0.724671f, 0.236236f, 0.764189f, 0.251363f );
		pSTRKDoorControlSys2Cover->DefineCoverGroup( GRP_O6SCOVER12_VC );
		pSTRKDoorControlSys2Cover->SetReference( _V( -0.3814, 3.1445, 13.7718 ), -SWITCH_VERTICAL );

		Add(pSTRKDoorControlSys1	= new StdSwitch3(_sts, "S TRK DR CNTL SYS1"));		 
		pSTRKDoorControlSys1->SetLabel(0, "CLOSE");
		pSTRKDoorControlSys1->SetLabel(1, "OFF");
		pSTRKDoorControlSys1->SetLabel(2, "OPEN");
		pSTRKDoorControlSys1->DefineSwitchGroup(GRP_O6S2_VC);
		pSTRKDoorControlSys1->SetMouseRegion(0.64f, 0.1767f, 0.7078f, 0.2315f);
		pSTRKDoorControlSys1->SetReference(_V(-0.3818, 3.1533, 13.7537), SWITCH_VERTICAL);
		pSTRKDoorControlSys1->SetInitialAnimState(0.5f);
		
		Add(pSTRKDoorControlSys2	= new StdSwitch3(_sts, "S TRK DR CNTL SYS2"));		 
		pSTRKDoorControlSys2->SetLabel(0, "CLOSE");
		pSTRKDoorControlSys2->SetLabel(1, "OFF");
		pSTRKDoorControlSys2->SetLabel(2, "OPEN");
		pSTRKDoorControlSys2->DefineSwitchGroup(GRP_O6S3_VC);
		pSTRKDoorControlSys2->SetMouseRegion(0.7267f, 0.1767f, 0.7706f, 0.2315f);
		pSTRKDoorControlSys2->SetReference(_V(-0.3818, 3.1533, 13.7537), SWITCH_VERTICAL);
		pSTRKDoorControlSys2->SetInitialAnimState(0.5f);
		
		
		/*		
		StdSwitch2* pSTRKPowerNY;
		StdSwitch2* pSTRKPowerNZ;
		StdSwitch3* pLeftSeatCenterFlood;
		StdSwitch2* pUHFXmitFreq;
		StdSwitch2* pUHFSPLXPowerAmp;
		StdSwitch2* pUHFSquelch;
		StdSwitch3* pAnnunciatorLampTest;
		StdSwitch3* pBusSelectACA1;
		StdSwitch3* pBusSelectACA23;
		StdSwitch3* pBusSelectACA1;
		StdSwitch3* pMTU;
		StdSwitch2* pIDP1;
		StdSwitch2* pIDP2;
		StdSwitch2* pIDP3;
		StdSwitch2* pIDP4;
		StdSwitch2* pMDMPL1;
		StdSwitch2* pMDMPL2;
		StdSwitch2* pMDMPL3;
		StdSwitch2* pMDMFA1;
		StdSwitch2* pMDMFA2;
		StdSwitch2* pMDMFA2;
		StdSwitch2* pMDMFA4;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		StdSwitch2* pMDMFF2;
		*/

		//_V(0.0, 3.099385, 13.96612)

		Add(pGPCPowerCover[0] = new StandardSwitchCover(_sts, "GPC_POWER_1_COVER"));
		pGPCPowerCover[0]->SetMouseRegion(0, 0.4798f, 0.5008f, 0.5473f, 0.5570f);
		pGPCPowerCover[0]->SetMouseRegion(1, 0.483795f, 0.561731f, 0.535098f, 0.576666f);
		pGPCPowerCover[0]->DefineCoverGroup(GRP_O6SCOVER1_VC);
		pGPCPowerCover[0]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);
		
		Add(pGPCPowerCover[1] = new StandardSwitchCover(_sts, "GPC_POWER_2_COVER"));
		pGPCPowerCover[1]->SetMouseRegion(0, 0.5587f, 0.5008f, 0.6032f, 0.5570f);
		pGPCPowerCover[1]->SetMouseRegion(1, 0.551317f, 0.561731f, 0.606547f, 0.576666f);
		pGPCPowerCover[1]->DefineCoverGroup(GRP_O6SCOVER2_VC);
		pGPCPowerCover[1]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);
		
		Add(pGPCPowerCover[2] = new StandardSwitchCover(_sts, "GPC_POWER_3_COVER"));
		pGPCPowerCover[2]->SetMouseRegion(0, 0.6212f, 0.5008f, 0.6657f, 0.5570f);
		pGPCPowerCover[2]->SetMouseRegion(1, 0.619886f, 0.561731f, 0.675091f, 0.576666f);
		pGPCPowerCover[2]->DefineCoverGroup(GRP_O6SCOVER3_VC);
		pGPCPowerCover[2]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);
		
		Add(pGPCPowerCover[3] = new StandardSwitchCover(_sts, "GPC_POWER_4_COVER"));
		pGPCPowerCover[3]->SetMouseRegion(0, 0.6828f, 0.5008f, 0.7375f, 0.5570f);
		pGPCPowerCover[3]->SetMouseRegion(1, 0.688901f, 0.561731f, 0.743673f, 0.576666f);
		pGPCPowerCover[3]->DefineCoverGroup(GRP_O6SCOVER4_VC);
		pGPCPowerCover[3]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);

		Add(pGPCPowerCover[4] = new StandardSwitchCover(_sts, "GPC_POWER_5_COVER"));
		pGPCPowerCover[4]->SetMouseRegion(0, 0.7538f, 0.5008f, 0.8021f, 0.5570f);
		pGPCPowerCover[4]->SetMouseRegion(1, 0.759426f, 0.561731f, 0.811709f, 0.576666f);
		pGPCPowerCover[4]->DefineCoverGroup(GRP_O6SCOVER5_VC);
		pGPCPowerCover[4]->SetReference(_V( -0.4652, 3.0908, 13.9972 ), -SWITCH_VERTICAL);
		/*
		_V(0.0, 3.09556, 13.98215)
		StdSwitch2* pGPC1Pwr;
		StdSwitch2* pGPC2Pwr;
		StdSwitch2* pGPC3Pwr;
		StdSwitch2* pGPC4Pwr;
		StdSwitch2* pGPC5Pwr;
		*/

		Add(pGPC1Pwr = new StdSwitch2(_sts, "GPC POWER 1"));
		pGPC1Pwr->SetLabel(0, "OFF");
		pGPC1Pwr->SetLabel(1, "ON");
		pGPC1Pwr->SetMouseRegion(0.4798f, 0.5008f, 0.5473f, 0.5570f);
		pGPC1Pwr->DefineSwitchGroup(GRP_O6S30_VC);
		pGPC1Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC1Pwr->SetInitialAnimState(0.5f);

		Add(pGPC2Pwr = new StdSwitch2(_sts, "GPC POWER 2"));
		pGPC2Pwr->SetLabel(0, "OFF");
		pGPC2Pwr->SetLabel(1, "ON");
		pGPC2Pwr->SetMouseRegion(0.5587f, 0.5008f, 0.6032f, 0.5570f);
		pGPC2Pwr->DefineSwitchGroup(GRP_O6S31_VC);
		pGPC2Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC2Pwr->SetInitialAnimState(0.5f);

		Add(pGPC3Pwr = new StdSwitch2(_sts, "GPC POWER 3"));
		pGPC3Pwr->SetLabel(0, "OFF");
		pGPC3Pwr->SetLabel(1, "ON");
		pGPC3Pwr->SetMouseRegion(0.6212f, 0.5008f, 0.6657f, 0.5570f);
		pGPC3Pwr->DefineSwitchGroup(GRP_O6S32_VC);
		pGPC3Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC3Pwr->SetInitialAnimState(0.5f);

		Add(pGPC4Pwr = new StdSwitch2(_sts, "GPC POWER 4"));
		pGPC4Pwr->SetLabel(0, "OFF");
		pGPC4Pwr->SetLabel(1, "ON");
		pGPC4Pwr->SetMouseRegion(0.6828f, 0.5008f, 0.7375f, 0.5570f);
		pGPC4Pwr->DefineSwitchGroup(GRP_O6S33_VC);
		pGPC4Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC4Pwr->SetInitialAnimState(0.5f);

		Add(pGPC5Pwr = new StdSwitch2(_sts, "GPC POWER 5"));
		pGPC5Pwr->SetLabel(0, "OFF");
		pGPC5Pwr->SetLabel(1, "ON");
		pGPC5Pwr->SetMouseRegion(0.7538f, 0.5008f, 0.8021f, 0.5570f);
		pGPC5Pwr->DefineSwitchGroup(GRP_O6S34_VC);
		pGPC5Pwr->SetReference(_V(0.0, 3.09556, 13.98215), SWITCH_VERTICAL);
		pGPC5Pwr->SetInitialAnimState(0.5f);

		Add(pGPCOutput[0] = new StandardTalkback_2(_sts, "GPC Output 1 (tb)", 1));
		
		Add(pGPCOutput[1] = new StandardTalkback_2(_sts, "GPC Output 2 (tb)", 1));

		Add(pGPCOutput[2] = new StandardTalkback_2(_sts, "GPC Output 3 (tb)", 1));

		Add(pGPCOutput[3] = new StandardTalkback_2(_sts, "GPC Output 4 (tb)", 1));

		Add(pGPCOutput[4] = new StandardTalkback_2(_sts, "GPC Output 5 (tb)", 1));

		//_V(-0.0, 3.075613, 14.06777)

		Add(pGPCOutputCover[0] = new StandardSwitchCover(_sts, "GPC_OUTPUT_1_COVER"));
		pGPCOutputCover[0]->SetMouseRegion(0, 0.4798f, 0.6438f, 0.5473f, 0.7007f);
		pGPCOutputCover[0]->SetMouseRegion(1, 0.483795f, 0.705872f, 0.535098f, 0.723074f);
		pGPCOutputCover[0]->DefineCoverGroup(GRP_O6SCOVER6_VC);
		pGPCOutputCover[0]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);
		
		Add(pGPCOutputCover[1] = new StandardSwitchCover(_sts, "GPC_OUTPUT_2_COVER"));
		pGPCOutputCover[1]->SetMouseRegion(0, 0.5587f, 0.6438f, 0.6032f, 0.7007f);
		pGPCOutputCover[1]->SetMouseRegion(1, 0.551317f, 0.705872f, 0.606547f, 0.723074f);
		pGPCOutputCover[1]->DefineCoverGroup(GRP_O6SCOVER7_VC);
		pGPCOutputCover[1]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);
		
		Add(pGPCOutputCover[2] = new StandardSwitchCover(_sts, "GPC_OUTPUT_3_COVER"));
		pGPCOutputCover[2]->SetMouseRegion(0, 0.6212f, 0.6438f, 0.6657f, 0.7007f);
		pGPCOutputCover[2]->SetMouseRegion(1, 0.619886f, 0.705872f, 0.675091f, 0.723074f);
		pGPCOutputCover[2]->DefineCoverGroup(GRP_O6SCOVER8_VC);
		pGPCOutputCover[2]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);
		
		Add(pGPCOutputCover[3] = new StandardSwitchCover(_sts, "GPC_OUTPUT_4_COVER"));
		pGPCOutputCover[3]->SetMouseRegion(0, 0.6828f, 0.6438f, 0.7375f, 0.7007f);
		pGPCOutputCover[3]->SetMouseRegion(1, 0.688901f, 0.705872f, 0.743673f, 0.723074f);
		pGPCOutputCover[3]->DefineCoverGroup(GRP_O6SCOVER9_VC);
		pGPCOutputCover[3]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);

		Add(pGPCOutputCover[4] = new StandardSwitchCover(_sts, "GPC_OUTPUT_5_COVER"));
		pGPCOutputCover[4]->SetMouseRegion(0, 0.7538f, 0.6438f, 0.8021f, 0.7007f);
		pGPCOutputCover[4]->SetMouseRegion(1, 0.759426f, 0.705872f, 0.811709f, 0.723074f);
		pGPCOutputCover[4]->DefineCoverGroup(GRP_O6SCOVER10_VC);
		pGPCOutputCover[4]->SetReference(_V( -0.4653, 3.0668, 14.0986 ), -SWITCH_VERTICAL);

		/*
		_V(0.0,  3.07197,  14.08016)
		StdSwitch3* pGPC1Output;
		StdSwitch3* pGPC2Output;
		StdSwitch3* pGPC3Output;
		StdSwitch3* pGPC4Output;
		StdSwitch3* pGPC5Output;
		*/

		Add(pGPC1Output = new StdSwitch3(_sts, "GPC OUTPUT 1"));
		pGPC1Output->SetLabel(0, "TERMINATE");
		pGPC1Output->SetLabel(1, "NORMAL");
		pGPC1Output->SetLabel(2, "BACKUP");
		pGPC1Output->SetMouseRegion(0.4798f, 0.6438f, 0.5473f, 0.7007f);
		pGPC1Output->DefineSwitchGroup(GRP_O6S35_VC);
		pGPC1Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC1Output->SetInitialAnimState(0.5f);

		Add(pGPC2Output = new StdSwitch3(_sts, "GPC OUTPUT 2"));
		pGPC2Output->SetLabel(0, "TERMINATE");
		pGPC2Output->SetLabel(1, "NORMAL");
		pGPC2Output->SetLabel(2, "BACKUP");
		pGPC2Output->SetMouseRegion(0.5587f, 0.6438f, 0.6032f, 0.7007f);
		pGPC2Output->DefineSwitchGroup(GRP_O6S36_VC);
		pGPC2Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC2Output->SetInitialAnimState(0.5f);

		Add(pGPC3Output = new StdSwitch3(_sts, "GPC OUTPUT 3"));
		pGPC3Output->SetLabel(0, "TERMINATE");
		pGPC3Output->SetLabel(1, "NORMAL");
		pGPC3Output->SetLabel(2, "BACKUP");
		pGPC3Output->SetMouseRegion(0.6212f, 0.6438f, 0.6657f, 0.7007f);
		pGPC3Output->DefineSwitchGroup(GRP_O6S37_VC);
		pGPC3Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC3Output->SetInitialAnimState(0.5f);

		Add(pGPC4Output = new StdSwitch3(_sts, "GPC OUTPUT 4"));
		pGPC4Output->SetLabel(0, "TERMINATE");
		pGPC4Output->SetLabel(1, "NORMAL");
		pGPC4Output->SetLabel(2, "BACKUP");
		pGPC4Output->SetMouseRegion(0.6828f, 0.6438f, 0.7375f, 0.7007f);
		pGPC4Output->DefineSwitchGroup(GRP_O6S38_VC);
		pGPC4Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC4Output->SetInitialAnimState(0.5f);

		Add(pGPC5Output = new StdSwitch3(_sts, "GPC OUTPUT 5"));
		pGPC5Output->SetLabel(0, "TERMINATE");
		pGPC5Output->SetLabel(1, "NORMAL");
		pGPC5Output->SetLabel(2, "BACKUP");
		pGPC5Output->SetMouseRegion(0.7538f, 0.6438f, 0.8021f, 0.7007f);
		pGPC5Output->DefineSwitchGroup(GRP_O6S39_VC);
		pGPC5Output->SetReference(_V(0.0,  3.07197,  14.08016), SWITCH_VERTICAL);
		pGPC5Output->SetInitialAnimState(0.5f);


		/*
		_V(0.0, 3.056848, 14.14088)
		StdSwitch3* pIPLSource;
		*/

		Add(pIPLSource = new StdSwitch3(_sts, "IPL SOURCE"));
		pIPLSource->SetLabel(0, "MMU2");
		pIPLSource->SetLabel(1, "OFF");
		pIPLSource->SetLabel(2, "MMU1");
		pIPLSource->SetMouseRegion(0.8518f, 0.7391f, 0.8936f, 0.7720f);
		pIPLSource->DefineSwitchGroup(GRP_O6S45_VC);
		pIPLSource->SetReference(_V(0.0, 3.056848, 14.14088), SWITCH_VERTICAL);
		pIPLSource->SetInitialAnimState( 0.5f );

		Add(pIPL1 = new PushButton(_sts, "IPL1"));
		pIPL1->SetMouseRegion(0.4949f, 0.7365f, 0.5439f, 0.7741f);
		Add(pIPL2 = new PushButton(_sts, "IPL2"));
		pIPL2->SetMouseRegion(0.5599f, 0.7365f, 0.6079f, 0.7741f);
		Add(pIPL3 = new PushButton(_sts, "IPL3"));
		pIPL3->SetMouseRegion(0.6268f, 0.7365f, 0.6727f, 0.7741f);
		Add(pIPL4 = new PushButton(_sts, "IPL4"));
		pIPL4->SetMouseRegion(0.6905f, 0.7365f, 0.7396f, 0.7741f);
		Add(pIPL5 = new PushButton(_sts, "IPL5"));
		pIPL5->SetMouseRegion(0.7554f, 0.7365f, 0.8026f, 0.7741f);

		Add(pGPCMode[0] = new StandardTalkback_2(_sts, "GPC MODE 1 (tb)", 2));

		Add(pGPCMode[1] = new StandardTalkback_2(_sts, "GPC MODE 2 (tb)", 2));

		Add(pGPCMode[2] = new StandardTalkback_2(_sts, "GPC MODE 3 (tb)", 2));

		Add(pGPCMode[3] = new StandardTalkback_2(_sts, "GPC MODE 4 (tb)", 2));

		Add(pGPCMode[4] = new StandardTalkback_2(_sts, "GPC MODE 5 (tb)", 2));

		//0.0,  3.033531,  14.23673
		Add(pGPC1Mode = new LockableLever3(_sts, "GPC MODE 1"));
		pGPC1Mode->SetLabel(0, "HALT");
		pGPC1Mode->SetLabel(1, "STBY");
		pGPC1Mode->SetLabel(2, "RUN");
		pGPC1Mode->SetMouseRegion(0.4798f, 0.8718f, 0.5473f, 0.9117f);
		pGPC1Mode->DefineSwitchGroup(GRP_O6S46_VC);
		pGPC1Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC1Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC1Mode->SetInitialAnimState(0.5f);

		Add(pGPC2Mode = new LockableLever3(_sts, "GPC MODE 2"));
		pGPC2Mode->SetLabel(0, "HALT");
		pGPC2Mode->SetLabel(1, "STBY");
		pGPC2Mode->SetLabel(2, "RUN");
		pGPC2Mode->SetMouseRegion(0.5587f, 0.8718f, 0.6032f, 0.9117f);
		pGPC2Mode->DefineSwitchGroup(GRP_O6S47_VC);
		pGPC2Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC2Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC2Mode->SetInitialAnimState(0.5f);

		Add(pGPC3Mode = new LockableLever3(_sts, "GPC MODE 3"));
		pGPC3Mode->SetLabel(0, "HALT");
		pGPC3Mode->SetLabel(1, "STBY");
		pGPC3Mode->SetLabel(2, "RUN");
		pGPC3Mode->SetMouseRegion(0.6212f, 0.8718f, 0.6657f, 0.9117f);
		pGPC3Mode->DefineSwitchGroup(GRP_O6S48_VC);
		pGPC3Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC3Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC3Mode->SetInitialAnimState(0.5f);

		Add(pGPC4Mode = new LockableLever3(_sts, "GPC MODE 4"));
		pGPC4Mode->SetLabel(0, "HALT");
		pGPC4Mode->SetLabel(1, "STBY");
		pGPC4Mode->SetLabel(2, "RUN");
		pGPC4Mode->SetMouseRegion(0.6828f, 0.8718f, 0.7375f, 0.9117f);
		pGPC4Mode->DefineSwitchGroup(GRP_O6S49_VC);
		pGPC4Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC4Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC4Mode->SetInitialAnimState(0.5f);

		Add(pGPC5Mode = new LockableLever3(_sts, "GPC MODE 5"));
		pGPC5Mode->SetLabel(0, "HALT");
		pGPC5Mode->SetLabel(1, "STBY");
		pGPC5Mode->SetLabel(2, "RUN");
		pGPC5Mode->SetMouseRegion(0.7538f, 0.8718f, 0.8021f, 0.9117f);
		pGPC5Mode->DefineSwitchGroup(GRP_O6S50_VC);
		pGPC5Mode->SetReference(_V(0.0,  3.033531,  14.23673), SWITCH_VERTICAL);
		pGPC5Mode->SetPullDirection( _V( 0.0, -0.971961, -0.235142 ) );
		pGPC5Mode->SetInitialAnimState(0.5f);
		oapiWriteLog("(PanelO6::PanelO6) Leave constructor.");
	}

	PanelO6::~PanelO6() {
	}

	void PanelO6::DefineVC()
	{
		oapiWriteLog("(PanelO6::Define VC)");
		AddAIDToMouseEventList(AID_O6);

		pSTYDoorPosition->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK2_VC );
		
		pSTZDoorPosition->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK1_VC );

		pGPCOutput[0]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK3_VC );
		
		pGPCOutput[1]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK4_VC );
		
		pGPCOutput[2]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK5_VC );
		
		pGPCOutput[3]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK6_VC );
		
		pGPCOutput[4]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK7_VC );

		pGPCMode[0]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK8_VC );
		
		pGPCMode[1]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK9_VC );
		
		pGPCMode[2]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK10_VC );
		
		pGPCMode[3]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK11_VC );
		
		pGPCMode[4]->DefineMeshGroup( STS()->mesh_vc, GRP_O6TALKBACK12_VC );
	}

	void PanelO6::Realize()
	{
	

		//Warning: This is not correct, panel should talk to FMCA
		discsignals::DiscreteBundle* pBundle = 
			STS()->BundleManager()->CreateBundle("FMCA_STARTRACKER", 16);

		pSTRKDoorControlSys1->outputA.Connect(pBundle, 0);
		pSTRKDoorControlSys1->outputB.Connect(pBundle, 1);

		pSTRKDoorControlSys2->outputA.Connect(pBundle, 4);
		pSTRKDoorControlSys2->outputB.Connect(pBundle, 5);

		pSTYDoorPosition->SetInput( 0, pBundle, 6, TB_CL );
		pSTYDoorPosition->SetInput( 1, pBundle, 7, TB_OP );
		pSTZDoorPosition->SetInput( 0, pBundle, 8, TB_CL );
		pSTZDoorPosition->SetInput( 1, pBundle, 9, TB_OP );

		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC1", 16);
		pGPC1Pwr->output.Connect(pBundle, 0);
		pGPC1Output->outputA.Connect(pBundle, 1);
		pGPC1Output->outputB.Connect(pBundle, 2);
		pIPL1->output.Connect(pBundle, 3);
		pGPC1Mode->ConnectPort( 1, pBundle, 4 );
		pGPC1Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[0]->SetInput( 0, pBundle, 6, TB_GRAY );
		pGPCMode[0]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[0]->SetInput( 1, pBundle, 8, TB_IPL );
		
		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC2", 16);
		pGPC2Pwr->output.Connect(pBundle, 0);
		pGPC2Output->outputA.Connect(pBundle, 1);
		pGPC2Output->outputB.Connect(pBundle, 2);
		pIPL2->output.Connect(pBundle, 3);
		pGPC2Mode->ConnectPort( 1, pBundle, 4 );
		pGPC2Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[1]->SetInput( 0, pBundle, 6, TB_GRAY );
		pGPCMode[1]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[1]->SetInput( 1, pBundle, 8, TB_IPL );
		
		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC3", 16);
		pGPC3Pwr->output.Connect(pBundle, 0);
		pGPC3Output->outputA.Connect(pBundle, 1);
		pGPC3Output->outputB.Connect(pBundle, 2);
		pIPL3->output.Connect(pBundle, 3);
		pGPC3Mode->ConnectPort( 1, pBundle, 4 );
		pGPC3Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[2]->SetInput( 0, pBundle, 6, TB_GRAY );
		pGPCMode[2]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[2]->SetInput( 1, pBundle, 8, TB_IPL );

		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC4", 16);
		pGPC4Pwr->output.Connect(pBundle, 0);
		pGPC4Output->outputA.Connect(pBundle, 1);
		pGPC4Output->outputB.Connect(pBundle, 2);
		pIPL4->output.Connect(pBundle, 3);
		pGPC4Mode->ConnectPort( 1, pBundle, 4 );
		pGPC4Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[3]->SetInput( 0, pBundle, 6, TB_GRAY );
		pGPCMode[3]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[3]->SetInput( 1, pBundle, 8, TB_IPL );
		
		pBundle = STS()->BundleManager()->CreateBundle("O6_GPC5", 16);
		pGPC5Pwr->output.Connect(pBundle, 0);
		pGPC5Output->outputA.Connect(pBundle, 1);
		pGPC5Output->outputB.Connect(pBundle, 2);
		pIPL5->output.Connect(pBundle, 3);
		pGPC5Mode->ConnectPort( 1, pBundle, 4 );
		pGPC5Mode->ConnectPort( 2, pBundle, 5 );
		pGPCOutput[4]->SetInput( 0, pBundle, 6, TB_GRAY );
		pGPCMode[4]->SetInput( 0, pBundle, 7, TB_RUN );
		pGPCMode[4]->SetInput( 1, pBundle, 8, TB_IPL );

		pBundle = STS()->BundleManager()->CreateBundle("O6_IPL", 16);
		pIPLSource->outputA.Connect(pBundle, 0);
		pIPLSource->outputB.Connect(pBundle, 1);
		
		AtlantisPanel::Realize();
	}

	void PanelO6::RegisterVC()
	{
		oapiWriteLog("(PanelO6::RegisterVC) Begin registration.");
		AtlantisPanel::RegisterVC();
		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;

		oapiVCRegisterArea(AID_O6, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBUP | PANEL_MOUSE_LBPRESSED );
		oapiVCSetAreaClickmode_Quadrilateral (AID_O6, 
			_V(-0.726224, 3.18582, 13.6082)+ofs, _V(-0.217782, 3.18582, 13.6082)+ofs, 
			_V(-0.726224, 3.01496, 14.3133) + ofs, _V(-0.217782, 3.01496, 14.3133)+ofs);
	}

};
