#pragma once

namespace eps {

	/**
	 * A power bus distributes electrical power from one or more sources
	 * to one or more power units. 
	 */
	class PowerBus;
	class DCBus;
	class ACBus;

	/**
	 * Interface defining the behavior of sources of power
	 * 
	 */
	class IPowerNode {
	public:
		/**
		 * electrical potential at the node
		 */
		virtual double GetVoltage() const;
		/** 
		 * either amps produced by a power source or amps flowing 
		 * through a node
		 */
		virtual double GetAmps() const;
		virtual bool DrawPower(double fPower) const;
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

};