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
	v.get<picojson::object>()["status"] = picojson::value(-1.0);
	v.get<picojson::object>()["message"] = picojson::value("Internal Error");
	
	if (argc != 2) {
		v.get<picojson::object>()["status"] = picojson::value((double)ERROR_FILE_NOT_FOUND);
		v.get<picojson::object>()["message"] = picojson::value("Usage: BallanceModInfoReader.exe <path to .bmod>");
		std::cout << v.serialize();
		return 0;
	}

	//std::cout << argv[1] << std::endl;
	HMODULE handle = LoadLibrary("BML.dll");
	if (!handle) {
		v.get<picojson::object>()["status"] = picojson::value((double)GetLastError());
		v.get<picojson::object>()["message"] = picojson::value("BML stub load failed.");
		std::cout << v.serialize();
		return 0;
	}
	
	InfoFunc get_info = reinterpret_cast<InfoFunc>(GetProcAddress(handle, "GetModInfo"));
	ModInfo* mod_instance = get_info(argv[1]);
	if (!mod_instance) {
		v.get<picojson::object>()["status"] = picojson::value((double)GetLastError());
		v.get<picojson::object>()["message"] = picojson::value("Bmod load failed.");
		std::cout << v.serialize();
		return 0;
	}
	picojson::value& mod = v.get<picojson::object>()["mod"];
	mod.set<picojson::object>(picojson::object());
	mod.get<picojson::object>()["id"] = picojson::value(mod_instance->ID);
	mod.get<picojson::object>()["name"] = picojson::value(mod_instance->Name);
	mod.get<picojson::object>()["version"] = picojson::value(mod_instance->Version);
	mod.get<picojson::object>()["description"] = picojson::value(mod_instance->Description);
	mod.get<picojson::object>()["author"] = picojson::value(mod_instance->Author);
	picojson::value& bml = mod.get<picojson::object>()["bml"];
	bml.set<picojson::object>(picojson::object());
	bml.get<picojson::object>()["major"] = picojson::value((double)mod_instance->BMLMajor);
	bml.get<picojson::object>()["minor"] = picojson::value((double)mod_instance->BMLMinor);
	bml.get<picojson::object>()["build"] = picojson::value((double)mod_instance->BMLBuild);
	v.get<picojson::object>()["status"] = picojson::value(0.0);
	v.get<picojson::object>()["message"] = picojson::value("OK");
	//picojson::value& mod = v.get<picojson::object>()["mod"];
	//mod.set<picojson::object>(picojson::object());
	//mod.get<picojson::object>()["test"] = picojson::value("a");
	std::cout << v.serialize();
}