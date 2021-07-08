#include "Injection.h"
#include <filesystem>
#include <iostream>
#include <string>

DWORD GetProcessId()
{
	DWORD procId = 0;

	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
		return 0;

	BOOL bRet = Process32First(hSnap, &PE32);
	while (bRet)
	{
		if (!wcscmp(L"GTA5.exe", PE32.szExeFile))
		{
			procId = PE32.th32ProcessID;
			break;
		}
		bRet = Process32Next(hSnap, &PE32);
	}
	CloseHandle(hSnap);
	return procId;
}

int main()
{
	std::ifstream src("multiplayerL.dll", std::ios::binary);

	if (src.fail())
	{
		std::cout << "Can't find multiplayerL.dll" << std::endl;
		system("pause");
		return 0;
	}

	std::ofstream file("C:/RAGEMP/multiplayerL.dll", std::ios::binary);
	file << src.rdbuf();
	file.close();

	std::cout << "DLL patched! Waiting for GTA5..." << std::endl;

	DWORD procId = 0;

	while (!procId)
	{
		procId = GetProcessId();
		Sleep(10);
	}

	std::cout << "Found GTA5! Injecting..." << std::endl;

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	if (hProc)
	{
		std::string fileName;
		std::string execstr;

		std::cout << "exec file name: ";
		std::cin >> fileName;

		std::ifstream ifs("C:/rage-exec/" + fileName);

		if (ifs)
			std::getline(ifs, execstr, '\0');
		else
		{
			std::cout << "Couldn't open file!" << std::endl;
			system("pause");
			return 0;
		}

		std::filesystem::path tmp_dir_path = { std::filesystem::temp_directory_path() /= "execfile.js" };
		std::ofstream ofs(tmp_dir_path);
		ofs << execstr;
		ofs.close();

		char path[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, path);
		strcat_s(path, MAX_PATH, "\\rage-exec.dll");

		ManualMap(hProc, path);
		CloseHandle(hProc);
	}

	return 0;
}