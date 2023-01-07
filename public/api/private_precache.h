#pragma once

#include <string>

class IPrivatePrecache {
public:
	~IPrivatePrecache() { }

	virtual int PrecacheModel(std::string filepath, std::string nclFilepath) = 0;
	virtual int PrecacheSound(std::string filepath, std::string nclFilepath) = 0;
	virtual void PrecacheClientOnly(std::string filepath, std::string nclFilepath) = 0;
};