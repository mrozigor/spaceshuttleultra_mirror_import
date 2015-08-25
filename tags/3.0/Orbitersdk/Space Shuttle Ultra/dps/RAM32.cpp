#include "RAM32.h"

namespace dps {

RAM32::RAM32()
{
}

RAM32::~RAM32()
{
}

void RAM32::OnSaveState(FILEHANDLE scn)
{
	char buffer[256];
	long dword_buffer[8];
	unsigned long ulDumpAddr = 0;

	while(ulDumpAddr + 8 <= GetNumberOfWords())
	{
		sprintf_s(buffer, 255, "%5X %8X %8X %8X %8X %8X %8X %8X %8X",
			ulDumpAddr, 
			GetDWord(ulDumpAddr + 0),
			GetDWord(ulDumpAddr + 1),
			GetDWord(ulDumpAddr + 2),
			GetDWord(ulDumpAddr + 3),
			GetDWord(ulDumpAddr + 4),
			GetDWord(ulDumpAddr + 5),
			GetDWord(ulDumpAddr + 6),
			GetDWord(ulDumpAddr + 7));
		oapiWriteScenario_string(scn, "MEMORY", buffer);
		ulDumpAddr += 8;
	}
	if(ulDumpAddr < GetNumberOfWords())
	{
		for(unsigned int i = 0; i<8; i++)
			dword_buffer[i] = 0;

		for(unsigned int i = 0; ulDumpAddr + i < GetNumberOfWords(); i++)
			dword_buffer[i] = GetDWord(ulDumpAddr + i);

		sprintf_s(buffer, 255, "%5X %8X %8X %8X %8X %8X %8X %8X %8X",
			ulDumpAddr, 
			dword_buffer[0],
			dword_buffer[1],
			dword_buffer[2],
			dword_buffer[3],
			dword_buffer[4],
			dword_buffer[5],
			dword_buffer[6],
			dword_buffer[7]);
		oapiWriteScenario_string(scn, "MEMORY", buffer);
		
	}
}

void RAM32::OnParseLine(char *line)
{
	oapiWriteLog("(RAM32::OnParseLine) Unsupported function used.");
}

void RAM32::ReadImage(const std::string &strFileName)
{
	oapiWriteLog("(RAM32::ReadImage) Unsupported function used.");
}

void RAM32::WriteImage(const std::string &strFileName) const
{
	oapiWriteLog("(RAM32::WriteImage) Unsupported function used.");
}

};