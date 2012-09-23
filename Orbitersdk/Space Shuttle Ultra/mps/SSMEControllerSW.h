/****************************************************************************
  This file is part of Space Shuttle Ultra

  Space Shuttle Main Engine Controller Software definition



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
#ifndef _mps_SSMECONTROLLERSW_H_
#define _mps_SSMECONTROLLERSW_H_


namespace mps
{
	// must be the same values as in CIE
	const unsigned short DEV_CIE_OEchA = 0x0000;// 0000 0000 0000 0000
	const unsigned short DEV_CIE_OEchB = 0x1000;// 0001 0000 0000 0000

	const unsigned short DEV_OE_ON_OFF_1 = 0x0001;// 0000 0000 0000 0001
	const unsigned short DEV_OE_ON_OFF_2 = 0x0002;// 0000 0000 0000 0010
	const unsigned short DEV_OE_CCV = 0x0003;// 0000 0000 0000 0011
	const unsigned short DEV_OE_MFV = 0x0004;// 0000 0000 0000 0100
	const unsigned short DEV_OE_MOV = 0x0005;// 0000 0000 0000 0101
	const unsigned short DEV_OE_FPOV = 0x0006;// 0000 0000 0000 0110
	const unsigned short DEV_OE_OPOV = 0x0007;// 0000 0000 0000 0111

	const unsigned short DEV_CIE_VIE_CMD1 = 0x2000;// 0010 0000 0000 0000
	const unsigned short DEV_CIE_VIE_CMD2 = 0x2100;// 0010 0001 0000 0000
	const unsigned short DEV_CIE_VIE_CMD3 = 0x2200;// 0010 0010 0000 0000

	class DigitalComputerUnit;

	class SSMEControllerSW
	{
		protected:
			DigitalComputerUnit* DCU;
		public:
			SSMEControllerSW( DigitalComputerUnit* DCU );
			virtual ~SSMEControllerSW( void );

			virtual void Checkout_HydraulicConditioning( void ) = 0;
			virtual void Checkout_Standby( void ) = 0;
			virtual void Checkout_ActuatorCheckout( void ) = 0;
			virtual void Checkout_EngineLeakDetection( void ) = 0;
			virtual void Checkout_IgniterCheckout( void ) = 0;
			virtual void Checkout_PneumaticCheckout( void ) = 0;
			virtual void Checkout_SensorCheckout( void ) = 0;
			virtual void Checkout_ControllerCheckout( void ) = 0;

			virtual void StartPrep_PSN1( void ) = 0;
			virtual void StartPrep_PSN2( void ) = 0;
			virtual void StartPrep_PSN3( void ) = 0;
			virtual void StartPrep_PSN4( void ) = 0;
			virtual void StartPrep_EngineReady( void ) = 0;

			virtual void Start_StartInitiation( void ) = 0;
			virtual void Start_ThrustBuildup( void ) = 0;

			virtual void Mainstage_NormalControl( void ) = 0;
			virtual void Mainstage_FixedDensity( void ) = 0;
			virtual void Mainstage_ThrustLimiting( void ) = 0;
			virtual void Mainstage_HydraulicLockup( void ) = 0;
			virtual void Mainstage_ElectricalLockup( void ) = 0;

			virtual void Shutdown_ThrottleTo0( void ) = 0;
			virtual void Shutdown_PropellantValvesClosed( void ) = 0;
			virtual void Shutdown_FailSafePneumatic( void ) = 0;

			virtual void PostShutdown_Standby( void ) = 0;
			virtual void PostShutdown_OxidizerDump( void ) = 0;
			virtual void PostShutdown_TerminateSequence( void ) = 0;

			void GetCommands( void );
			void ValidateCommands( void );

			void SensorInput( void );

			virtual void BuiltVDT( void ) = 0;

			virtual void PowerFailureSense( void ) = 0;
	};
}


#endif// _mps_SSMECONTROLLERSW_H_
