#include "Includes.h"

void InitiateHooks()
{
    //Ammo
    DWORD ammo = FindPatterns((char*)"ac_client.exe", (char*)"\x89\x0A\x8B\x76\x14\xFF\x0E", (char*)"xxxxxxx");
    ammo += 5;
    //MsgBox(ammo);

    AmmoJmpBack = ammo + 0x7;

    PlaceJMP((BYTE*)ammo, (DWORD)InfiniteAmmo, 7);




    //mouseOnPlayer (Is the mouse overlapped with the player)
    DWORD mouseOnPlayer = FindPatterns((char*)"ac_client.exe", (char*)"\x8B\x5C\x24\x3C\x68\x00\x00\x00\x00\xFF\xD7\xE8\x00\x00\x00\x00", (char*)"xxxxx????xxx????");
    mouseOnPlayer += 11;
    //MsgBox(mouseOnPlayer);

    mouseOnPlayerJmpBack = mouseOnPlayer + 0x5;

    PlaceJMP((BYTE*)mouseOnPlayer, (DWORD)Triggerbot, 5);

}
DWORD WINAPI OverwriteValues()
{

    while (TRUE)
    {
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
        {
             Aimbot();
        }
        Sleep(1);
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        InitiateHooks();
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)OverwriteValues, NULL, NULL, NULL);
        break;
    }
    return TRUE;
}