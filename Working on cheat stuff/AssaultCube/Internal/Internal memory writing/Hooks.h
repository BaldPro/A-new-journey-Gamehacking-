DWORD AmmoJmpBack = 0;
DWORD AmmoValue = 0;
__declspec(naked) void InfiniteAmmo()
{
	if (option::InfiniteAmmo == true)
	{
		__asm INC[ESI]
	}
	else {
		__asm DEC[ESI]
	}
	//__asm INC [ESI]
	__asm push edi
	__asm mov edi, [esp + 0x14]
	__asm jmp[AmmoJmpBack]
}

DWORD mouseOnPlayerJmpBack = 0;
DWORD currentMouseOnPlayerValue = 0;
DWORD defaultCall = 0x004607C0;
INPUT input = { 0 };

__declspec(naked) void Triggerbot()
{
	__asm {
		call defaultCall
		pushad
		mov currentMouseOnPlayerValue, eax
	}

	//When the plr is on enemy with mouse: Shoot
	if (currentMouseOnPlayerValue != 0 && option::Triggerbot == true) {
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
	}
	else {
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	_asm {
		popad
		jmp mouseOnPlayerJmpBack
	}
}