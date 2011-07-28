#include "MMU.h"

namespace dps {

	const DWORD BLOCK_SIZE = 1024;

	MMU::MMU(AtlantisSubsystemDirector *_direct, const std::string &_ident)
		: AtlantisSubsystem(_direct, _ident), tape_image(NULL)
	{

	}

	MMU::~MMU()
	{
		if(tape_image)
		{
			CloseHandle(tape_image);
		}
	}

	void MMU::SetTapeImage(const std::string &img_file)
	{
		tape_image = CreateFile(img_file.c_str(), (GENERIC_READ | GENERIC_WRITE), 
			FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(tape_image == INVALID_HANDLE_VALUE)
		{
			char buffer[300];
			DWORD dwErr = GetLastError();
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 
				LANG_USER_DEFAULT, buffer, 300, NULL);
			oapiWriteLog(buffer);
			tape_image = NULL;
		} else {
			//If the file had been created
			if(GetLastError() != ERROR_ALREADY_EXISTS)
			{
				//Produce an empty tape
				memset(block_buffer, 0, BLOCK_SIZE);
				for(int i = 0; i<8*2048; i++) {
					WriteToBlock(i);
				}
			}
		}
	}

	bool MMU::WriteToBlock(short block_index)
	{
		DWORD wordcount = BLOCK_SIZE;
		if(tape_image != NULL)
		{
			DWORD newPos = SetFilePointer(tape_image, block_index * BLOCK_SIZE, NULL, FILE_BEGIN);
			if(newPos != block_index * BLOCK_SIZE)
			{
				oapiWriteLog("Positioning tape for reading failed.");
				return false;
			}

			if(!WriteFile(tape_image, block_buffer, BLOCK_SIZE, &wordcount, NULL)) {
				char buffer[300];
				DWORD dwErr = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 
					LANG_USER_DEFAULT, buffer, 300, NULL);
				oapiWriteLog(buffer);
				return false;
			}
			return true;
		}
		return false;
	}

	bool MMU::ReadBlock(short block_index)
	{
		DWORD wordcount = BLOCK_SIZE;
		if(tape_image != NULL)
		{
			DWORD newPos = SetFilePointer(tape_image, block_index * BLOCK_SIZE, NULL, FILE_BEGIN);
			if(newPos != block_index * BLOCK_SIZE)
			{
				oapiWriteLog("Positioning tape for reading failed.");
				return false;
			}

			if(!ReadFile(tape_image, block_buffer, BLOCK_SIZE, &wordcount, NULL)) {
				char buffer[300];
				DWORD dwErr = GetLastError();
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 
					LANG_USER_DEFAULT, buffer, 300, NULL);
				oapiWriteLog(buffer);
				return false;
			}
			return true;
		}
		return false;
	}


};