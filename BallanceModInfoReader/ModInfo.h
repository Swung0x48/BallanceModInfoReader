#pragma once
#include <string>
struct ModInfo
{
	std::string ID;
	std::string Version;
	std::string Name;
	std::string Author;
	std::string Description;
	int BMLMajor = 0;
	int BMLMinor = 0;
	int BMLBuild = 0;
};

