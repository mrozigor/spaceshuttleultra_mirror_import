#pragma once
#include <OrbiterAPI.h>

/**
 * Simple inline class for storing options for SSU and parsing them
 * from a configuration file.
 */ 
class SSUOptions
{
	bool m_available;
	bool m_complex_GPC;
	bool m_complex_IDP;
public:

	SSUOptions(void)
		: m_available(false), m_complex_GPC(false), 
		  m_complex_IDP(false)
	{
	}

	virtual ~SSUOptions(void)
	{
	}

	void Parse(FILEHANDLE cfg)
	{
		oapiReadItem_bool(cfg, "ComplexGPC", m_complex_GPC);
		oapiReadItem_bool(cfg, "ComplexIDP", m_complex_IDP);
		m_available = true;
	}

	bool AreAvailable() const
	{
		return m_available;
	}

	bool UseComplexGPC() const {
		return m_complex_GPC;
	};

	bool UseComplexIDP() const {
		return m_complex_IDP;
	};
};
