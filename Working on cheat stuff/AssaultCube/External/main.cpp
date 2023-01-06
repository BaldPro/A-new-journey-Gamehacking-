/*
* #include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

int main(int argc, char** argv) {
    DWORD process_id = 0;

    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapShot, &entry))
    {
        do
        {
            if (!strcmp("ac_client.exe", entry.szExeFile))
            {
                process_id = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapShot, &entry));
    }

    HANDLE assault_Cube_process = OpenProcess(PROCESS_ALL_ACCESS, true, process_id);

    DWORD health_Value = 0;
    SIZE_T bytes_read = 0;

    BOOL success = ReadProcessMemory(assault_Cube_process, (void*)0x00F1A4A8, &health_Value, 4, &bytes_read);
    if (success)
    {
        // Reading was successful
        std::cout << "Health value: " << health_Value << std::endl;
    }
    else
    {
        // An error occurred
        std::cout << "Error reading process memory" << std::endl;
    }

    return 0;
}
*/

#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
int main(int argc, char** argv) {
    DWORD process_id = 0;

    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapShot, &entry))
    {
        do
        {
            if (!strcmp("ac_client.exe", (char*)entry.szExeFile))
            {
                process_id = entry.th32ProcessID;
                break;
            }
        } while (Process32Next(snapShot, &entry));
    }

    HANDLE assault_Cube_process = OpenProcess(PROCESS_ALL_ACCESS, true, process_id);

    int pointer_To_Health = 0xF1A3B0;
    DWORD randomstuffloleee = 0;
    auto health_Offset = 0xF8;
    SIZE_T bytes_read = 0;

    BOOL success = ReadProcessMemory(assault_Cube_process, (void*)pointer_To_Health, &randomstuffloleee, 4, &bytes_read);
    if (success)
    {
        // Reading was successful
        std::cout << "___working___ " << std::endl;
        std::cout << "pointer_To_Health value: " << pointer_To_Health << std::endl;
    }
    else
    {
        // An error occurred
        std::cout << "___error___ " << std::endl;
        std::cout << "pointer_To_Health value: " << pointer_To_Health << std::endl;
    }

    if (success == NULL or success == FALSE) { return 1; } //if the pointer health is not working it will then stop the programm
    pointer_To_Health += 0xF8; // pointer + offset = health offset


    DWORD health_Value = 0;
    BOOL success2 = ReadProcessMemory(assault_Cube_process, (void*)pointer_To_Health, &health_Value, 4, &bytes_read); // reading pointer to health to get the value give for the address
    if (success2)
    {
        // Reading was successful
        std::cout << "Health value: " << health_Value << std::endl;
    }
    else
    {
        // An error occurred
        std::cout << "Error reading process memory: pointer_To_Health = " << health_Value << std::endl;
    }

    if (success2 == NULL or success2 == FALSE) { return 1; } //if the health is not readable it will then stop the programm

    //WRITE PROCESS MEMORY
    DWORD new_Health_Value = 555;
    SIZE_T bytes_Written = 0;

    BOOL success3 = WriteProcessMemory(assault_Cube_process, (void*)pointer_To_Health, &new_Health_Value, 4, &bytes_Written);
    if (success3)
    {
        // Writing was successful
        std::cout << "Health value changed to: " << new_Health_Value << std::endl;
    }
    else
    {
        // An error occurred
        std::cout << "Error writing process memory Failed: " << std::endl;
    }

    if (success3 == NULL or success3 == FALSE) { return 1; } //if the health cant be changed the programm will return

    while (TRUE) //  set the health to 555 each 50 ms
    {
        WriteProcessMemory(assault_Cube_process, (void*)pointer_To_Health, &new_Health_Value, 4, &bytes_Written);
        Sleep(50);
    }

    return 0;
}