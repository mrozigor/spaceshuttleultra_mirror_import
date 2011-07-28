#pragma once
#include "dps_defs.h"
#include <string>

namespace dps {

	namespace hals {

		class EventExpression;
		class INTEGER;
		class DOUBLE_INTEGER;
		class SCALAR;
		class EVENT;
		class BIT;
		class CHARACTER;
		class ARRAY;
		class NAME;
		class STRUCTURE;
		class STRUCT_TEMPLATE;
		class PROGRAM;
		class COMPOOL;
		class FUNCTION;
		class PROCEDURE;

		class Symbol {
		public:
			typedef enum {
				UNKNOWN = 0,
				PROGRAM,
				FUNCTION,
				TASK,
				STRUCTURE,
				ARRAY,
				BITSTRING,
				BIT,
				CHARACTER,
				SCALAR,
				INTEGER,
				BOOLEAN,
				MATRIX,
				VECTOR,
				EVENT,
				ADDR_Y,
				ADDR_AC,
				ADDR_Z,
				FIXED
			} VARIABLE_TYPE;
		protected:
			std::string m_name;
			word16 m_hwaddr;
			word32 m_fwaddr;
			bool m_constant;
		public:
			Symbol();
			Symbol(const std::string& name, word16 hw_addr);
			Symbol(const Symbol& copy);
			virtual ~Symbol();
			const std::string& Name() const;
			virtual VARIABLE_TYPE Type() const;
			virtual VARIABLE_TYPE Subtype() const;
			virtual word16 size() const;
			void Y(word16 ofs);
			word16 Y() const;
			word32 Z() const;

			virtual word32 Resolve(SimpleGPC* gpc, word32 base_addr);
		};


		class PROCEDURE: public Symbol
		{
		public:
			PROCEDURE();
			PROCEDURE(const std::string& name);
			PROCEDURE(const std::string& name, word16 hwaddr);
			PROCEDURE(const PROCEDURE& copy);
			virtual ~PROCEDURE();
		};

		class FUNCTION: public Symbol
		{
		public:
			FUNCTION();
			FUNCTION(const std::string& name);
			FUNCTION(const std::string& name, word16 hwaddr);
			FUNCTION(const FUNCTION& copy);
			virtual ~FUNCTION();
		};

		class INTEGER: public Symbol
		{
			word16* m_var;
		public:
			INTEGER();
			INTEGER(const std::string& name, word16 hw_addr);
			INTEGER(const INTEGER& copy);
			virtual ~INTEGER();

			virtual word16 size() const;
			virtual word32 Resolve(SimpleGPC* gpc, word32 base_addr);

			INTEGER& operator =(const INTEGER& b);
			INTEGER& operator =(const word32 b);

			bool operator ==(const INTEGER& b) const;
			bool operator ==(const word32 b) const;

			bool operator >=(const INTEGER& b) const;
			bool operator >=(const word32 b) const;

			bool operator <=(const INTEGER& b) const;
			bool operator <=(const word32 b) const;

			bool operator >(const INTEGER& b) const;
			bool operator >(const word32 b) const;

			bool operator <(const INTEGER& b) const;
			bool operator <(const word32 b) const;

			word32 operator -(const INTEGER& b) const;
			word32 operator -(const word32 b) const;

			word32 operator +(const INTEGER& b) const;
			word32 operator +(const word32 b) const;

			word32 operator *(const INTEGER& b) const;
			word32 operator *(const word32 b) const;

			word32 operator /(const INTEGER& b) const;
			word32 operator /(const word32 b) const;

			word32 operator &(const INTEGER& b) const;
			word32 operator &(const word32 b) const;

			word32 operator |(const INTEGER& b) const;
			word32 operator |(const word32 b) const;

			word32 operator() () const;
		};

		inline word32 operator +(word32 a, const INTEGER& b)
		{
			return b + a;
		}

		inline word32 operator -(word32 a, const INTEGER& b)
		{
			return a - b();
		}

		inline word32 operator *(word32 a, const INTEGER& b)
		{
			return b * a;
		}

		inline word32 operator /(word32 a, const INTEGER& b)
		{
			return a / b();
		}

		class DOUBLE_INTEGER: public Symbol
		{
			word32* m_resolved_var;
		public:
			DOUBLE_INTEGER();
			DOUBLE_INTEGER(const std::string& name, word16 hw_addr);
			DOUBLE_INTEGER(const DOUBLE_INTEGER& copy);

			virtual word32 Resolve(GPC* gpc, word32 base_addr);

			DOUBLE_INTEGER& operator =(const hals::INTEGER& b) const;
			DOUBLE_INTEGER& operator =(const DOUBLE_INTEGER& b) const;

			bool operator ==(const dps::hals::INTEGER& b) const;
			bool operator ==(const DOUBLE_INTEGER& b) const;
			bool operator >=(const DOUBLE_INTEGER& b) const;
			bool operator <=(const DOUBLE_INTEGER& b) const;
			bool operator >(const DOUBLE_INTEGER& b) const;
			bool operator <(const DOUBLE_INTEGER& b) const;

			word32 operator -(const hals::INTEGER& b) const;
			word32 operator +(const hals::INTEGER& b) const;
			word32 operator *(const hals::INTEGER& b) const;
			word32 operator /(const hals::INTEGER& b) const;
			word32 operator &(const hals::INTEGER& b) const;
			word32 operator |(const hals::INTEGER& b) const;

			word32 operator -(const DOUBLE_INTEGER& b) const;
			word32 operator +(const DOUBLE_INTEGER& b) const;
			word32 operator *(const DOUBLE_INTEGER& b) const;
			word32 operator /(const DOUBLE_INTEGER& b) const;
			word32 operator &(const DOUBLE_INTEGER& b) const;
			word32 operator |(const DOUBLE_INTEGER& b) const;
		};

		class EVENT: public Symbol
		{
			word16* m_evt_variable;
		public:
			EVENT();
			EVENT(const std::string& name, word16 hw_addr);
			EVENT(const EVENT& evt);
			virtual ~EVENT();

		};

		

		class UnresolvedVariableError: public std::logic_error
		{
			std::string m_symbol;
			std::string m_type;
		public:
			UnresolvedVariableError(const std::string& var_symbol, const std::string& hals_type = "INTEGER")
				: std::logic_error(std::string("Unresolved HAL/S variable ") 
				+ var_symbol + std::string(":") 
				+ hals_type + std::string(".")), 
				m_symbol(var_symbol),
				m_type(hals_type)
			{
			}

			UnresolvedVariableError(const UnresolvedVariableError& e)
				: std::logic_error(e), m_symbol(e.m_symbol), m_type(e.m_type)
			{

			}

			virtual ~UnresolvedVariableError() {	}

			virtual const char* what() const 
			{
				static char buffer[400];
				sprintf_s(buffer, 400, "Unresolved HAL/S variable %s:%s",
					m_symbol.c_str(), m_type.c_str());
				return buffer;
			}
		};

	};
};