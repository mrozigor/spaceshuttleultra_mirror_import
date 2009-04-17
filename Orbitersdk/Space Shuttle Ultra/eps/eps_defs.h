#pragma once
#include <string>
#include <vector>
#include "../discsignals/DiscInPort.h"
#include "../AtlantisSubsystem.h"

namespace eps {

	typedef enum {
		PHASE1 = 0,
		PHASE2,
		PHASE3
	} ACPHASE;

	class IDCElement {
	public:
		/**
		 * electrical potential at the node
		 */
		virtual double GetVoltage() const = 0;
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps() const = 0;
		virtual double GetPower() const = 0;
		virtual bool DrawPower(double fPower) const = 0;
	};

	class IACElement {
	public:
		/**
		 * electrical potential at the node
		 */
		virtual double GetVoltage(ACPHASE phase) const = 0;
		virtual double GetPhaseAngle(ACPHASE phase) const = 0;
		virtual double GetPhaseDrift(ACPHASE phase) const = 0;
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps(ACPHASE phase) const = 0;
		virtual bool DrawPower(ACPHASE phase, double fPower) = 0;
		virtual bool DrawPower(double fPower) = 0;

		virtual bool ConnectToSource(IACElement* pSource) = 0;
		virtual void acPropagate(double fSimT, double fDeltaT) = 0;
	};

	/**
	 * A power bus distributes electrical power from one or more sources
	 * to one or more power units. 
	 */
	class PowerBus;
	class DCBus;
	
	class ACBus : public IACElement
	{
		std::string strName;
		std::vector<IACElement*> sources;
		double fPowerP1[2];
		double fPowerP2[2];
		double fPowerP3[2];
	protected:
		friend class ACBusSystem;
		ACBus(const std::string& name);
	public:
		
		virtual ~ACBus();
		const std::string& GetName() const;
		virtual double GetVoltage(ACPHASE phase) const;
		virtual double GetPhaseAngle(ACPHASE phase) const;
		virtual double GetPhaseDrift(ACPHASE phase) const;
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps(ACPHASE phase) const;
		virtual bool DrawPower(double fPower);
		virtual bool DrawPower(ACPHASE phase, double fPower);

		virtual bool ConnectToSource(IACElement* pSource);

		void acPropagate(double fSimT, double fDeltaT);
	};


	/**
	 * A Power supply unit is a special power unit, which supply 
	 * electrical power to a bus, either from a power generator 
	 * or battery, or another bus. 
	 */
	class PowerSupplyUnit;

	/** 
	 * A power unit draws power from the connected power bus and 
	 * manages available power.
	 */
	class PowerUnit;
	class DCPowerUnit;
	class ACPowerUnit;


	/**
	 * Motor controllers switch AC power in one or the the other direction of flow
	*/
	class ACMotorController: public IACElement
	{
		std::string strName;
		std::vector<IACElement*> sources;
		double fPowerP1[2];
		double fPowerP2[2];
		double fPowerP3[2];
		float GetFlowDirection() const;
	public:
		ACMotorController(const std::string& name);
		virtual ~ACMotorController();
		virtual double GetVoltage(ACPHASE phase) const;
		virtual double GetPhaseAngle(ACPHASE phase) const;
		virtual double GetPhaseDrift(ACPHASE phase) const;
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps(ACPHASE phase) const;
		virtual bool DrawPower(double fPower);
		virtual bool DrawPower(ACPHASE phase, double fPower);

		virtual bool ConnectToSource(IACElement* pSource);
	
		void acPropagate(double fSimT, double fDeltaT);
	
		discsignals::DiscInPort CmdFwd;
		discsignals::DiscInPort CmdRwd;
	};

	/** 
	 *
	 */
	class ACMotor: public IACElement 
	{
		IACElement* pACSource;
		double fMotorSpeed;
	public:
		ACMotor();

		/**
		 * electrical potential at the node
		 */
		virtual double GetVoltage(ACPHASE phase) const;
		virtual double GetPhaseAngle(ACPHASE phase) const;
		virtual double GetPhaseDrift(ACPHASE phase) const;
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps(ACPHASE phase) const;
		virtual bool DrawPower(ACPHASE phase, double fPower);
		virtual bool DrawPower(double fPower);

		virtual bool ConnectToSource(IACElement* pSource);
		virtual void acPropagate(double fSimT, double fDeltaT);

		void SetSpeed(double fNewSpeed, double fTorque0, double fTorque1, double fDeltaT);
		double GetSpeed() const;
		
		/**
		 * Calculate the torque of a AC Motor
		 * 
		 * If powered: release brakes. 
		 * If not powered: brakes apply.
		 */
		double GetTorque(double fSpeed) const;
	};

	class Inverter : public IACElement, public AtlantisSubsystem
	{
		std::string strName;
		std::vector<IACElement*> sources;
		double fPowerP1[2];
		double fPowerP2[2];
		double fPowerP3[2];
	public:
		Inverter(SubsystemDirector* _direct, const std::string& name);
		virtual ~Inverter();
		virtual double GetVoltage(ACPHASE phase) const;
		virtual double GetPhaseAngle(ACPHASE phase) const;
		virtual double GetPhaseDrift(ACPHASE phase) const;
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps(ACPHASE phase) const;
		virtual bool DrawPower(double fPower);
		virtual bool DrawPower(ACPHASE phase, double fPower);

		virtual bool ConnectToSource(IACElement* pSource);

		void acPropagate(double fSimT, double fDeltaT);

		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);
	};
	

	class ACBusSystem: public AtlantisSubsystem {
		std::vector<ACBus*> bus_systems;
		ACBus* CreateBus(const std::string& name);
	public:
		ACBusSystem(SubsystemDirector* _pDirect);
		virtual ~ACBusSystem();
		virtual void OnPreStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPostStep(double fSimT, double fDeltaT, double fMJD);
		virtual void OnPropagate(double fSimT, double fDeltaT, double fMJD);

		bool ExistsBus(const std::string& name) const;
		ACBus* FindBus(const std::string& name) const;
		ACBus* GetBus(const std::string& name);
	};

};