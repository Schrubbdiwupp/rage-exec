#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <process.h>
#include <chrono>
#include <Windows.h>
#include <filesystem>

#include "detours.h"
#include "HWBP.h"

extern "C" __int64 SetRbx(const char*);
typedef size_t(__cdecl* _StrLen)(const char* Str);
typedef __int64(__fastcall* _Value)(__int64 a1, __int64 a2, __int64 a3);

_StrLen StrLen = (_StrLen)((uintptr_t)GetModuleHandleA("v8.dll") + 0xC920E0);
_Value Value = (_Value)((uintptr_t)GetModuleHandleA("v8.dll") + 0x30CE0);

const char* Str = nullptr;
int count = 0;
int max_count = 1;

std::string execstr;

__int64 __fastcall DetourValue(__int64 a1, __int64 a2, __int64 a3)
{
    HWBP::DeleteHWBP((DWORD_PTR)Value);

    __int64 res = Value(a1, a2, a3);

    strcpy(*(char**)res, Str);

    return res;
}

__declspec(noinline) void CopyString()
{
    execstr += Str;
}

size_t __cdecl DetourStrLen(const char* _Str)
{
    Str = _Str;

    if (count == max_count)
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach((LPVOID*)&StrLen, DetourStrLen);
        DetourTransactionCommit();

        CopyString();
        SetRbx(execstr.c_str());

        HWBP::InstallHWBP((DWORD_PTR)Value, (DWORD_PTR)DetourValue);

        return execstr.size();
    }

    count++;

    return StrLen(_Str);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach((LPVOID*)&StrLen, DetourStrLen);
        DetourTransactionCommit();

        std::filesystem::path tmp_dir_path = { std::filesystem::temp_directory_path() /= "execfile.js" };
        std::ifstream file(tmp_dir_path);

        if (file)
            std::getline(file, execstr, '\0');
    }
    return TRUE;
}