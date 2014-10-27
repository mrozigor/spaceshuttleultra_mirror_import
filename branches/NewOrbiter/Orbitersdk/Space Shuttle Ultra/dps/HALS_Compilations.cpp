#include <OrbiterAPI.h>
#include "HALS_Compilations.h"

namespace dps {
	namespace hals {

		Compilation::Compilation(const std::string& name)
			: Symbol(name, 0)
		{
		}

		Compilation::~Compilation()
		{
		}

		void Compilation::Define(Symbol& sym)
		{
			sym_table.push_back(&sym);
		}

		void Compilation::Declare(Symbol& sym)
		{
			sym.Y(new_ofs);
			new_ofs += sym.size();
			Define(sym);
		}

		word32 Compilation::Resolve(SimpleGPC* gpc, word32 base)
		{
			return 0;
		}

		void Compilation::Schedule(Compilation& proc, unsigned char prio)
		{
			//Create SVC Schedule Parameter List
			//Get PDE
			//
		}

		word32 Compilation::GetSize() const
		{
			return new_ofs;
		}

		word32 Compilation::operator()(word32 calling_pc)
		{
			return 0;
		}

		void Compilation::LogException(std::exception& e)
		{
			static char buffer[400];
			sprintf_s(buffer, 400, "(Space Shuttle Ultra) Exception in HAL/S software %s: %s",
				m_name.c_str(), e.what());
			oapiWriteLog(buffer);
		}
	};
};