#include "Hooks.hpp"
#include <filesystem>

FnTableManagerLoad OgTableManagerLoad;

void CreateDirectories(const wchar_t* path) {
	std::wstring dirPath(path);
	size_t pos = dirPath.find_first_of(L"/\\");
	while (pos != std::wstring::npos) {
		std::wstring subDir = dirPath.substr(0, pos);
		if (CreateDirectoryW(subDir.c_str(), NULL) == 0 && GetLastError() != ERROR_ALREADY_EXISTS) {
			std::wcout << L"Failed to create directory: " << subDir << std::endl;
			DebugBreak();
			return;
		}
		pos = dirPath.find_first_of(L"/\\", pos + 1);
	}
}

Il2CppString* __fastcall TableManagerLoadHook(Il2CppString* Path) {
	Il2CppString* Content = OgTableManagerLoad(Path);
	
	wchar_t SavePath[MAX_PATH] = L"./TableDumped/";
	wchar_t* ResPath = Path->Read();
	wcscat(SavePath, ResPath);

	printf("XTableManager.Load: '%ls'\n", ResPath);

	if (GetFileAttributesW(SavePath) != INVALID_FILE_ATTRIBUTES) {
		printf("Already exists, skipping: '%ls'\n", SavePath);
		return Content;
	}

	CreateDirectories(SavePath);

	HANDLE Hdl = CreateFileW(SavePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, 1, 0x80, NULL);
	if (Hdl == NULL) {
		DebugBreak();
		return Content;
	}
	uint32_t Dw = 0;
	wchar_t* ContentString = Content->Read();

	if (!WriteFile(Hdl, R(void*, ContentString), Content->Length * sizeof(wchar_t), (LPDWORD)&Dw, NULL)) {
		DebugBreak();
		return Content;
	}
	printf("Saved '%ls'(%d bytes) to '%ls'\n", Path->Read(), Dw, SavePath);
	return Content;
}

void SetupHooks() {
	MH_Initialize();

	MH_CreateHook((void*)(GameAssembly + Offsets::TableManagerLoad), TableManagerLoadHook, ORIGINAL(OgTableManagerLoad));

	MH_EnableHook(MH_ALL_HOOKS);
}