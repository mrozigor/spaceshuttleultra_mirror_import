#include "GPCSoftware.h"
#include "GPC.h"

namespace dps
{

	

	

	GPCSoftware::GPCSoftware(const std::string& name)
		: Compilation(name)
	{
	}

	GPCSoftware::~GPCSoftware()
	{
	}

	
	word16 GPCSoftware::operator ()(word16 instr_addr)
	{
		//Idle task: endless loop
		return instr_addr;
	}
};