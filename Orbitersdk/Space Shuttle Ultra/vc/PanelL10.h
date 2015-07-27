/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel L10 definition



  Space Shuttle Ultra is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Space Shuttle Ultra is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Space Shuttle Ultra; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  See http://spaceshuttleultra.sourceforge.net/license/ for more details.

  **************************************************************************/
#ifndef __VC_PANELL10_H
#define __VC_PANELL10_H


#include "AtlantisPanel.h"
#include "StandardSwitch.h"
#include "Talkback.h"


namespace vc
{
	class PanelL10:public AtlantisPanel
	{
		private:
			MESHHANDLE hPanelMesh;
			UINT mesh_index;

			StdSwitch3* pPowerSourceASEBatteriesPri;
			StdSwitch3* pPowerSourceASEBatteriesAlt;
			StdSwitch3* pPowerSourceOrbiterSCConvPwrPri;
			StdSwitch3* pPowerSourceOrbiterSCConvPwrAlt;
			StdSwitch3* pPowerSourceIUSSCBatteryPri;
			StdSwitch3* pPowerSourceIUSSCBatteryAlt;
			StdSwitch3* pPowerSourceIUSBusABatteriesPri;
			StdSwitch3* pPowerSourceIUSBusABatteriesAlt;
			StdSwitch3* pPowerSourceIUSBusBBatteriesPri;
			StdSwitch3* pPowerSourceIUSBusBBatteriesAlt;
			StdSwitch3* pIUSConvPwrPri;
			StdSwitch3* pIUSConvPwrAlt;
			StdSwitch3* pSCSupportSCRegPwrPri;
			StdSwitch3* pSCSupportSCRegPwrAlt;
			StdSwitch3* pSCSupportSCPowerPri;
			StdSwitch3* pSCSupportSCPowerAlt;
			StdSwitch3* pIUSSupportAFeedPri;
			StdSwitch3* pIUSSupportAFeedAlt;
			StdSwitch3* pIUSSupportBFeedPri;
			StdSwitch3* pIUSSupportBFeedAlt;
			StdSwitch3* pTestPri;
			StdSwitch3* pTestAlt;
			StdSwitch3* pIUSSupportHeatersBatteryPri;
			StdSwitch3* pIUSSupportHeatersBatteryAlt;
			StdSwitch3* pIUSSupportHeatersCriticalPri;
			StdSwitch3* pIUSSupportHeatersCriticalAlt;
			StdSwitch3* pIUSSupportWBDIPri;
			StdSwitch3* pIUSSupportWBDIAlt;
			StdSwitch3* pIUSPowerPri;
			StdSwitch3* pIUSPowerAlt;
			StdSwitch3* pNormBusPwrPri;
			StdSwitch3* pNormBusPwrAlt;
			StdSwitch3* pIUSSupportAutoShutdownStandbyModePri;
			StdSwitch3* pIUSSupportAutoShutdownStandbyModeAlt;
			StdSwitch3* pTiltTableActuatorDriveEnablePri1;
			StdSwitch3* pTiltTableActuatorDriveEnableAlt2;
			StdSwitch3* pTiltTableActuatorMotionPri1;
			StdSwitch3* pTiltTableActuatorMotionAlt2;
			StdSwitch3* pPyroBusPri;
			StdSwitch3* pPyroBusAlt;
			StdSwitch3* pUmbilicalsEnaPri;
			StdSwitch3* pUmbilicalsEnaAlt;
			StdSwitch3* pIUSDeploymentEnaPri;
			StdSwitch3* pIUSDeploymentEnaAlt;

			LockableLever2* pPanelMode;
			LockableLever2* pTiltTableActuatorAltDrAct1;
			LockableLever2* pUmbilicalsRelPri;
			LockableLever2* pUmbilicalsRelAlt;
			LockableLever2* pIUSDeploymentDpyPri;
			LockableLever2* pIUSDeploymentDpyAlt;
			LockableLever2* pPowerKill;

			LockableLever3* pCmdPathEnabled;

			StandardTalkback_2* pPowerSourceASEBatteriesPriTB;
			StandardTalkback_2* pPowerSourceASEBatteriesAltTB;
			StandardTalkback_2* pPowerSourceOrbiterSCConvPwrPriTB;
			StandardTalkback_2* pPowerSourceOrbiterSCConvPwrAltTB;
			StandardTalkback_2* pPowerSourceIUSSCBatteryPriTB;
			StandardTalkback_2* pPowerSourceIUSSCBatteryAltTB;
			StandardTalkback_2* pPowerSourceIUSBusABatteriesPriTB;
			StandardTalkback_2* pPowerSourceIUSBusABatteriesAltTB;
			StandardTalkback_2* pPowerSourceIUSBusBBatteriesPriTB;
			StandardTalkback_2* pPowerSourceIUSBusBBatteriesAltTB;
			StandardTalkback_2* pIUSConvPwrPriTB;
			StandardTalkback_2* pIUSConvPwrAltTB;
			StandardTalkback_2* pSCSupportSCRegPwrPriTB;
			StandardTalkback_2* pSCSupportSCRegPwrAltTB;
			StandardTalkback_2* pSCSupportSCPowerPriTB;
			StandardTalkback_2* pSCSupportSCPowerAltTB;
			StandardTalkback_2* pIUSSupportAFeedPriTB;
			StandardTalkback_2* pIUSSupportAFeedAltTB;
			StandardTalkback_2* pIUSSupportBFeedPriTB;
			StandardTalkback_2* pIUSSupportBFeedAltTB;
			StandardTalkback_2* pMatrixStatusPriTB;
			StandardTalkback_2* pMatrixStatusAltTB;
			StandardTalkback_2* pIUSSupportHeatersBatteryPriTB;
			StandardTalkback_2* pIUSSupportHeatersBatteryAltTB;
			StandardTalkback_2* pIUSSupportHeatersCriticalPriTB;
			StandardTalkback_2* pIUSSupportHeatersCriticalAltTB;
			StandardTalkback_2* pIUSSupportWBDIPriTB;
			StandardTalkback_2* pIUSSupportWBDIAltTB;
			StandardTalkback_2* pIUSPowerPriTB;
			StandardTalkback_2* pIUSPowerAltTB;
			StandardTalkback_2* pNormBusPwrPriTB;
			StandardTalkback_2* pNormBusPwrAltTB;
			StandardTalkback_2* pIUSSupportAutoShutdownStandbyModePriTB;
			StandardTalkback_2* pIUSSupportAutoShutdownStandbyModeAltTB;
			StandardTalkback_2* pTiltTableActuatorMotionPri1TB;
			StandardTalkback_2* pTiltTableActuatorMotionAlt2TB;
			StandardTalkback_2* pTiltTableActuatorPositionPri1TB;
			StandardTalkback_2* pTiltTableActuatorPositionAlt2TB;
			StandardTalkback_2* pTiltTableActuatorPriDrAct1EngagedTB;
			StandardTalkback_2* pTiltTableActuatorAltDrAct2EngagedTB;
			StandardTalkback_2* pPyroBusPriTB;
			StandardTalkback_2* pPyroBusAltTB;
			StandardTalkback_2* pUmbilicalsEnaPriTB;
			StandardTalkback_2* pUmbilicalsEnaAltTB;
			StandardTalkback_2* pUmbilicalsRelPriTB;
			StandardTalkback_2* pUmbilicalsRelAltTB;
			StandardTalkback_2* pIUSDeploymentEnaPriTB;
			StandardTalkback_2* pIUSDeploymentEnaAltTB;

		public:
			PanelL10( Atlantis* _sts );
			virtual ~PanelL10();

			virtual void RegisterVC();
			virtual void DefineVC();
			virtual void AddMeshes( const VECTOR3& ofs );
			virtual void SetMeshVisibility( bool visible );
			virtual UINT GetVCMeshIndex() const;
			virtual void DefineVCAnimations( UINT vcidx );
			virtual void Realize();
	};
};

#endif // __VC_PANELL10_H
