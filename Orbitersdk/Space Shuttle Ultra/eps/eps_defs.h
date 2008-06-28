#pragma once

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
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps(ACPHASE phase) const = 0;
		virtual bool DrawPower(ACPHASE phase, double fPower) const = 0;
	};

	/**
	 * A power bus distributes electrical power from one or more sources
	 * to one or more power units. 
	 */
	class PowerBus;
	class DCBus;
	class ACBus;


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

};