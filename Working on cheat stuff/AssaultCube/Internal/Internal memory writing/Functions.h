void MsgBox(DWORD addr)
{
	char szBuffer[1024];
	sprintf(szBuffer, "Address: %02x", addr);
	MessageBox(NULL, szBuffer, "Found", MB_OK);
}

void PlaceJMP(BYTE* address, DWORD jumpTo, DWORD lenght)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	VirtualProtect(address, lenght, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	dwRelAddr = (DWORD)(jumpTo - (DWORD)address) - 5;

	*address = 0xE9;

	*((DWORD*)(address + 0x1)) = dwRelAddr;

	for (DWORD x = 0x5; x < lenght; x++)
		*(address + x) = 0x90;

	VirtualProtect(address, lenght, dwOldProtect, &dwBkup);
}

MODULEINFO GetModuleInfo(char* szModule)
{
	MODULEINFO modInfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);

	if (hModule == 0)
		return modInfo;

	GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));

	return modInfo;
}

DWORD FindPatterns(char* module, char* pattern, char* mask)
{
	MODULEINFO mInfo = GetModuleInfo(module);

	DWORD base = (DWORD)mInfo.lpBaseOfDll;
	DWORD size = (DWORD)mInfo.SizeOfImage;

	DWORD patternLength = (DWORD)strlen(mask);

	for (DWORD i = 0; i < size - patternLength; i++)
	{
		bool found = true;

		for (DWORD j = 0; j < patternLength; j++)
		{
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}