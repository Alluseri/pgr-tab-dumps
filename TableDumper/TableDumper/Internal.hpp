#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <stdint.h>
#include <Windows.h>

#define R(Type, Subject) reinterpret_cast<Type>(Subject)

typedef LPCSTR StringA;
typedef LPCWSTR StringW;
typedef unsigned char byte;

#pragma pack(4)
class Il2CppString {
public:
	void* Object;
	uint64_t Padding;
	uint32_t Length; // The amount of letters, not bytes, minus null terminator
	wchar_t* Read() {
		return R(wchar_t*, R(byte*, this) + sizeof(Il2CppString));
	};
};

namespace Offsets {
	const uint64_t TableManagerLoad = 0x76C6F0;
}

#define ORIGINAL(FuncPtr) reinterpret_cast<void**>(&FuncPtr)

extern HMODULE GameAssemblyModule;
extern uint64_t GameAssembly;

using FnTableManagerLoad = Il2CppString*(__fastcall*)(Il2CppString* Name);