#include "private_precache.h"

#include <fstream>
#include <cstdio>

CPrivatePrecache::CPrivatePrecache() {
	filepathResourceListRelative = std::string(MF_GetLocalInfo("amxx_datadir", "addons/amxmodx/data")) + "/ncl_private_precache.txt";
	payloadResourceListLocation = std::string("\x02ncl\x07") + filepathResourceListRelative;
	filepathResourceListAbsolute = MF_BuildPathname(filepathResourceListRelative.c_str());

	DeleteResourceListFromDisk();
}

int CPrivatePrecache::PrecacheModel(const std::string& filepath, const std::string& nclFilepath) {
	bool result = AppendResource(filepath, nclFilepath, true);
	if (!result)
		return 0;

	return PRECACHE_MODEL(filepath.c_str());
}

int CPrivatePrecache::PrecacheSound(const std::string& filepath, const std::string& nclFilepath) {
	bool result = AppendResource("sound/" + filepath, "sound/" + nclFilepath, true);
	if (!result)
		return 0;

	return PRECACHE_SOUND(filepath.c_str());
}

bool CPrivatePrecache::PrecacheClientOnly(const std::string& filepath, const std::string& nclFilepath) {
	return AppendResource(filepath, nclFilepath, false);
}

void CPrivatePrecache::OnClientConnect(int client) {
	if (!isResourceListWritten) {
		WriteResourceListToDisk();
		isResourceListWritten = true;
	}

	if (NAPI()->GetNextClientVersion(client) < NextClientVersion::V_2_1_8)
		return;

	MESSAGE_BEGIN(MSG_ONE, SVC_STUFFTEXT, NULL, INDEXENT(client));
	WRITE_STRING(payloadResourceListLocation.c_str());
	MESSAGE_END();
}

bool CPrivatePrecache::AppendResource(const std::string& filepath, const std::string& nclFilepath, bool replace) {
	if (mapResourceList.count(filepath) != 0)
		return false;

	std::string filepathAbsolute = MF_BuildPathname(nclFilepath.c_str());
	size_t filesize = utils::FileSize(filepathAbsolute);
	if (filesize == 0)
		return false;

	CRC32_t checksum = 0;
	utils::CRC_File(filepathAbsolute, &checksum);
	if (checksum == 0)
		return false;

	char buffer[256];
	sprintf(buffer, "%d:%s:%s:%x:%d", replace, filepath.c_str(), nclFilepath.c_str(), checksum, filesize);

	mapResourceList[filepath] = buffer;

	return true;
}

bool CPrivatePrecache::WriteResourceListToDisk() {
	if (mapResourceList.empty())
		return false;

	std::ofstream file(filepathResourceListAbsolute);
	if (!file.is_open())
		return false;

	for (const auto& entry : mapResourceList) {
		file << entry.second << std::endl;
	}

	return true;
}

void CPrivatePrecache::DeleteResourceListFromDisk() {
	std::remove(filepathResourceListAbsolute.c_str());
	std::remove((filepathResourceListAbsolute + ".ztmp").c_str());
}