#include "private_precache.h"

#include <fstream>
#include <stdio.h>

CPrivatePrecache::CPrivatePrecache() {
	this->filepathResourceListRelative = std::string(MF_GetLocalInfo("amxx_datadir", "addons/amxmodx/data"))
		+ "/ncl_private_precache.txt";

	this->payloadResourceListLocation = std::string("\x02ncl\x07")
		+ this->filepathResourceListRelative;

	this->filepathResourceListAbsolute = MF_BuildPathname(this->filepathResourceListRelative.c_str());
}

int CPrivatePrecache::PrecacheModel(std::string filepath, std::string nclFilepath) {
	this->AppendResource(filepath, nclFilepath, true);
	return PRECACHE_MODEL(filepath.c_str());
}

int CPrivatePrecache::PrecacheSound(std::string filepath, std::string nclFilepath) {
	this->AppendResource(filepath, nclFilepath, true);
	return PRECACHE_SOUND(filepath.c_str());
}

void CPrivatePrecache::PrecacheClientOnly(std::string filepath, std::string nclFilepath) {
	this->AppendResource(filepath, nclFilepath, false);
}

void CPrivatePrecache::OnClientConnect(int client) {
	if (!this->isResourceListWritten) {
		this->WriteResourceListToDisk();
	}

	// Add nextclient v2.1.8+ check

	MESSAGE_BEGIN(MSG_ONE, SVC_STUFFTEXT, NULL, INDEXENT(client));
	WRITE_STRING(this->payloadResourceListLocation.c_str());
	MESSAGE_END();
}

bool CPrivatePrecache::AppendResource(std::string filepath, std::string nclFilepath, bool replace) {
	if (this->mapResourceList.count(filepath) != 0)
		return false;

	std::string filepathAbsolute = MF_BuildPathname(nclFilepath.c_str());
	size_t filesize = utils::FileSize(filepathAbsolute);
	CRC32_t checksum = 0;
	utils::CRC_File(filepathAbsolute, &checksum);

	char buffer[128];
	sprintf_s(buffer, "%d:%s:%s:%x:%d",
		replace, filepath.c_str(), nclFilepath.c_str(), checksum, filesize);

	this->mapResourceList[filepath] = buffer;

	return true;
}

bool CPrivatePrecache::WriteResourceListToDisk() {
	if (!this->mapResourceList.size())
		return false;

	std::ofstream file(this->filepathResourceListAbsolute);
	if (!file.is_open())
		return false;

	for (auto entry : this->mapResourceList) {
		file << entry.second << std::endl;
	}

	file.close();
	this->isResourceListWritten = true;

	return true;
}