#pragma once
#include <OrbiterAPI.h>
#include <string>

/**
 * Simple inline class for storing options for SSU and parsing them
 * from a configuration file.
 */ 
class SSUOptions
{
	bool m_available;
	bool m_complex_GPC;
	bool m_complex_IDP;
	std::string m_romfile_path;
public:

	SSUOptions(void)
		: m_available(false), m_complex_GPC(false), 
		  m_complex_IDP(false), m_romfile_path("Config\\SSU\\ROM")
	{
	}

	virtual ~SSUOptions(void)
	{
	}

	void Parse(FILEHANDLE cfg)
	{
		static char buffer[400];
		oapiReadItem_bool(cfg, "ComplexGPC", m_complex_GPC);
		oapiReadItem_bool(cfg, "ComplexIDP", m_complex_IDP);
		if(oapiReadItem_string(cfg, "ROMPath", buffer))
		{
			m_romfile_path = buffer;
		}
		m_available = true;

	}

	bool AreAvailable() const
	{
		return m_available;
	}

	const std::string& GetROMFilePath() const {
		return m_romfile_path;
	}

	bool UseComplexGPC() const {
		return m_complex_GPC;
	};

	bool UseComplexIDP() const {
		return m_complex_IDP;
	};

	
};
