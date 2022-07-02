// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include "web/api.hpp"
unsigned long __stdcall initial_thread(void* base_pointer) {

    // here's where you'd get the username communicated from the loader
    // I'd reccomend using https://github.com/GiovanniDicanio/WinReg 
    // It's a really simple registry wrapper and It makes using the registry for IPC really easy

    // for testing purposes, I just strcpy my account username to the username
    std::strcpy(api::user, "YOURUSERNAMEHERE");

    ATL::CAccessToken access_token;
    ATL::CSid current_sid;

    if (!access_token.GetProcessToken(TOKEN_READ | TOKEN_QUERY) || !access_token.GetUser(&current_sid))
        return false;

    // copy HWID to the client_hwid char
    std::strcpy(api::client_hwid, (const char*)current_sid.Sid());

    // call api on_init
    api::on_init();
}

int __stdcall DllMain(HMODULE base_pointer, unsigned int reason_to_call, void* reserved)
{
	if (reason_to_call == 1) {
        DisableThreadLibraryCalls(base_pointer);
        CreateThread(nullptr, 0, &initial_thread, base_pointer, 0, nullptr);
    }
}