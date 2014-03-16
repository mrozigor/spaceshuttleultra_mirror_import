//code by Face

#pragma once
#pragma unmanaged
#include "orbitersdk.h"
#pragma managed
#include <vcclr.h>
#include <map>

using namespace System::Reflection;

namespace SSU
{
	namespace Toolbox
	{
		namespace Client
		{
			class Wrapper;

			public ref class Launcher
			{
			public:
				Launcher();
				void SetWrapper(Wrapper *wrapper);
				void Open();
				void Init();
			private:
				Object^ managed;
				PropertyInfo^ wrapper;
				MethodInfo^ open;
				MethodInfo^ init;
			};

			public class Wrapper
			{
			public:
				Wrapper(Launcher^ launcher);
				HWND GetManaged(HWND unmanaged);
				void CreateUnmanagedDialog(HWND managed);
				void CloseUnmanagedDialog(HWND managed);
				void Open();
				void Init();
			private:				
				gcroot<Launcher^> launcher;
				std::map<HWND, HWND> managed2unmanaged, unmanaged2managed;
				HWND current;
			};
		}	
	}
}