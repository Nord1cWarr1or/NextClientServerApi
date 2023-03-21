#pragma once

#include "events.h"
#include "main.h"

#include <string>
#include <unordered_map>

#define SVC_STUFFTEXT		9

class CPrivatePrecache : public IPrivatePrecache,
						 public IEventClientConnect {
	std::string filepath_resource_list_absolute_;
	std::string filepath_resource_list_relative_;
	std::string payload_resource_list_location_;
	std::unordered_map<std::string, std::string> map_resource_list_;
	bool is_resource_list_written_{};

public:
	CPrivatePrecache();

	int PrecacheModel(const std::string& filepath, const std::string& nclFilepath) override;
	int PrecacheSound(const std::string& filepath, const std::string& nclFilepath) override;
	bool PrecacheClientOnly(const std::string& filepath, const std::string& nclFilepath) override;

	void OnClientConnect(int client) override;

private:
    bool WriteResourceListToDisk();
    void DeleteResourceListFromDisk();
    bool AppendResource(const std::string& filepath, const std::string& nclFilepath, bool replace);
};