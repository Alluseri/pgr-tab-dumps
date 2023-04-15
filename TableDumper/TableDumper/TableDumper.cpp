#include "Internal.hpp"
#include "Hooks.hpp"

HMODULE GameAssemblyModule = NULL;
uint64_t GameAssembly = 0;

const LPCWSTR ConWatermark = L"TableDumper for PGR";

HMODULE WaitUntilLoadedA(StringA ModuleName) {
	printf("Seeking for handle to %s...\n", ModuleName);
	
	HMODULE Mod = GetModuleHandleA(ModuleName);
	while (Mod == NULL) {
		Sleep(1000);
		Mod = GetModuleHandleA(ModuleName);
	}

	printf("Found %s handle: 0x%08llX.\n", ModuleName, (uint64_t) Mod);

	return Mod;
}
void AquaThread() {
	AllocConsole();
	freopen_s((FILE**) stdout, "CONOUT$", "w", stdout);
	SetConsoleTitle(ConWatermark);

	printf("");

	printf("[0/2] Waiting for necessary modules\n");

	GameAssembly = (uint64_t) (GameAssemblyModule = WaitUntilLoadedA("GameAssembly.dll"));

	printf("[1/2] Hooking necessary functions\n");

	SetupHooks();

	printf("[2/2] Enjoy!\n");
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) AquaThread, NULL, 0, NULL)) return FALSE;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}
	return TRUE;
}