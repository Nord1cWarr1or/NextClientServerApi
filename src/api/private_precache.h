#pragma once

#include "events.h"
#include "main.h"

#include <string>
#include <unordered_map>

#define SVC_STUFFTEXT		9

class CPrivatePrecache : public IPrivatePrecache,
						 public IEventClientConnect {
	std::string filepathResourceListAbsolute;
	std::string filepathResourceListRelative;
	std::string payloadResourceListLocation;
	std::unordered_map<std::string, std::string> mapResourceList;
	bool isResourceListWritten = false;

	bool WriteResourceListToDisk();
	bool AppendResource(std::string filepath, std::string nclFilepath, bool replace);

public:
	CPrivatePrecache();

	int PrecacheModel(std::string filepath, std::string nclFilepath);
	int PrecacheSound(std::string filepath, std::string nclFilepath);
	void PrecacheClientOnly(std::string filepath, std::string nclFilepath);

	void OnClientConnect(int client);
};