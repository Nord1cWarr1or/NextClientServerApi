#pragma once

#include <string>

class IPrivatePrecache {
public:
	~IPrivatePrecache() { }

	virtual int PrecacheModel(int client, std::string filepath, std::string nclFilepath) = 0;
	virtual int PrecacheSound(int client, std::string filepath, std::string nclFilepath) = 0;
	virtual int PrecacheClientOnly(int client, std::string filepath, std::string nclFilepath) = 0;
};