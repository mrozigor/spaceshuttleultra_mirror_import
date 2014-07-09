#pragma once
#include "HALS_Types.h"
#include <vector>

namespace dps
{
	namespace hals {

		/**
		* Basic class for all data types that can have resolvable data.
		* Represents symbolic data in GPC RAM
		*/
		class Compilation: public Symbol {
		private:
			word16 new_ofs;
			std::vector<Symbol*> sym_table;
		protected:
			SimpleGPC* m_gpc;
			//std::string m_name;
			virtual void Define(Symbol& sym);
			virtual void Declare(Symbol& sym);
			void LogException(std::exception& e);
		protected:
			void Schedule(Compilation& proc, unsigned char priority);
			void ScheduleOn(Compilation& proc, unsigned char priority, EventExpression& evt);
			void ScheduleIn(Compilation& proc, unsigned char priority, double delay);
			void ScheduleAt(Compilation& proc, unsigned char priority, double t0);
			void Terminate(Compilation& proc);
			void Cancel(Compilation& proc);
			void Wait(double delay);
			void WaitUntil(double t0);
			void WaitFor(EventExpression& evt);
			void UpdatePriority(Compilation& proc, unsigned char priority);
			double Runtime() const;
			unsigned char Prio() const;
			void Set(hals::EVENT& evt);
			void Reset(hals::EVENT& evt);
			void Signal(hals::EVENT& evt);
			void Call(Compilation* called);
			word16 SVC(unsigned char function);
		public:
			Compilation(const std::string& name = "__unnamed");
			virtual ~Compilation();
			virtual word32 Resolve(SimpleGPC* gpc, word32 base_addr);
			virtual word32 operator()(word32 calling_pc);
			virtual word32 GetSize() const;
		};

		class CompilationList:public Compilation
		{
		protected:
			std::vector<Symbol*> m_global_symbols;
			std::vector<Compilation*> m_compilation_units;
		public:
			CompilationList(const std::string& name = "__unnamed");
			virtual ~CompilationList();
			virtual word32 Resolve(SimpleGPC* gpc, word32 base_addr);
		};
	};
};