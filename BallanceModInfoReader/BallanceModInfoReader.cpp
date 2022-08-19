#include <iostream>
#include "picojson.h"
#include "ModInfo.h"
#include <Windows.h>
#include "IMod.h"

typedef void IBML;
typedef IMod* (*EntryFunc)(IBML*);
typedef int (*AddFunc)(int, int);
typedef ModInfo* (*InfoFunc)(const char* path);
int main(int argc, char** argv)
{
	ModInfo info;
	picojson::value v;
	v.set<picojson::object>(picojson::object());
	v.get<picojson::object>()["Status"] = picojson::value(-1.0);
	v.get<picojson::object>()["Message"] = picojson::value("Internal Error");
	
	if (argc != 2) {
		v.get<picojson::object>()["Status"] = picojson::value((double)ERROR_FILE_NOT_FOUND);
		v.get<picojson::object>()["Message"] = picojson::value("Usage: BallanceModInfoReader.exe <path to .bmod>");
		std::cout << v.serialize();
		return 0;
	}
	
	HMODULE handle = LoadLibrary("BML.dll");
	if (!handle) {
		v.get<picojson::object>()["Status"] = picojson::value((double)GetLastError());
		v.get<picojson::object>()["Message"] = picojson::value("BML stub load failed.");
		std::cout << v.serialize();
		return 0;
	}
	
	InfoFunc get_info = reinterpret_cast<InfoFunc>(GetProcAddress(handle, "GetModInfo"));
	ModInfo* mod_instance = get_info(argv[1]);
	if (!mod_instance) {
		v.get<picojson::object>()["Status"] = picojson::value((double)GetLastError());
		v.get<picojson::object>()["Message"] = picojson::value("Bmod load failed.");
		std::cout << v.serialize();
		return 0;
	}
	picojson::value& mod = v.get<picojson::object>()["Mod"];
	mod.set<picojson::object>(picojson::object());
	mod.get<picojson::object>()["Id"] = picojson::value(mod_instance->ID);
	mod.get<picojson::object>()["Name"] = picojson::value(mod_instance->Name);
	mod.get<picojson::object>()["Version"] = picojson::value(mod_instance->Version);
	mod.get<picojson::object>()["Description"] = picojson::value(mod_instance->Description);
	mod.get<picojson::object>()["Author"] = picojson::value(mod_instance->Author);
	picojson::value& bml = mod.get<picojson::object>()["Bml"];
	bml.set<picojson::object>(picojson::object());
	bml.get<picojson::object>()["Major"] = picojson::value((double)mod_instance->BMLMajor);
	bml.get<picojson::object>()["Minor"] = picojson::value((double)mod_instance->BMLMinor);
	bml.get<picojson::object>()["Build"] = picojson::value((double)mod_instance->BMLBuild);
	v.get<picojson::object>()["Status"] = picojson::value(0.0);
	v.get<picojson::object>()["Message"] = picojson::value("OK");
	std::cout << v.serialize();
}