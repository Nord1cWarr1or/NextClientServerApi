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
	void DeleteResourceListFromDisk();
	bool AppendResource(const std::string& filepath, const std::string& nclFilepath, bool replace);

public:
	CPrivatePrecache();

	int PrecacheModel(const std::string& filepath, const std::string& nclFilepath) override;
	int PrecacheSound(const std::string& filepath, const std::string& nclFilepath) override;
	bool PrecacheClientOnly(const std::string& filepath, const std::string& nclFilepath) override;

	void OnClientConnect(int client) override;
};