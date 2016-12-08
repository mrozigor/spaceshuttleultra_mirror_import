#include "HALS_Types.h"

namespace dps 
{

	namespace hals
	{
		Symbol::Symbol()
			: m_name("__unknown"), m_hwaddr(0), m_fwaddr(0)
		{
		}

		Symbol::Symbol(const std::string& name, word16 hw_addr)
			: m_name(name), m_hwaddr(hw_addr), m_fwaddr(0)
		{
		}

		Symbol::~Symbol()
		{
		}

		Symbol::VARIABLE_TYPE Symbol::Type() const
		{
			return Symbol::UNKNOWN;
		}

		Symbol::VARIABLE_TYPE Symbol::Subtype() const
		{
			return Symbol::UNKNOWN;
		}

		word16 Symbol::size() const
		{
			return 0;
		}

		void Symbol::Y(word16 ofs)
		{
			m_hwaddr = ofs;
		}

		word16 Symbol::Y() const
		{
			return m_hwaddr;
		}

		word32 Symbol::Resolve(SimpleGPC* gpc, word32 base_addr)
		{
			m_fwaddr = base_addr + m_hwaddr;
			return m_fwaddr;
		}
	};

};