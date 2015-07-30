/****************************************************************************
  This file is part of Space Shuttle Ultra

  Panel L10 VC implementation



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

#include "PanelL10.h"
#include "../meshres_vc_l10.h"
#include "../meshres_vc_additions.h"
#include "../Atlantis.h"
#include "../Atlantis_defs.h"
#include "../CommonDefs.h"


namespace vc
{
	PanelL10::PanelL10( Atlantis* _sts ):AtlantisPanel( _sts, "L10" )
	{
		hPanelMesh = oapiLoadMeshGlobal( DEFAULT_MESHNAME_PANELL10 );
		mesh_index = MESH_UNDEFINED;

		Add( pPanelMode = new LockableLever2( _sts, "Panel Mode" ) );
		pPanelMode->SetLabel( 0, "Alternate" );
		pPanelMode->SetLabel( 1, "Primary" );

		Add( pPowerSourceASEBatteriesPri = new StdSwitch3( _sts, "Power Source ASE Batteries Pri" ) );
		pPowerSourceASEBatteriesPri->SetLabel( 0, "Off" );
		pPowerSourceASEBatteriesPri->SetLabel( 1, "-" );
		pPowerSourceASEBatteriesPri->SetLabel( 2, "On" );

		Add( pPowerSourceASEBatteriesAlt = new StdSwitch3( _sts, "Power Source ASE Batteries Alt" ) );
		pPowerSourceASEBatteriesAlt->SetLabel( 0, "Off" );
		pPowerSourceASEBatteriesAlt->SetLabel( 1, "-" );
		pPowerSourceASEBatteriesAlt->SetLabel( 2, "On" );

		Add( pPowerSourceOrbiterSCConvPwrPri = new StdSwitch3( _sts, "Power Source Orbiter S/C Conv Pwr Pri" ) );
		pPowerSourceOrbiterSCConvPwrPri->SetLabel( 0, "Off" );
		pPowerSourceOrbiterSCConvPwrPri->SetLabel( 1, "-" );
		pPowerSourceOrbiterSCConvPwrPri->SetLabel( 2, "On" );

		Add( pPowerSourceOrbiterSCConvPwrAlt = new StdSwitch3( _sts, "Power Source Orbiter S/C Conv Pwr Alt" ) );
		pPowerSourceOrbiterSCConvPwrAlt->SetLabel( 0, "Off" );
		pPowerSourceOrbiterSCConvPwrAlt->SetLabel( 1, "-" );
		pPowerSourceOrbiterSCConvPwrAlt->SetLabel( 2, "On" );

		Add( pPowerSourceIUSSCBatteryPri = new StdSwitch3( _sts, "Power Source IUS S/C Battery Pri" ) );
		pPowerSourceIUSSCBatteryPri->SetLabel( 0, "Stage I & II Off" );
		pPowerSourceIUSSCBatteryPri->SetLabel( 1, "-" );
		pPowerSourceIUSSCBatteryPri->SetLabel( 2, "Stage I On" );

		Add( pPowerSourceIUSSCBatteryAlt = new StdSwitch3( _sts, "Power Source IUS S/C Battery Alt" ) );
		pPowerSourceIUSSCBatteryAlt->SetLabel( 0, "Stage I & II Off" );
		pPowerSourceIUSSCBatteryAlt->SetLabel( 1, "-" );
		pPowerSourceIUSSCBatteryAlt->SetLabel( 2, "Stage I On" );

		Add( pPowerSourceIUSBusABatteriesPri = new StdSwitch3( _sts, "Power Source IUS Bus A Batteries Pri" ) );
		pPowerSourceIUSBusABatteriesPri->SetLabel( 0, "Stage I & II Off" );
		pPowerSourceIUSBusABatteriesPri->SetLabel( 1, "-" );
		pPowerSourceIUSBusABatteriesPri->SetLabel( 2, "Stage I On" );

		Add( pPowerSourceIUSBusABatteriesAlt = new StdSwitch3( _sts, "Power Source IUS Bus A Batteries Alt" ) );
		pPowerSourceIUSBusABatteriesAlt->SetLabel( 0, "Stage I & II Off" );
		pPowerSourceIUSBusABatteriesAlt->SetLabel( 1, "-" );
		pPowerSourceIUSBusABatteriesAlt->SetLabel( 2, "Stage I On" );

		Add( pPowerSourceIUSBusBBatteriesPri = new StdSwitch3( _sts, "Power Source IUS Bus B Batteries Pri" ) );
		pPowerSourceIUSBusBBatteriesPri->SetLabel( 0, "Stage I & II Off" );
		pPowerSourceIUSBusBBatteriesPri->SetLabel( 1, "-" );
		pPowerSourceIUSBusBBatteriesPri->SetLabel( 2, "Stage I On" );

		Add( pPowerSourceIUSBusBBatteriesAlt = new StdSwitch3( _sts, "Power Source IUS Bus B Batteries Alt" ) );
		pPowerSourceIUSBusBBatteriesAlt->SetLabel( 0, "Stage I & II Off" );
		pPowerSourceIUSBusBBatteriesAlt->SetLabel( 1, "-" );
		pPowerSourceIUSBusBBatteriesAlt->SetLabel( 2, "Stage I On" );

		Add( pIUSConvPwrPri = new StdSwitch3( _sts, "IUS Conv Pwr Pri" ) );
		pIUSConvPwrPri->SetLabel( 0, "Off" );
		pIUSConvPwrPri->SetLabel( 1, "-" );
		pIUSConvPwrPri->SetLabel( 2, "On" );

		Add( pIUSConvPwrAlt = new StdSwitch3( _sts, "IUS Conv Pwr Alt" ) );
		pIUSConvPwrAlt->SetLabel( 0, "Off" );
		pIUSConvPwrAlt->SetLabel( 1, "-" );
		pIUSConvPwrAlt->SetLabel( 2, "On" );
		
		Add( pSCSupportSCRegPwrPri = new StdSwitch3( _sts, "S/C Support S/C Reg Pwr Pri" ) );
		pSCSupportSCRegPwrPri->SetLabel( 0, "Off" );
		pSCSupportSCRegPwrPri->SetLabel( 1, "-" );
		pSCSupportSCRegPwrPri->SetLabel( 2, "On" );

		Add( pSCSupportSCRegPwrAlt = new StdSwitch3( _sts, "S/C Support S/C Reg Pwr Alt" ) );
		pSCSupportSCRegPwrAlt->SetLabel( 0, "Off" );
		pSCSupportSCRegPwrAlt->SetLabel( 1, "-" );
		pSCSupportSCRegPwrAlt->SetLabel( 2, "On" );
		
		Add( pSCSupportSCPowerPri = new StdSwitch3( _sts, "S/C Support S/C Power Pri" ) );
		pSCSupportSCPowerPri->SetLabel( 0, "Off" );
		pSCSupportSCPowerPri->SetLabel( 1, "-" );
		pSCSupportSCPowerPri->SetLabel( 2, "On" );

		Add( pSCSupportSCPowerAlt = new StdSwitch3( _sts, "S/C Support S/C Power Alt" ) );
		pSCSupportSCPowerAlt->SetLabel( 0, "Off" );
		pSCSupportSCPowerAlt->SetLabel( 1, "-" );
		pSCSupportSCPowerAlt->SetLabel( 2, "On" );
		
		Add( pIUSSupportAFeedPri = new StdSwitch3( _sts, "IUS Support A Feed Pri" ) );
		pIUSSupportAFeedPri->SetLabel( 0, "Off" );
		pIUSSupportAFeedPri->SetLabel( 1, "-" );
		pIUSSupportAFeedPri->SetLabel( 2, "On" );

		Add( pIUSSupportAFeedAlt = new StdSwitch3( _sts, "IUS Support A Feed Alt" ) );
		pIUSSupportAFeedAlt->SetLabel( 0, "Off" );
		pIUSSupportAFeedAlt->SetLabel( 1, "-" );
		pIUSSupportAFeedAlt->SetLabel( 2, "On" );
		
		Add( pIUSSupportBFeedPri = new StdSwitch3( _sts, "IUS Support B Feed Pri" ) );
		pIUSSupportBFeedPri->SetLabel( 0, "Off" );
		pIUSSupportBFeedPri->SetLabel( 1, "-" );
		pIUSSupportBFeedPri->SetLabel( 2, "On" );

		Add( pIUSSupportBFeedAlt = new StdSwitch3( _sts, "IUS Support B Feed Alt" ) );
		pIUSSupportBFeedAlt->SetLabel( 0, "Off" );
		pIUSSupportBFeedAlt->SetLabel( 1, "-" );
		pIUSSupportBFeedAlt->SetLabel( 2, "On" );
		
		Add( pTestPri = new StdSwitch3( _sts, "Test Pri" ) );
		pTestPri->SetLabel( 0, "Barber Pole" );
		pTestPri->SetLabel( 1, "-" );
		pTestPri->SetLabel( 2, "Gray" );

		Add( pTestAlt = new StdSwitch3( _sts, "Test Alt" ) );
		pTestAlt->SetLabel( 0, "Barber Pole" );
		pTestAlt->SetLabel( 1, "-" );
		pTestAlt->SetLabel( 2, "Gray" );
		
		Add( pIUSSupportHeatersBatteryPri = new StdSwitch3( _sts, "IUS Support Heaters Battery Pri" ) );
		pIUSSupportHeatersBatteryPri->SetLabel( 0, "Off" );
		pIUSSupportHeatersBatteryPri->SetLabel( 1, "-" );
		pIUSSupportHeatersBatteryPri->SetLabel( 2, "On" );

		Add( pIUSSupportHeatersBatteryAlt = new StdSwitch3( _sts, "IUS Support Heaters Battery Alt" ) );
		pIUSSupportHeatersBatteryAlt->SetLabel( 0, "Off" );
		pIUSSupportHeatersBatteryAlt->SetLabel( 1, "-" );
		pIUSSupportHeatersBatteryAlt->SetLabel( 2, "On" );
		
		Add( pIUSSupportHeatersCriticalPri = new StdSwitch3( _sts, "IUS Support Heaters Critical Pri" ) );
		pIUSSupportHeatersCriticalPri->SetLabel( 0, "Off" );
		pIUSSupportHeatersCriticalPri->SetLabel( 1, "-" );
		pIUSSupportHeatersCriticalPri->SetLabel( 2, "On" );

		Add( pIUSSupportHeatersCriticalAlt = new StdSwitch3( _sts, "IUS Support Heaters Critical Alt" ) );
		pIUSSupportHeatersCriticalAlt->SetLabel( 0, "Off" );
		pIUSSupportHeatersCriticalAlt->SetLabel( 1, "-" );
		pIUSSupportHeatersCriticalAlt->SetLabel( 2, "On" );
		
		Add( pIUSSupportWBDIPri = new StdSwitch3( _sts, "IUS Support WBDI Pri" ) );
		pIUSSupportWBDIPri->SetLabel( 0, "Off" );
		pIUSSupportWBDIPri->SetLabel( 1, "-" );
		pIUSSupportWBDIPri->SetLabel( 2, "On" );

		Add( pIUSSupportWBDIAlt = new StdSwitch3( _sts, "IUS Support WBDI Alt" ) );
		pIUSSupportWBDIAlt->SetLabel( 0, "Off" );
		pIUSSupportWBDIAlt->SetLabel( 1, "-" );
		pIUSSupportWBDIAlt->SetLabel( 2, "On" );
		
		Add( pIUSPowerPri = new StdSwitch3( _sts, "IUS Power Pri" ) );
		pIUSPowerPri->SetLabel( 0, "Off" );
		pIUSPowerPri->SetLabel( 1, "-" );
		pIUSPowerPri->SetLabel( 2, "On" );

		Add( pIUSPowerAlt = new StdSwitch3( _sts, "IUS Power Alt" ) );
		pIUSPowerAlt->SetLabel( 0, "Off" );
		pIUSPowerAlt->SetLabel( 1, "-" );
		pIUSPowerAlt->SetLabel( 2, "On" );
		
		Add( pNormBusPwrPri = new StdSwitch3( _sts, "Norm Bus Pwr Pri" ) );
		pNormBusPwrPri->SetLabel( 0, "Off" );
		pNormBusPwrPri->SetLabel( 1, "-" );
		pNormBusPwrPri->SetLabel( 2, "On" );

		Add( pNormBusPwrAlt = new StdSwitch3( _sts, "Norm Bus Pwr Alt" ) );
		pNormBusPwrAlt->SetLabel( 0, "Off" );
		pNormBusPwrAlt->SetLabel( 1, "-" );
		pNormBusPwrAlt->SetLabel( 2, "On" );
		
		Add( pIUSSupportAutoShutdownStandbyModePri = new StdSwitch3( _sts, "IUS Support Auto Shutdown Standby Mode Pri" ) );
		pIUSSupportAutoShutdownStandbyModePri->SetLabel( 0, "Off" );
		pIUSSupportAutoShutdownStandbyModePri->SetLabel( 1, "-" );
		pIUSSupportAutoShutdownStandbyModePri->SetLabel( 2, "Enable" );

		Add( pIUSSupportAutoShutdownStandbyModeAlt = new StdSwitch3( _sts, "IUS Support Auto Shutdown Standby Mode Alt" ) );
		pIUSSupportAutoShutdownStandbyModeAlt->SetLabel( 0, "Off" );
		pIUSSupportAutoShutdownStandbyModeAlt->SetLabel( 1, "-" );
		pIUSSupportAutoShutdownStandbyModeAlt->SetLabel( 2, "Enable" );
		
		Add( pTiltTableActuatorDriveEnablePri1 = new StdSwitch3( _sts, "Tilt Table Actuator Drive Enable Pri 1" ) );
		pTiltTableActuatorDriveEnablePri1->SetLabel( 0, "Intermediate" );
		pTiltTableActuatorDriveEnablePri1->SetLabel( 1, "Off" );
		pTiltTableActuatorDriveEnablePri1->SetLabel( 2, "Maximum" );

		Add( pTiltTableActuatorDriveEnableAlt2 = new StdSwitch3( _sts, "Tilt Table Actuator Drive Enable Alt 2" ) );
		pTiltTableActuatorDriveEnableAlt2->SetLabel( 0, "Intermediate" );
		pTiltTableActuatorDriveEnableAlt2->SetLabel( 1, "Off" );
		pTiltTableActuatorDriveEnableAlt2->SetLabel( 2, "Maximum" );
		
		Add( pTiltTableActuatorMotionPri1 = new StdSwitch3( _sts, "Tilt Table Actuator Motion Pri 1" ) );
		pTiltTableActuatorMotionPri1->SetLabel( 0, "Lower" );
		pTiltTableActuatorMotionPri1->SetLabel( 1, "-" );
		pTiltTableActuatorMotionPri1->SetLabel( 2, "Raise" );

		Add( pTiltTableActuatorMotionAlt2 = new StdSwitch3( _sts, "Tilt Table Actuator Motion Alt 2" ) );
		pTiltTableActuatorMotionAlt2->SetLabel( 0, "Lower" );
		pTiltTableActuatorMotionAlt2->SetLabel( 1, "-" );
		pTiltTableActuatorMotionAlt2->SetLabel( 2, "Raise" );
		
		Add( pTiltTableActuatorAltDrAct1 = new LockableLever2( _sts, "Tilt Table Actuator Alt Dr Act 1" ) );
		pTiltTableActuatorAltDrAct1->SetLabel( 0, "Off" );
		pTiltTableActuatorAltDrAct1->SetLabel( 1, "Disengage" );
		
		Add( pCmdPathEnabled = new LockableLever3( _sts, "Cmd Path Enabled" ) );
		pCmdPathEnabled->SetLabel( 0, "Hardline" );
		pCmdPathEnabled->SetLabel( 1, "RF" );
		pCmdPathEnabled->SetLabel( 2, "Hardline" );
		
		Add( pPyroBusPri = new StdSwitch3( _sts, "Pyro Bus Pri" ) );
		pPyroBusPri->SetLabel( 0, "Off" );
		pPyroBusPri->SetLabel( 1, "-" );
		pPyroBusPri->SetLabel( 2, "On" );

		Add( pPyroBusAlt = new StdSwitch3( _sts, "Pyro Bus Alt" ) );
		pPyroBusAlt->SetLabel( 0, "Off" );
		pPyroBusAlt->SetLabel( 1, "-" );
		pPyroBusAlt->SetLabel( 2, "On" );
		
		Add( pUmbilicalsEnaPri = new StdSwitch3( _sts, "Umbilicals Ena Pri" ) );
		pUmbilicalsEnaPri->SetLabel( 0, "Off" );
		pUmbilicalsEnaPri->SetLabel( 1, "-" );
		pUmbilicalsEnaPri->SetLabel( 2, "Enable" );

		Add( pUmbilicalsEnaAlt = new StdSwitch3( _sts, "Umbilicals Ena Alt" ) );
		pUmbilicalsEnaAlt->SetLabel( 0, "Off" );
		pUmbilicalsEnaAlt->SetLabel( 1, "-" );
		pUmbilicalsEnaAlt->SetLabel( 2, "Enable" );
		
		Add( pUmbilicalsRelPri = new LockableLever2( _sts, "Umbilicals Rel Pri" ) );
		pUmbilicalsRelPri->SetLabel( 0, "Off" );
		pUmbilicalsRelPri->SetLabel( 1, "Release" );

		Add( pUmbilicalsRelAlt = new LockableLever2( _sts, "Umbilicals Rel Alt" ) );
		pUmbilicalsRelAlt->SetLabel( 0, "Off" );
		pUmbilicalsRelAlt->SetLabel( 1, "Release" );
		
		Add( pIUSDeploymentEnaPri = new StdSwitch3( _sts, "IUS Deployment Ena Pri" ) );
		pIUSDeploymentEnaPri->SetLabel( 0, "Off" );
		pIUSDeploymentEnaPri->SetLabel( 1, "-" );
		pIUSDeploymentEnaPri->SetLabel( 2, "Enable" );

		Add( pIUSDeploymentEnaAlt = new StdSwitch3( _sts, "IUS Deployment Ena Alt" ) );
		pIUSDeploymentEnaAlt->SetLabel( 0, "Off" );
		pIUSDeploymentEnaAlt->SetLabel( 1, "-" );
		pIUSDeploymentEnaAlt->SetLabel( 2, "Enable" );
		
		Add( pIUSDeploymentDpyPri = new LockableLever2( _sts, "IUS Deployment Dpy Pri" ) );
		pIUSDeploymentDpyPri->SetLabel( 0, "Off" );
		pIUSDeploymentDpyPri->SetLabel( 1, "Deploy" );

		Add( pIUSDeploymentDpyAlt = new LockableLever2( _sts, "IUS Deployment Dpy Alt" ) );
		pIUSDeploymentDpyAlt->SetLabel( 0, "Off" );
		pIUSDeploymentDpyAlt->SetLabel( 1, "Deploy" );
		
		Add( pPowerKill = new LockableLever2( _sts, "Power Kill" ) );
		pPowerKill->SetLabel( 0, "Off" );
		pPowerKill->SetLabel( 1, "Kill" );

		Add( pPowerSourceASEBatteriesPriTB = new StandardTalkback_2( _sts, "Power Source ASE Batteries Pri TB", 1 ) );
		Add( pPowerSourceASEBatteriesAltTB = new StandardTalkback_2( _sts, "Power Source ASE Batteries Alt TB", 1 ) );
		Add( pPowerSourceOrbiterSCConvPwrPriTB = new StandardTalkback_2( _sts, "Power Source Orbiter S/C Conv Pwr Pri TB", 1 ) );
		Add( pPowerSourceOrbiterSCConvPwrAltTB = new StandardTalkback_2( _sts, "Power Source Orbiter S/C Conv Pwr Alt TB", 1 ) );
		Add( pPowerSourceIUSSCBatteryPriTB = new StandardTalkback_2( _sts, "Power Source IUS S/C Battery Pri TB", 1 ) );
		Add( pPowerSourceIUSSCBatteryAltTB = new StandardTalkback_2( _sts, "Power Source IUS S/C Battery Alt TB", 1 ) );
		Add( pPowerSourceIUSBusABatteriesPriTB = new StandardTalkback_2( _sts, "Power Source IUS Bus A Batteries Pri TB", 1 ) );
		Add( pPowerSourceIUSBusABatteriesAltTB = new StandardTalkback_2( _sts, "Power Source IUS Bus A Batteries Alt TB", 1 ) );
		Add( pPowerSourceIUSBusBBatteriesPriTB = new StandardTalkback_2( _sts, "Power Source IUS Bus B Batteries Pri TB", 1 ) );
		Add( pPowerSourceIUSBusBBatteriesAltTB = new StandardTalkback_2( _sts, "Power Source IUS Bus B Batteries Alt TB", 1 ) );
		Add( pIUSConvPwrPriTB = new StandardTalkback_2( _sts, "IUS Conv Pwr Pri TB", 1 ) );
		Add( pIUSConvPwrAltTB = new StandardTalkback_2( _sts, "IUS Conv Pwr Alt TB", 1 ) );
		Add( pSCSupportSCRegPwrPriTB = new StandardTalkback_2( _sts, "S/C Support S/C Reg Pwr Pri TB", 1 ) );
		Add( pSCSupportSCRegPwrAltTB = new StandardTalkback_2( _sts, "S/C Support S/C Reg Pwr Alt TB", 1 ) );
		Add( pSCSupportSCPowerPriTB = new StandardTalkback_2( _sts, "S/C Support S/C Power Pri TB", 1 ) );
		Add( pSCSupportSCPowerAltTB = new StandardTalkback_2( _sts, "S/C Support S/C Power Alt TB", 1 ) );
		Add( pIUSSupportAFeedPriTB = new StandardTalkback_2( _sts, "IUS Support A Feed Pri TB", 1 ) );
		Add( pIUSSupportAFeedAltTB = new StandardTalkback_2( _sts, "IUS Support A Feed Alt TB", 1 ) );
		Add( pIUSSupportBFeedPriTB = new StandardTalkback_2( _sts, "IUS Support B Feed Pri TB", 1 ) );
		Add( pIUSSupportBFeedAltTB = new StandardTalkback_2( _sts, "IUS Support B Feed Alt TB", 1 ) );
		Add( pMatrixStatusPriTB = new StandardTalkback_2( _sts, "Matrix Status Pri TB", 1 ) );
		Add( pMatrixStatusAltTB = new StandardTalkback_2( _sts, "Matrix Status Alt TB", 1 ) );
		Add( pIUSSupportHeatersBatteryPriTB = new StandardTalkback_2( _sts, "IUS Support Heaters Battery Pri TB", 1 ) );
		Add( pIUSSupportHeatersBatteryAltTB = new StandardTalkback_2( _sts, "IUS Support Heaters Battery Alt TB", 1 ) );
		Add( pIUSSupportHeatersCriticalPriTB = new StandardTalkback_2( _sts, "IUS Support Heaters Critical Pri TB", 1 ) );
		Add( pIUSSupportHeatersCriticalAltTB = new StandardTalkback_2( _sts, "IUS Support Heaters Critical Alt TB", 1 ) );
		Add( pIUSSupportWBDIPriTB = new StandardTalkback_2( _sts, "IUS Support WBDI Pri TB", 1 ) );
		Add( pIUSSupportWBDIAltTB = new StandardTalkback_2( _sts, "IUS Support WBDI Alt TB", 1 ) );
		Add( pIUSPowerPriTB = new StandardTalkback_2( _sts, "IUS Power Pri TB", 1 ) );
		Add( pIUSPowerAltTB = new StandardTalkback_2( _sts, "IUS Power Alt TB", 1 ) );
		Add( pNormBusPwrPriTB = new StandardTalkback_2( _sts, "Norm Bus Pwr Pri TB", 1 ) );
		Add( pNormBusPwrAltTB = new StandardTalkback_2( _sts, "Norm Bus Pwr Alt TB", 1 ) );
		Add( pIUSSupportAutoShutdownStandbyModePriTB = new StandardTalkback_2( _sts, "IUS Support Auto Shutdown Standby Mode Pri TB", 1 ) );
		Add( pIUSSupportAutoShutdownStandbyModeAltTB = new StandardTalkback_2( _sts, "IUS Support Auto Shutdown Standby Mode Alt TB", 1 ) );
		Add( pTiltTableActuatorMotionPri1TB = new StandardTalkback_2( _sts, "Tilt Table Actuator Motion Pri 1 TB", 1 ) );
		Add( pTiltTableActuatorMotionAlt2TB = new StandardTalkback_2( _sts, "Tilt Table Actuator Motion Alt 2 TB", 1 ) );
		Add( pTiltTableActuatorPositionPri1TB = new StandardTalkback_2( _sts, "Tilt Table Actuator Position Pri 1 TB", 1 ) );
		Add( pTiltTableActuatorPositionAlt2TB = new StandardTalkback_2( _sts, "Tilt Table Actuator Position Alt 2 TB", 1 ) );
		Add( pTiltTableActuatorPriDrAct1EngagedTB = new StandardTalkback_2( _sts, "Tilt Table Actuator Pri Dr Act 1 Engaged TB", 1 ) );
		Add( pTiltTableActuatorAltDrAct2EngagedTB = new StandardTalkback_2( _sts, "Tilt Table Actuator Alt Dr Act 2 Engaged TB", 1 ) );
		Add( pPyroBusPriTB = new StandardTalkback_2( _sts, "Pyro Bus Pri TB", 1 ) );
		Add( pPyroBusAltTB = new StandardTalkback_2( _sts, "Pyro Bus Alt TB", 1 ) );
		Add( pUmbilicalsEnaPriTB = new StandardTalkback_2( _sts, "Umbilicals Ena Pri TB", 1 ) );
		Add( pUmbilicalsEnaAltTB = new StandardTalkback_2( _sts, "Umbilicals Ena Alt TB", 1 ) );
		Add( pUmbilicalsRelPriTB = new StandardTalkback_2( _sts, "Umbilicals Rel Pri TB", 1 ) );
		Add( pUmbilicalsRelAltTB = new StandardTalkback_2( _sts, "Umbilicals Rel Alt TB", 1 ) );
		Add( pIUSDeploymentEnaPriTB = new StandardTalkback_2( _sts, "IUS Deployment Ena Pri TB", 1 ) );
		Add( pIUSDeploymentEnaAltTB = new StandardTalkback_2( _sts, "IUS Deployment Ena Alt TB", 1 ) );
		return;
	}

	PanelL10::~PanelL10()
	{
		return;
	}

	void PanelL10::RegisterVC()
	{
		AtlantisPanel::RegisterVC();

		VECTOR3 ofs = STS()->GetOrbiterCoGOffset() + VC_OFFSET;
		oapiVCRegisterArea( AID_L10, PANEL_REDRAW_NEVER, PANEL_MOUSE_LBDOWN | PANEL_MOUSE_LBPRESSED | PANEL_MOUSE_LBUP );
		oapiVCSetAreaClickmode_Quadrilateral( AID_L10,
			_V( -1.5437, 2.4325, 13.0847 ) + ofs, _V( -1.5437, 2.4325, 13.6310 ) + ofs, 
			_V( -1.0516, 2.1129, 13.0847 ) + ofs, _V( -1.0516, 2.1129, 13.6310 ) + ofs );
		return;
	}

	void PanelL10::DefineVC()
	{
		const VECTOR3 SWITCH_ROT = _V( 0, 0, 1 );
		const VECTOR3 SWITCH_PULL = _V( 0.544639, 0.838671, 0 );

		AddAIDToMouseEventList( AID_L10 );

		pPanelMode->SetInitialAnimState( 0.5f );
		pPanelMode->DefineSwitchGroup( GRP_L10TOGGLE1_VC );
		pPanelMode->SetReference( _V( -1.4702, 2.3884, 13.5557 ), SWITCH_ROT );
		pPanelMode->SetMouseRegion( 0.852753f, 0.124612f, 0.882422f, 0.166456f );
		pPanelMode->SetPullDirection( SWITCH_PULL );
		pPanelMode->SetInitialPosition( 2 );

		pPowerSourceASEBatteriesPri->SetInitialAnimState( 0.5f );
		pPowerSourceASEBatteriesPri->DefineSwitchGroup( GRP_L10SWITCH1_VC );
		pPowerSourceASEBatteriesPri->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceASEBatteriesPri->SetMouseRegion( 0.113853f, 0.180123f, 0.147586f, 0.219617f );
		pPowerSourceASEBatteriesPri->SetSpringLoaded( true, 0 );
		pPowerSourceASEBatteriesPri->SetSpringLoaded( true, 2 );

		pPowerSourceASEBatteriesAlt->SetInitialAnimState( 0.5f );
		pPowerSourceASEBatteriesAlt->DefineSwitchGroup( GRP_L10SWITCH2_VC );
		pPowerSourceASEBatteriesAlt->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceASEBatteriesAlt->SetMouseRegion( 0.167758f, 0.180064f, 0.205525f, 0.220610f );
		pPowerSourceASEBatteriesAlt->SetSpringLoaded( true, 0 );
		pPowerSourceASEBatteriesAlt->SetSpringLoaded( true, 2 );

		pPowerSourceOrbiterSCConvPwrPri->SetInitialAnimState( 0.5f );
		pPowerSourceOrbiterSCConvPwrPri->DefineSwitchGroup( GRP_L10SWITCH3_VC );
		pPowerSourceOrbiterSCConvPwrPri->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceOrbiterSCConvPwrPri->SetMouseRegion( 0.237598f, 0.178404f, 0.269871f, 0.218495f );
		pPowerSourceOrbiterSCConvPwrPri->SetSpringLoaded( true, 0 );
		pPowerSourceOrbiterSCConvPwrPri->SetSpringLoaded( true, 2 );

		pPowerSourceOrbiterSCConvPwrAlt->SetInitialAnimState( 0.5f );
		pPowerSourceOrbiterSCConvPwrAlt->DefineSwitchGroup( GRP_L10SWITCH4_VC );
		pPowerSourceOrbiterSCConvPwrAlt->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceOrbiterSCConvPwrAlt->SetMouseRegion( 0.296558f, 0.179452f, 0.328858f, 0.218466f );
		pPowerSourceOrbiterSCConvPwrAlt->SetSpringLoaded( true, 0 );
		pPowerSourceOrbiterSCConvPwrAlt->SetSpringLoaded( true, 2 );

		pPowerSourceIUSSCBatteryPri->SetInitialAnimState( 0.5f );
		pPowerSourceIUSSCBatteryPri->DefineSwitchGroup( GRP_L10SWITCH5_VC );
		pPowerSourceIUSSCBatteryPri->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceIUSSCBatteryPri->SetMouseRegion( 0.395487f, 0.180426f, 0.429601f, 0.220517f );
		pPowerSourceIUSSCBatteryPri->SetSpringLoaded( true, 0 );
		pPowerSourceIUSSCBatteryPri->SetSpringLoaded( true, 2 );

		pPowerSourceIUSSCBatteryAlt->SetInitialAnimState( 0.5f );
		pPowerSourceIUSSCBatteryAlt->DefineSwitchGroup( GRP_L10SWITCH6_VC );
		pPowerSourceIUSSCBatteryAlt->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceIUSSCBatteryAlt->SetMouseRegion( 0.456171f, 0.180185f, 0.489332f, 0.219832f );
		pPowerSourceIUSSCBatteryAlt->SetSpringLoaded( true, 0 );
		pPowerSourceIUSSCBatteryAlt->SetSpringLoaded( true, 2 );

		pPowerSourceIUSBusABatteriesPri->SetInitialAnimState( 0.5f );
		pPowerSourceIUSBusABatteriesPri->DefineSwitchGroup( GRP_L10SWITCH7_VC );
		pPowerSourceIUSBusABatteriesPri->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceIUSBusABatteriesPri->SetMouseRegion( 0.530112f, 0.179549f, 0.562831f, 0.220697f );
		pPowerSourceIUSBusABatteriesPri->SetSpringLoaded( true, 0 );
		pPowerSourceIUSBusABatteriesPri->SetSpringLoaded( true, 2 );

		pPowerSourceIUSBusABatteriesAlt->SetInitialAnimState( 0.5f );
		pPowerSourceIUSBusABatteriesAlt->DefineSwitchGroup( GRP_L10SWITCH8_VC );
		pPowerSourceIUSBusABatteriesAlt->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceIUSBusABatteriesAlt->SetMouseRegion( 0.591074f, 0.179826f, 0.623668f, 0.220067f );
		pPowerSourceIUSBusABatteriesAlt->SetSpringLoaded( true, 0 );
		pPowerSourceIUSBusABatteriesAlt->SetSpringLoaded( true, 2 );

		pPowerSourceIUSBusBBatteriesPri->SetInitialAnimState( 0.5f );
		pPowerSourceIUSBusBBatteriesPri->DefineSwitchGroup( GRP_L10SWITCH9_VC );
		pPowerSourceIUSBusBBatteriesPri->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceIUSBusBBatteriesPri->SetMouseRegion( 0.660245f, 0.179550f, 0.690858f, 0.218969f );
		pPowerSourceIUSBusBBatteriesPri->SetSpringLoaded( true, 0 );
		pPowerSourceIUSBusBBatteriesPri->SetSpringLoaded( true, 2 );

		pPowerSourceIUSBusBBatteriesAlt->SetInitialAnimState( 0.5f );
		pPowerSourceIUSBusBBatteriesAlt->DefineSwitchGroup( GRP_L10SWITCH10_VC );
		pPowerSourceIUSBusBBatteriesAlt->SetReference( _V( -1.4441, 2.3686, 13.1502 ), SWITCH_ROT );
		pPowerSourceIUSBusBBatteriesAlt->SetMouseRegion( 0.720762f, 0.180016f, 0.753149f, 0.219112f );
		pPowerSourceIUSBusBBatteriesAlt->SetSpringLoaded( true, 0 );
		pPowerSourceIUSBusBBatteriesAlt->SetSpringLoaded( true, 2 );

		pIUSConvPwrPri->SetInitialAnimState( 0.5f );
		pIUSConvPwrPri->DefineSwitchGroup( GRP_L10SWITCH11_VC );
		pIUSConvPwrPri->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pIUSConvPwrPri->SetMouseRegion( 0.094067f, 0.390423f, 0.126817f, 0.430282f );
		pIUSConvPwrPri->SetSpringLoaded( true, 0 );
		pIUSConvPwrPri->SetSpringLoaded( true, 2 );

		pIUSConvPwrAlt->SetInitialAnimState( 0.5f );
		pIUSConvPwrAlt->DefineSwitchGroup( GRP_L10SWITCH12_VC );
		pIUSConvPwrAlt->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pIUSConvPwrAlt->SetMouseRegion( 0.155074f, 0.390227f, 0.188251f, 0.431424f );
		pIUSConvPwrAlt->SetSpringLoaded( true, 0 );
		pIUSConvPwrAlt->SetSpringLoaded( true, 2 );

		pSCSupportSCRegPwrPri->SetInitialAnimState( 0.5f );
		pSCSupportSCRegPwrPri->DefineSwitchGroup( GRP_L10SWITCH13_VC );
		pSCSupportSCRegPwrPri->SetReference( _V( -1.3572, 2.3106, 13.2181 ), SWITCH_ROT );
		pSCSupportSCRegPwrPri->SetMouseRegion( 0.2372062f, 0.359336f, 0.271265f, 0.399214f );
		pSCSupportSCRegPwrPri->SetSpringLoaded( true, 0 );
		pSCSupportSCRegPwrPri->SetSpringLoaded( true, 2 );

		pSCSupportSCRegPwrAlt->SetInitialAnimState( 0.5f );
		pSCSupportSCRegPwrAlt->DefineSwitchGroup( GRP_L10SWITCH14_VC );
		pSCSupportSCRegPwrAlt->SetReference( _V( -1.3572, 2.3106, 13.2181 ), SWITCH_ROT );
		pSCSupportSCRegPwrAlt->SetMouseRegion( 0.297839f, 0.359164f, 0.330424f, 0.399969f );
		pSCSupportSCRegPwrAlt->SetSpringLoaded( true, 0 );
		pSCSupportSCRegPwrAlt->SetSpringLoaded( true, 2 );

		pSCSupportSCPowerPri->SetInitialAnimState( 0.5f );
		pSCSupportSCPowerPri->DefineSwitchGroup( GRP_L10SWITCH15_VC );
		pSCSupportSCPowerPri->SetReference( _V( -1.3572, 2.3106, 13.2181 ), SWITCH_ROT );
		pSCSupportSCPowerPri->SetMouseRegion( 0.392651f, 0.359357f, 0.426633f, 0.400193f );
		pSCSupportSCPowerPri->SetSpringLoaded( true, 0 );
		pSCSupportSCPowerPri->SetSpringLoaded( true, 2 );

		pSCSupportSCPowerAlt->SetInitialAnimState( 0.5f );
		pSCSupportSCPowerAlt->DefineSwitchGroup( GRP_L10SWITCH16_VC );
		pSCSupportSCPowerAlt->SetReference( _V( -1.3572, 2.3106, 13.2181 ), SWITCH_ROT );
		pSCSupportSCPowerAlt->SetMouseRegion( 0.453977f, 0.360261f, 0.486132f, 0.400180f );
		pSCSupportSCPowerAlt->SetSpringLoaded( true, 0 );
		pSCSupportSCPowerAlt->SetSpringLoaded( true, 2 );

		pIUSSupportAFeedPri->SetInitialAnimState( 0.5f );
		pIUSSupportAFeedPri->DefineSwitchGroup( GRP_L10SWITCH17_VC );
		pIUSSupportAFeedPri->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pIUSSupportAFeedPri->SetMouseRegion( 0.533648f, 0.390069f, 0.567750f, 0.430798f );
		pIUSSupportAFeedPri->SetSpringLoaded( true, 0 );
		pIUSSupportAFeedPri->SetSpringLoaded( true, 2 );

		pIUSSupportAFeedAlt->SetInitialAnimState( 0.5f );
		pIUSSupportAFeedAlt->DefineSwitchGroup( GRP_L10SWITCH18_VC );
		pIUSSupportAFeedAlt->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pIUSSupportAFeedAlt->SetMouseRegion( 0.596000f, 0.389347f, 0.626979f, 0.430596f );
		pIUSSupportAFeedAlt->SetSpringLoaded( true, 0 );
		pIUSSupportAFeedAlt->SetSpringLoaded( true, 2 );

		pIUSSupportBFeedPri->SetInitialAnimState( 0.5f );
		pIUSSupportBFeedPri->DefineSwitchGroup( GRP_L10SWITCH19_VC );
		pIUSSupportBFeedPri->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pIUSSupportBFeedPri->SetMouseRegion( 0.660756f, 0.388670f, 0.691974f, 0.430090f );
		pIUSSupportBFeedPri->SetSpringLoaded( true, 0 );
		pIUSSupportBFeedPri->SetSpringLoaded( true, 2 );

		pIUSSupportBFeedAlt->SetInitialAnimState( 0.5f );
		pIUSSupportBFeedAlt->DefineSwitchGroup( GRP_L10SWITCH20_VC );
		pIUSSupportBFeedAlt->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pIUSSupportBFeedAlt->SetMouseRegion( 0.718953f, 0.390628f, 0.751530f, 0.430818f );
		pIUSSupportBFeedAlt->SetSpringLoaded( true, 0 );
		pIUSSupportBFeedAlt->SetSpringLoaded( true, 2 );

		pTestPri->SetInitialAnimState( 0.5f );
		pTestPri->DefineSwitchGroup( GRP_L10SWITCH21_VC );
		pTestPri->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pTestPri->SetMouseRegion( 0.822632f, 0.390149f, 0.855152f, 0.430973f );
		pTestPri->SetSpringLoaded( true, 0 );
		pTestPri->SetSpringLoaded( true, 2 );

		pTestAlt->SetInitialAnimState( 0.5f );
		pTestAlt->DefineSwitchGroup( GRP_L10SWITCH22_VC );
		pTestAlt->SetReference( _V( -1.3415, 2.3013, 13.1399 ), SWITCH_ROT );
		pTestAlt->SetMouseRegion( 0.883398f, 0.390262f, 0.914446f, 0.430280f );
		pTestAlt->SetSpringLoaded( true, 0 );
		pTestAlt->SetSpringLoaded( true, 2 );

		pIUSSupportHeatersBatteryPri->SetInitialAnimState( 0.5f );
		pIUSSupportHeatersBatteryPri->DefineSwitchGroup( GRP_L10SWITCH23_VC );
		pIUSSupportHeatersBatteryPri->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportHeatersBatteryPri->SetMouseRegion( 0.095766f, 0.568545f, 0.126822f, 0.608904f );
		pIUSSupportHeatersBatteryPri->SetSpringLoaded( true, 0 );
		pIUSSupportHeatersBatteryPri->SetSpringLoaded( true, 2 );

		pIUSSupportHeatersBatteryAlt->SetInitialAnimState( 0.5f );
		pIUSSupportHeatersBatteryAlt->DefineSwitchGroup( GRP_L10SWITCH24_VC );
		pIUSSupportHeatersBatteryAlt->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportHeatersBatteryAlt->SetMouseRegion( 0.154715f, 0.568617f, 0.185813f, 0.608895f );
		pIUSSupportHeatersBatteryAlt->SetSpringLoaded( true, 0 );
		pIUSSupportHeatersBatteryAlt->SetSpringLoaded( true, 2 );

		pIUSSupportHeatersCriticalPri->SetInitialAnimState( 0.5f );
		pIUSSupportHeatersCriticalPri->DefineSwitchGroup( GRP_L10SWITCH25_VC );
		pIUSSupportHeatersCriticalPri->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportHeatersCriticalPri->SetMouseRegion( 0.218462f, 0.567740f, 0.250986f, 0.608784f );
		pIUSSupportHeatersCriticalPri->SetSpringLoaded( true, 0 );
		pIUSSupportHeatersCriticalPri->SetSpringLoaded( true, 2 );

		pIUSSupportHeatersCriticalAlt->SetInitialAnimState( 0.5f );
		pIUSSupportHeatersCriticalAlt->DefineSwitchGroup( GRP_L10SWITCH26_VC );
		pIUSSupportHeatersCriticalAlt->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportHeatersCriticalAlt->SetMouseRegion( 0.278251f, 0.566908f, 0.311681f, 0.607395f );
		pIUSSupportHeatersCriticalAlt->SetSpringLoaded( true, 0 );
		pIUSSupportHeatersCriticalAlt->SetSpringLoaded( true, 2 );

		pIUSSupportWBDIPri->SetInitialAnimState( 0.5f );
		pIUSSupportWBDIPri->DefineSwitchGroup( GRP_L10SWITCH27_VC );
		pIUSSupportWBDIPri->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportWBDIPri->SetMouseRegion( 0.360761f, 0.568326f, 0.394916f, 0.608589f );
		pIUSSupportWBDIPri->SetSpringLoaded( true, 0 );
		pIUSSupportWBDIPri->SetSpringLoaded( true, 2 );

		pIUSSupportWBDIAlt->SetInitialAnimState( 0.5f );
		pIUSSupportWBDIAlt->DefineSwitchGroup( GRP_L10SWITCH28_VC );
		pIUSSupportWBDIAlt->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportWBDIAlt->SetMouseRegion( 0.422435f, 0.568450f, 0.455397f, 0.608290f );
		pIUSSupportWBDIAlt->SetSpringLoaded( true, 0 );
		pIUSSupportWBDIAlt->SetSpringLoaded( true, 2 );

		pIUSPowerPri->SetInitialAnimState( 0.5f );
		pIUSPowerPri->DefineSwitchGroup( GRP_L10SWITCH29_VC );
		pIUSPowerPri->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSPowerPri->SetMouseRegion( 0.541147f, 0.568843f, 0.573920f, 0.609133f );
		pIUSPowerPri->SetSpringLoaded( true, 0 );
		pIUSPowerPri->SetSpringLoaded( true, 2 );

		pIUSPowerAlt->SetInitialAnimState( 0.5f );
		pIUSPowerAlt->DefineSwitchGroup( GRP_L10SWITCH30_VC );
		pIUSPowerAlt->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSPowerAlt->SetMouseRegion( 0.599976f, 0.568463f, 0.631928f, 0.608484f );
		pIUSPowerAlt->SetSpringLoaded( true, 0 );
		pIUSPowerAlt->SetSpringLoaded( true, 2 );

		pNormBusPwrPri->SetInitialAnimState( 0.5f );
		pNormBusPwrPri->DefineSwitchGroup( GRP_L10SWITCH31_VC );
		pNormBusPwrPri->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pNormBusPwrPri->SetMouseRegion( 0.683755f, 0.569051f, 0.716306f, 0.609468f );
		pNormBusPwrPri->SetSpringLoaded( true, 0 );
		pNormBusPwrPri->SetSpringLoaded( true, 2 );

		pNormBusPwrAlt->SetInitialAnimState( 0.5f );
		pNormBusPwrAlt->DefineSwitchGroup( GRP_L10SWITCH32_VC );
		pNormBusPwrAlt->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pNormBusPwrAlt->SetMouseRegion( 0.743800f, 0.569090f, 0.776005f, 0.609276f );
		pNormBusPwrAlt->SetSpringLoaded( true, 0 );
		pNormBusPwrAlt->SetSpringLoaded( true, 2 );

		pIUSSupportAutoShutdownStandbyModePri->SetInitialAnimState( 0.5f );
		pIUSSupportAutoShutdownStandbyModePri->DefineSwitchGroup( GRP_L10SWITCH33_VC );
		pIUSSupportAutoShutdownStandbyModePri->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportAutoShutdownStandbyModePri->SetMouseRegion( 0.823231f, 0.569670f, 0.854505f, 0.609477f );
		pIUSSupportAutoShutdownStandbyModePri->SetSpringLoaded( true, 0 );
		pIUSSupportAutoShutdownStandbyModePri->SetSpringLoaded( true, 2 );

		pIUSSupportAutoShutdownStandbyModeAlt->SetInitialAnimState( 0.5f );
		pIUSSupportAutoShutdownStandbyModeAlt->DefineSwitchGroup( GRP_L10SWITCH34_VC );
		pIUSSupportAutoShutdownStandbyModeAlt->SetReference( _V( -1.2541, 2.2442, 13.1401 ), SWITCH_ROT );
		pIUSSupportAutoShutdownStandbyModeAlt->SetMouseRegion( 0.882804f, 0.569229f, 0.914825f, 0.608940f );
		pIUSSupportAutoShutdownStandbyModeAlt->SetSpringLoaded( true, 0 );
		pIUSSupportAutoShutdownStandbyModeAlt->SetSpringLoaded( true, 2 );

		pTiltTableActuatorDriveEnablePri1->SetInitialAnimState( 0.5f );
		pTiltTableActuatorDriveEnablePri1->DefineSwitchGroup( GRP_L10SWITCH35_VC );
		pTiltTableActuatorDriveEnablePri1->SetReference( _V( -1.1797, 2.1957, 13.1493 ), SWITCH_ROT );
		pTiltTableActuatorDriveEnablePri1->SetMouseRegion( 0.111635f, 0.719421f, 0.141082f, 0.759150f );

		pTiltTableActuatorDriveEnableAlt2->SetInitialAnimState( 0.5f );
		pTiltTableActuatorDriveEnableAlt2->DefineSwitchGroup( GRP_L10SWITCH36_VC );
		pTiltTableActuatorDriveEnableAlt2->SetReference( _V( -1.1797, 2.1957, 13.1493 ), SWITCH_ROT );
		pTiltTableActuatorDriveEnableAlt2->SetMouseRegion( 0.171649f, 0.719137f, 0.201649f, 0.759119f );

		pTiltTableActuatorMotionPri1->SetInitialAnimState( 0.5f );
		pTiltTableActuatorMotionPri1->DefineSwitchGroup( GRP_L10SWITCH37_VC );
		pTiltTableActuatorMotionPri1->SetReference( _V( -1.1797, 2.1957, 13.1493 ), SWITCH_ROT );
		pTiltTableActuatorMotionPri1->SetMouseRegion( 0.254209f, 0.719156f, 0.285940f, 0.759275f );
		pTiltTableActuatorMotionPri1->SetSpringLoaded( true, 0 );
		pTiltTableActuatorMotionPri1->SetSpringLoaded( true, 2 );

		pTiltTableActuatorMotionAlt2->SetInitialAnimState( 0.5f );
		pTiltTableActuatorMotionAlt2->DefineSwitchGroup( GRP_L10SWITCH38_VC );
		pTiltTableActuatorMotionAlt2->SetReference( _V( -1.1797, 2.1957, 13.1493 ), SWITCH_ROT );
		pTiltTableActuatorMotionAlt2->SetMouseRegion( 0.314874f, 0.719508f, 0.347282f, 0.759363f );
		pTiltTableActuatorMotionAlt2->SetSpringLoaded( true, 0 );
		pTiltTableActuatorMotionAlt2->SetSpringLoaded( true, 2 );

		pTiltTableActuatorAltDrAct1->SetInitialAnimState( 0.5f );
		pTiltTableActuatorAltDrAct1->DefineSwitchGroup( GRP_L10TOGGLE2_VC );
		pTiltTableActuatorAltDrAct1->SetReference( _V( -1.1782, 2.1988, 13.5538 ), SWITCH_ROT );
		pTiltTableActuatorAltDrAct1->SetMouseRegion( 0.754145f, 0.717768f, 0.787739f, 0.759606f );
		pTiltTableActuatorAltDrAct1->SetPullDirection( SWITCH_PULL );
		pTiltTableActuatorAltDrAct1->SetSpringLoaded( true, 1 );

		pCmdPathEnabled->SetInitialAnimState( 0.5f );
		pCmdPathEnabled->DefineSwitchGroup( GRP_L10TOGGLE3_VC );
		pCmdPathEnabled->SetReference( _V( -1.1782, 2.1988, 13.5538 ), SWITCH_ROT );
		pCmdPathEnabled->SetMouseRegion( 0.838003f, 0.717705f, 0.870962f, 0.759780f );
		pCmdPathEnabled->SetPullDirection( SWITCH_PULL );
		pCmdPathEnabled->SetInitialPosition( 0 );

		pPyroBusPri->SetInitialAnimState( 0.5f );
		pPyroBusPri->DefineSwitchGroup( GRP_L10SWITCH39_VC );
		pPyroBusPri->SetReference( _V( -1.0958, 2.1410, 13.1492 ), SWITCH_ROT );
		pPyroBusPri->SetMouseRegion( 0.111920f, 0.890384f, 0.145745f, 0.930124f );
		pPyroBusPri->SetSpringLoaded( true, 0 );
		pPyroBusPri->SetSpringLoaded( true, 2 );

		pPyroBusAlt->SetInitialAnimState( 0.5f );
		pPyroBusAlt->DefineSwitchGroup( GRP_L10SWITCH40_VC );
		pPyroBusAlt->SetReference( _V( -1.0958, 2.1410, 13.1492 ), SWITCH_ROT );
		pPyroBusAlt->SetMouseRegion( 0.172300f, 0.889325f, 0.203690f, 0.929150f );
		pPyroBusAlt->SetSpringLoaded( true, 0 );
		pPyroBusAlt->SetSpringLoaded( true, 2 );

		pUmbilicalsEnaPri->SetInitialAnimState( 0.5f );
		pUmbilicalsEnaPri->DefineSwitchGroup( GRP_L10SWITCH41_VC );
		pUmbilicalsEnaPri->SetReference( _V( -1.0958, 2.1410, 13.1492 ), SWITCH_ROT );
		pUmbilicalsEnaPri->SetMouseRegion( 0.255508f, 0.889941f, 0.287612f, 0.930524f );
		pUmbilicalsEnaPri->SetSpringLoaded( true, 0 );
		pUmbilicalsEnaPri->SetSpringLoaded( true, 2 );

		pUmbilicalsEnaAlt->SetInitialAnimState( 0.5f );
		pUmbilicalsEnaAlt->DefineSwitchGroup( GRP_L10SWITCH42_VC );
		pUmbilicalsEnaAlt->SetReference( _V( -1.0958, 2.1410, 13.1492 ), SWITCH_ROT );
		pUmbilicalsEnaAlt->SetMouseRegion( 0.316503f, 0.890866f, 0.347555f, 0.930231f );
		pUmbilicalsEnaAlt->SetSpringLoaded( true, 0 );
		pUmbilicalsEnaAlt->SetSpringLoaded( true, 2 );

		pUmbilicalsRelPri->SetInitialAnimState( 0.5f );
		pUmbilicalsRelPri->DefineSwitchGroup( GRP_L10TOGGLE4_VC );
		pUmbilicalsRelPri->SetReference( _V( -1.0939, 2.1438, 13.3103 ), SWITCH_ROT );
		pUmbilicalsRelPri->SetMouseRegion( 0.395237f, 0.887448f, 0.424710f, 0.930452f );
		pUmbilicalsRelPri->SetPullDirection( SWITCH_PULL );
		pUmbilicalsRelPri->SetSpringLoaded( true, 1 );

		pUmbilicalsRelAlt->SetInitialAnimState( 0.5f );
		pUmbilicalsRelAlt->DefineSwitchGroup( GRP_L10TOGGLE5_VC );
		pUmbilicalsRelAlt->SetReference( _V( -1.0939, 2.1438, 13.3103 ), SWITCH_ROT );
		pUmbilicalsRelAlt->SetMouseRegion( 0.449887f, 0.889443f, 0.480940f, 0.931571f );
		pUmbilicalsRelAlt->SetPullDirection( SWITCH_PULL );
		pUmbilicalsRelAlt->SetSpringLoaded( true, 1 );

		pIUSDeploymentEnaPri->SetInitialAnimState( 0.5f );
		pIUSDeploymentEnaPri->DefineSwitchGroup( GRP_L10SWITCH43_VC );
		pIUSDeploymentEnaPri->SetReference( _V( -1.0958, 2.1410, 13.1492 ), SWITCH_ROT );
		pIUSDeploymentEnaPri->SetMouseRegion( 0.534739f, 0.890996f, 0.565632f, 0.929932f );
		pIUSDeploymentEnaPri->SetSpringLoaded( true, 0 );
		pIUSDeploymentEnaPri->SetSpringLoaded( true, 2 );

		pIUSDeploymentEnaAlt->SetInitialAnimState( 0.5f );
		pIUSDeploymentEnaAlt->DefineSwitchGroup( GRP_L10SWITCH44_VC );
		pIUSDeploymentEnaAlt->SetReference( _V( -1.0958, 2.1410, 13.1492 ), SWITCH_ROT );
		pIUSDeploymentEnaAlt->SetMouseRegion( 0.591902f, 0.889207f, 0.624428f, 0.928453f );
		pIUSDeploymentEnaAlt->SetSpringLoaded( true, 0 );
		pIUSDeploymentEnaAlt->SetSpringLoaded( true, 2 );

		pIUSDeploymentDpyPri->SetInitialAnimState( 0.5f );
		pIUSDeploymentDpyPri->DefineSwitchGroup( GRP_L10TOGGLE6_VC );
		pIUSDeploymentDpyPri->SetReference( _V( -1.0939, 2.1438, 13.3103 ), SWITCH_ROT );
		pIUSDeploymentDpyPri->SetMouseRegion( 0.666605f, 0.889142f, 0.699841f, 0.929472f );
		pIUSDeploymentDpyPri->SetPullDirection( SWITCH_PULL );
		pIUSDeploymentDpyPri->SetSpringLoaded( true, 1 );

		pIUSDeploymentDpyAlt->SetInitialAnimState( 0.5f );
		pIUSDeploymentDpyAlt->DefineSwitchGroup( GRP_L10TOGGLE7_VC );
		pIUSDeploymentDpyAlt->SetReference( _V( -1.0939, 2.1438, 13.3103 ), SWITCH_ROT );
		pIUSDeploymentDpyAlt->SetMouseRegion( 0.726243f, 0.889508f, 0.758000f, 0.929481f );
		pIUSDeploymentDpyAlt->SetPullDirection( SWITCH_PULL );
		pIUSDeploymentDpyAlt->SetSpringLoaded( true, 1 );

		pPowerKill->SetInitialAnimState( 0.5f );
		pPowerKill->DefineSwitchGroup( GRP_L10TOGGLE8_VC );
		pPowerKill->SetReference( _V( -1.1141, 2.1573, 13.5453 ), SWITCH_ROT );
		pPowerKill->SetMouseRegion( 0.834209f, 0.848043f, 0.869189f, 0.889788f );
		pPowerKill->SetPullDirection( SWITCH_PULL );
		pPowerKill->SetSpringLoaded( true, 1 );

		pPowerSourceASEBatteriesPriTB->DefineMeshGroup( mesh_index, GRP_L10TB1_VC );
		pPowerSourceASEBatteriesAltTB->DefineMeshGroup( mesh_index, GRP_L10TB2_VC );
		pPowerSourceOrbiterSCConvPwrPriTB->DefineMeshGroup( mesh_index, GRP_L10TB3_VC );
		pPowerSourceOrbiterSCConvPwrAltTB->DefineMeshGroup( mesh_index, GRP_L10TB4_VC );
		pPowerSourceIUSSCBatteryPriTB->DefineMeshGroup( mesh_index, GRP_L10TB5_VC );
		pPowerSourceIUSSCBatteryAltTB->DefineMeshGroup( mesh_index, GRP_L10TB6_VC );
		pPowerSourceIUSBusABatteriesPriTB->DefineMeshGroup( mesh_index, GRP_L10TB7_VC );
		pPowerSourceIUSBusABatteriesAltTB->DefineMeshGroup( mesh_index, GRP_L10TB8_VC );
		pPowerSourceIUSBusBBatteriesPriTB->DefineMeshGroup( mesh_index, GRP_L10TB9_VC );
		pPowerSourceIUSBusBBatteriesAltTB->DefineMeshGroup( mesh_index, GRP_L10TB10_VC );
		pIUSConvPwrPriTB->DefineMeshGroup( mesh_index, GRP_L10TB11_VC );
		pIUSConvPwrAltTB->DefineMeshGroup( mesh_index, GRP_L10TB12_VC );
		pSCSupportSCRegPwrPriTB->DefineMeshGroup( mesh_index, GRP_L10TB13_VC );
		pSCSupportSCRegPwrAltTB->DefineMeshGroup( mesh_index, GRP_L10TB14_VC );
		pSCSupportSCPowerPriTB->DefineMeshGroup( mesh_index, GRP_L10TB15_VC );
		pSCSupportSCPowerAltTB->DefineMeshGroup( mesh_index, GRP_L10TB16_VC );
		pIUSSupportAFeedPriTB->DefineMeshGroup( mesh_index, GRP_L10TB17_VC );
		pIUSSupportAFeedAltTB->DefineMeshGroup( mesh_index, GRP_L10TB18_VC );
		pIUSSupportBFeedPriTB->DefineMeshGroup( mesh_index, GRP_L10TB19_VC );
		pIUSSupportBFeedAltTB->DefineMeshGroup( mesh_index, GRP_L10TB20_VC );
		pMatrixStatusPriTB->DefineMeshGroup( mesh_index, GRP_L10TB21_VC );
		pMatrixStatusAltTB->DefineMeshGroup( mesh_index, GRP_L10TB22_VC );
		pIUSSupportHeatersBatteryPriTB->DefineMeshGroup( mesh_index, GRP_L10TB23_VC );
		pIUSSupportHeatersBatteryAltTB->DefineMeshGroup( mesh_index, GRP_L10TB24_VC );
		pIUSSupportHeatersCriticalPriTB->DefineMeshGroup( mesh_index, GRP_L10TB25_VC );
		pIUSSupportHeatersCriticalAltTB->DefineMeshGroup( mesh_index, GRP_L10TB26_VC );
		pIUSSupportWBDIPriTB->DefineMeshGroup( mesh_index, GRP_L10TB27_VC );
		pIUSSupportWBDIAltTB->DefineMeshGroup( mesh_index, GRP_L10TB28_VC );
		pIUSPowerPriTB->DefineMeshGroup( mesh_index, GRP_L10TB29_VC );
		pIUSPowerAltTB->DefineMeshGroup( mesh_index, GRP_L10TB30_VC );
		pNormBusPwrPriTB->DefineMeshGroup( mesh_index, GRP_L10TB31_VC );
		pNormBusPwrAltTB->DefineMeshGroup( mesh_index, GRP_L10TB32_VC );
		pIUSSupportAutoShutdownStandbyModePriTB->DefineMeshGroup( mesh_index, GRP_L10TB33_VC );
		pIUSSupportAutoShutdownStandbyModeAltTB->DefineMeshGroup( mesh_index, GRP_L10TB34_VC );
		pTiltTableActuatorMotionPri1TB->DefineMeshGroup( mesh_index, GRP_L10TB35_VC );
		pTiltTableActuatorMotionAlt2TB->DefineMeshGroup( mesh_index, GRP_L10TB36_VC );
		pTiltTableActuatorPositionPri1TB->DefineMeshGroup( mesh_index, GRP_L10TB37_VC );
		pTiltTableActuatorPositionAlt2TB->DefineMeshGroup( mesh_index, GRP_L10TB38_VC );
		pTiltTableActuatorPriDrAct1EngagedTB->DefineMeshGroup( mesh_index, GRP_L10TB39_VC );
		pTiltTableActuatorAltDrAct2EngagedTB->DefineMeshGroup( mesh_index, GRP_L10TB40_VC );
		pPyroBusPriTB->DefineMeshGroup( mesh_index, GRP_L10TB41_VC );
		pPyroBusAltTB->DefineMeshGroup( mesh_index, GRP_L10TB42_VC );
		pUmbilicalsEnaPriTB->DefineMeshGroup( mesh_index, GRP_L10TB43_VC );
		pUmbilicalsEnaAltTB->DefineMeshGroup( mesh_index, GRP_L10TB44_VC );
		pUmbilicalsRelPriTB->DefineMeshGroup( mesh_index, GRP_L10TB45_VC );
		pUmbilicalsRelAltTB->DefineMeshGroup( mesh_index, GRP_L10TB46_VC );
		pIUSDeploymentEnaPriTB->DefineMeshGroup( mesh_index, GRP_L10TB47_VC );
		pIUSDeploymentEnaAltTB->DefineMeshGroup( mesh_index, GRP_L10TB48_VC );
		return;
	}

	void PanelL10::AddMeshes( const VECTOR3 &ofs )
	{
		SetHasOwnVCMesh();

		if (mesh_index == MESH_UNDEFINED)
		{
			mesh_index = STS()->AddMesh( hPanelMesh, &ofs );
			STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		}
		return;
	}

	void PanelL10::SetMeshVisibility( bool visible )
	{
		if (visible) STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_VC );
		else STS()->SetMeshVisibilityMode( mesh_index, MESHVIS_NEVER );
		return;
	}

	UINT PanelL10::GetVCMeshIndex() const
	{
		return mesh_index;
	}

	void PanelL10::DefineVCAnimations( UINT vcidx )
	{
		AtlantisPanel::DefineVCAnimations( mesh_index );
		return;
	}

	void PanelL10::Realize()
	{
		DiscreteBundle* pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_A", 16 );
		pPanelMode->ConnectPort( 1, pBundle, 0 );
		pPowerSourceASEBatteriesPri->ConnectPort( 1, pBundle, 1 );
		pPowerSourceASEBatteriesPri->ConnectPort( 2, pBundle, 2 );
		pPowerSourceASEBatteriesPriTB->SetInput( 0, pBundle, 3, TB_GRAY );
		pPowerSourceASEBatteriesAlt->ConnectPort( 1, pBundle, 4 );
		pPowerSourceASEBatteriesAlt->ConnectPort( 2, pBundle, 5 );
		pPowerSourceASEBatteriesAltTB->SetInput( 0, pBundle, 6, TB_GRAY );
		pPowerSourceOrbiterSCConvPwrPri->ConnectPort( 1, pBundle, 7 );
		pPowerSourceOrbiterSCConvPwrPri->ConnectPort( 2, pBundle, 8 );
		pPowerSourceOrbiterSCConvPwrPriTB->SetInput( 0, pBundle, 9, TB_GRAY );
		pPowerSourceOrbiterSCConvPwrAlt->ConnectPort( 1, pBundle, 10 );
		pPowerSourceOrbiterSCConvPwrAlt->ConnectPort( 2, pBundle, 11 );
		pPowerSourceOrbiterSCConvPwrAltTB->SetInput( 0, pBundle, 12, TB_GRAY );
		pPowerSourceIUSSCBatteryPri->ConnectPort( 1, pBundle, 13 );
		pPowerSourceIUSSCBatteryPri->ConnectPort( 2, pBundle, 14 );
		pPowerSourceIUSSCBatteryPriTB->SetInput( 0, pBundle, 15, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_B", 16 );
		pPowerSourceIUSSCBatteryAlt->ConnectPort( 1, pBundle, 0 );
		pPowerSourceIUSSCBatteryAlt->ConnectPort( 2, pBundle, 1 );
		pPowerSourceIUSSCBatteryAltTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pPowerSourceIUSBusABatteriesPri->ConnectPort( 1, pBundle, 3 );
		pPowerSourceIUSBusABatteriesPri->ConnectPort( 2, pBundle, 4 );
		pPowerSourceIUSBusABatteriesPriTB->SetInput( 0, pBundle, 5, TB_GRAY );
		pPowerSourceIUSBusABatteriesAlt->ConnectPort( 1, pBundle, 6 );
		pPowerSourceIUSBusABatteriesAlt->ConnectPort( 2, pBundle, 7 );
		pPowerSourceIUSBusABatteriesAltTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pPowerSourceIUSBusBBatteriesPri->ConnectPort( 1, pBundle, 9 );
		pPowerSourceIUSBusBBatteriesPri->ConnectPort( 2, pBundle, 10 );
		pPowerSourceIUSBusBBatteriesPriTB->SetInput( 0, pBundle, 11, TB_GRAY );
		pPowerSourceIUSBusBBatteriesAlt->ConnectPort( 1, pBundle, 12 );
		pPowerSourceIUSBusBBatteriesAlt->ConnectPort( 2, pBundle, 13 );
		pPowerSourceIUSBusBBatteriesAltTB->SetInput( 0, pBundle, 14, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_C", 16 );
		pIUSConvPwrPri->ConnectPort( 1, pBundle, 0 );
		pIUSConvPwrPri->ConnectPort( 2, pBundle, 1 );
		pIUSConvPwrPriTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pIUSConvPwrAlt->ConnectPort( 1, pBundle, 3 );
		pIUSConvPwrAlt->ConnectPort( 2, pBundle, 4 );
		pIUSConvPwrAltTB->SetInput( 0, pBundle, 5, TB_GRAY );
		pSCSupportSCRegPwrPri->ConnectPort( 1, pBundle, 6 );
		pSCSupportSCRegPwrPri->ConnectPort( 2, pBundle, 7 );
		pSCSupportSCRegPwrPriTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pSCSupportSCRegPwrAlt->ConnectPort( 1, pBundle, 9 );
		pSCSupportSCRegPwrAlt->ConnectPort( 2, pBundle, 10 );
		pSCSupportSCRegPwrAltTB->SetInput( 0, pBundle, 11, TB_GRAY );
		pSCSupportSCPowerPri->ConnectPort( 1, pBundle, 12 );
		pSCSupportSCPowerPri->ConnectPort( 2, pBundle, 13 );
		pSCSupportSCPowerPriTB->SetInput( 0, pBundle, 14, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_D", 16 );
		pSCSupportSCPowerAlt->ConnectPort( 1, pBundle, 0 );
		pSCSupportSCPowerAlt->ConnectPort( 2, pBundle, 1 );
		pSCSupportSCPowerAltTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pIUSSupportAFeedPri->ConnectPort( 1, pBundle, 3 );
		pIUSSupportAFeedPri->ConnectPort( 2, pBundle, 4 );
		pIUSSupportAFeedPriTB->SetInput( 0, pBundle, 5, TB_GRAY );
		pIUSSupportAFeedAlt->ConnectPort( 1, pBundle, 6 );
		pIUSSupportAFeedAlt->ConnectPort( 2, pBundle, 7 );
		pIUSSupportAFeedAltTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pIUSSupportBFeedPri->ConnectPort( 1, pBundle, 9 );
		pIUSSupportBFeedPri->ConnectPort( 2, pBundle, 10 );
		pIUSSupportBFeedPriTB->SetInput( 0, pBundle, 11, TB_GRAY );
		pIUSSupportBFeedAlt->ConnectPort( 1, pBundle, 12 );
		pIUSSupportBFeedAlt->ConnectPort( 2, pBundle, 13 );
		pIUSSupportBFeedAltTB->SetInput( 0, pBundle, 14, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_E", 16 );
		pTestPri->ConnectPort( 1, pBundle, 0 );
		pTestPri->ConnectPort( 2, pBundle, 1 );
		pMatrixStatusPriTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pTestAlt->ConnectPort( 1, pBundle, 3 );
		pTestAlt->ConnectPort( 2, pBundle, 4 );
		pMatrixStatusAltTB->SetInput( 0, pBundle, 5, TB_GRAY );
		pIUSSupportHeatersBatteryPri->ConnectPort( 1, pBundle, 6 );
		pIUSSupportHeatersBatteryPri->ConnectPort( 2, pBundle, 7 );
		pIUSSupportHeatersBatteryPriTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pIUSSupportHeatersBatteryAlt->ConnectPort( 1, pBundle, 9 );
		pIUSSupportHeatersBatteryAlt->ConnectPort( 2, pBundle, 10 );
		pIUSSupportHeatersBatteryAltTB->SetInput( 0, pBundle, 11, TB_GRAY );
		pIUSSupportHeatersCriticalPri->ConnectPort( 1, pBundle, 12 );
		pIUSSupportHeatersCriticalPri->ConnectPort( 2, pBundle, 13 );
		pIUSSupportHeatersCriticalPriTB->SetInput( 0, pBundle, 14, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_F", 16 );
		pIUSSupportHeatersCriticalAlt->ConnectPort( 1, pBundle, 0 );
		pIUSSupportHeatersCriticalAlt->ConnectPort( 2, pBundle, 1 );
		pIUSSupportHeatersCriticalAltTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pIUSSupportWBDIPri->ConnectPort( 1, pBundle, 3 );
		pIUSSupportWBDIPri->ConnectPort( 2, pBundle, 4 );
		pIUSSupportWBDIPriTB->SetInput( 0, pBundle, 5, TB_GRAY );
		pIUSSupportWBDIAlt->ConnectPort( 1, pBundle, 6 );
		pIUSSupportWBDIAlt->ConnectPort( 2, pBundle, 7 );
		pIUSSupportWBDIAltTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pIUSPowerPri->ConnectPort( 1, pBundle, 9 );
		pIUSPowerPri->ConnectPort( 2, pBundle, 10 );
		pIUSPowerPriTB->SetInput( 0, pBundle, 11, TB_GRAY );
		pIUSPowerAlt->ConnectPort( 1, pBundle, 12 );
		pIUSPowerAlt->ConnectPort( 2, pBundle, 13 );
		pIUSPowerAltTB->SetInput( 0, pBundle, 14, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_G", 16 );
		pNormBusPwrPri->ConnectPort( 1, pBundle, 0 );
		pNormBusPwrPri->ConnectPort( 2, pBundle, 1 );
		pNormBusPwrPriTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pNormBusPwrAlt->ConnectPort( 1, pBundle, 3 );
		pNormBusPwrAlt->ConnectPort( 2, pBundle, 4 );
		pNormBusPwrAltTB->SetInput( 0, pBundle, 5, TB_GRAY );
		pIUSSupportAutoShutdownStandbyModePri->ConnectPort( 1, pBundle, 6 );
		pIUSSupportAutoShutdownStandbyModePri->ConnectPort( 2, pBundle, 7 );
		pIUSSupportAutoShutdownStandbyModePriTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pIUSSupportAutoShutdownStandbyModeAlt->ConnectPort( 1, pBundle, 9 );
		pIUSSupportAutoShutdownStandbyModeAlt->ConnectPort( 2, pBundle, 10 );
		pIUSSupportAutoShutdownStandbyModeAltTB->SetInput( 0, pBundle, 11, TB_GRAY );
		pTiltTableActuatorDriveEnablePri1->ConnectPort( 1, pBundle, 12 );
		pTiltTableActuatorDriveEnablePri1->ConnectPort( 2, pBundle, 13 );
		pTiltTableActuatorDriveEnableAlt2->ConnectPort( 1, pBundle, 14 );
		pTiltTableActuatorDriveEnableAlt2->ConnectPort( 2, pBundle, 15 );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_H", 16 );
		pTiltTableActuatorMotionPri1->ConnectPort( 1, pBundle, 0 );
		pTiltTableActuatorMotionPri1->ConnectPort( 2, pBundle, 1 );
		pTiltTableActuatorMotionAlt2->ConnectPort( 1, pBundle, 2 );
		pTiltTableActuatorMotionAlt2->ConnectPort( 2, pBundle, 3 );
		pTiltTableActuatorMotionPri1TB->SetInput( 0, pBundle, 4, TB_GRAY );
		pTiltTableActuatorMotionAlt2TB->SetInput( 0, pBundle, 5, TB_GRAY );
		pTiltTableActuatorPositionPri1TB->SetInput( 0, pBundle, 6, TB_GRAY );
		pTiltTableActuatorPositionAlt2TB->SetInput( 0, pBundle, 7, TB_GRAY );
		pTiltTableActuatorPriDrAct1EngagedTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pTiltTableActuatorAltDrAct2EngagedTB->SetInput( 0, pBundle, 9, TB_GRAY );
		pTiltTableActuatorAltDrAct1->ConnectPort( 1, pBundle, 10 );
		pCmdPathEnabled->ConnectPort( 1, pBundle, 11 );
		pCmdPathEnabled->ConnectPort( 2, pBundle, 12 );
		pPyroBusPri->ConnectPort( 1, pBundle, 13 );
		pPyroBusPri->ConnectPort( 2, pBundle, 14 );
		pPyroBusPriTB->SetInput( 0, pBundle, 15, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_I", 16 );
		pPyroBusAlt->ConnectPort( 1, pBundle, 0 );
		pPyroBusAlt->ConnectPort( 2, pBundle, 1 );
		pPyroBusAltTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pUmbilicalsEnaPri->ConnectPort( 1, pBundle, 3 );
		pUmbilicalsEnaPri->ConnectPort( 2, pBundle, 4 );
		pUmbilicalsEnaPriTB->SetInput( 0, pBundle, 5, TB_GRAY );
		pUmbilicalsEnaAlt->ConnectPort( 1, pBundle, 6 );
		pUmbilicalsEnaAlt->ConnectPort( 2, pBundle, 7 );
		pUmbilicalsEnaAltTB->SetInput( 0, pBundle, 8, TB_GRAY );
		pUmbilicalsRelPri->ConnectPort( 1, pBundle, 9 );
		pUmbilicalsRelPriTB->SetInput( 0, pBundle, 10, TB_GRAY );
		pUmbilicalsRelAlt->ConnectPort( 1, pBundle, 11 );
		pUmbilicalsRelAltTB->SetInput( 0, pBundle, 12, TB_GRAY );
		pIUSDeploymentEnaPri->ConnectPort( 1, pBundle, 13 );
		pIUSDeploymentEnaPri->ConnectPort( 2, pBundle, 14 );
		pIUSDeploymentEnaPriTB->SetInput( 0, pBundle, 15, TB_GRAY );

		pBundle = STS()->BundleManager()->CreateBundle( "L10toASE_J", 16 );
		pIUSDeploymentEnaAlt->ConnectPort( 1, pBundle, 0 );
		pIUSDeploymentEnaAlt->ConnectPort( 2, pBundle, 1 );
		pIUSDeploymentEnaAltTB->SetInput( 0, pBundle, 2, TB_GRAY );
		pIUSDeploymentDpyPri->ConnectPort( 1, pBundle, 3 );
		pIUSDeploymentDpyAlt->ConnectPort( 1, pBundle, 4 );
		pPowerKill->ConnectPort( 1, pBundle, 5 );
		
		AtlantisPanel::Realize();
		return;
	}
};
