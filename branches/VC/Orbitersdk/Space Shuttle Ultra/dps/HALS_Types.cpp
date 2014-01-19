#include "HALS_Types.h"
#include  <exception>

namespace dps
{
	namespace hals {

		

		INTEGER::INTEGER()
			: m_var(0)
		{
		}

		INTEGER::INTEGER(const std::string& name, word16 hw_addr)
			: Symbol(name, hw_addr), m_var(0)
		{
		}

		INTEGER::~INTEGER()
		{
		}

		word16 INTEGER::size() const
		{
			return 2;
		}


		word32 INTEGER::Resolve(SimpleGPC* gpc, word32 base)
		{
			word32 addr = Symbol::Resolve(gpc, base);
			return addr;
		}

		INTEGER& INTEGER::operator =(const INTEGER& b)
		{
			if (m_var && b.m_var)
			{
				*m_var = *(b.m_var);
			}
			else
				throw UnresolvedVariableError(m_name);
			return *this;
		}

		INTEGER& INTEGER::operator =(const word32 b)
		{
			if (m_var)
			{
				*m_var = static_cast<word16>(b);
			}
			else
				throw UnresolvedVariableError(m_name);

			return *this;
		}

		bool INTEGER::operator ==(const INTEGER& b) const
		{
			if(m_var && b.m_var)
			{
				return (*m_var == *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return false;
			}
		}

		bool INTEGER::operator ==(const word32 b) const
		{
			if(m_var)
			{
				return (*m_var == b);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return false;
			}
		}
		
		bool INTEGER::operator >=(const INTEGER& b) const
		{
			if(m_var && b.m_var)
			{
				return (*m_var >= *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return false;
			}
		}

		bool INTEGER::operator <=(const INTEGER& b) const
		{
			if(m_var && b.m_var)
			{
				return (*m_var <= *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return false;
			}
		}

		bool INTEGER::operator >(const INTEGER& b) const
		{
			if(m_var && b.m_var)
			{
				return (*m_var > *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return false;
			}
		}

		bool INTEGER::operator <(const INTEGER& b) const
		{
			if(m_var && b.m_var)
			{
				return (*m_var < *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return false;
			}
		}

		word32 INTEGER::operator -(const INTEGER& b) const
		{
			
			if(m_var && b.m_var)
			{
				return (*m_var - *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator -(const word32 b) const
		{
			
			if(m_var)
			{
				return (*m_var & b);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator +(const INTEGER& b) const
		{
			
			if(m_var && b.m_var)
			{
				return (*m_var + *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator +(const word32 b) const
		{
			
			if(m_var)
			{
				return (*m_var + b);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator *(const INTEGER& b) const
		{
			
			if(m_var && b.m_var)
			{
				return (*m_var * *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator *(const word32 b) const
		{
			
			if(m_var)
			{
				return (*m_var * b);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator /(const INTEGER& b) const
		{
			
			if(m_var && b.m_var)
			{
				return (*m_var / *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator /(const word32 b) const
		{
			
			if(m_var)
			{
				return (*m_var / b);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator |(const INTEGER& b) const
		{
			
			if(m_var && b.m_var)
			{
				return (*m_var | *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator |(const word32 b) const
		{
			
			if(m_var)
			{
				return (*m_var | b);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator &(const INTEGER& b) const
		{
			
			if(m_var && b.m_var)
			{
				return (*m_var & *(b.m_var));
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator &(const word32 b) const
		{
			
			if(m_var)
			{
				return (*m_var & b);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}

		word32 INTEGER::operator() () const
		{
			if(m_var)
			{
				return (*m_var);
			}
			else
			{
				throw UnresolvedVariableError(m_name);
				return 0;
			}
		}
	};
};