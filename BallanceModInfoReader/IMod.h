#pragma once

#include "BuildVer.h"

#define DECLARE_BML_VERSION \
virtual BMLVersion GetBMLVersion() override { return { BML_MAJOR_VER, BML_MINOR_VER, BML_BUILD_VER }; }

struct BMLVersion {
	BMLVersion() : major(BML_MAJOR_VER), minor(BML_MINOR_VER), build(BML_BUILD_VER) {}
	BMLVersion(int mj, int mn, int bd) : major(mj), minor(mn), build(bd) {}
	int major, minor, build;
	bool operator <(const BMLVersion& o) {
		if (major == o.major) {
			if (minor == o.minor)
				return build < o.build;
			return minor < o.minor;
		}
		return major < o.major;
	}

	bool operator >=(const BMLVersion& o) {
		return !(*this < o);
	}
};

class IMod {
public:
	virtual ~IMod();

	virtual const char* GetID() = 0;
	virtual const char* GetVersion() = 0;
	virtual const char* GetName() = 0;
	virtual const char* GetAuthor() = 0;
	virtual const char* GetDescription() = 0;
	virtual BMLVersion GetBMLVersion() = 0;

private:
};