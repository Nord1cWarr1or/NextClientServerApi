#include "utilfuncs.h"
#include <fstream>
#include <string>
#include <vector>

namespace utils
{
    CBasePlayer* PlayerByIndex(int playerIndex)
    {
        return (CBasePlayer*) GET_PRIVATE(INDEXENT(playerIndex));
    }

    int RegUserMsgSafe(const char* pszName, int iSize)
    {
        int messageid = GET_USER_MSG_ID(&Plugin_info, pszName, NULL);

        if (messageid == 0)
            messageid = REG_USER_MSG(pszName, iSize);

        if (messageid == 0)
            MF_Log("Unable to register %s message", pszName);

        return messageid;
    }

    using std::ifstream;

    size_t FileSize(const std::string& path)
    {
        ifstream file(path, ifstream::binary);
        file.seekg(0, ifstream::end);
        size_t size = file.tellg();
        if (size == -1)
            return 0;

        return size;
    }

    bool CRC_File(const std::string& path, CRC32_t* crc)
    {
        ifstream file(path, ifstream::binary);

        CRC32_INIT(crc);

        char buffer[1024];
        long long readed;

        while (file.good())
        {
            readed = file.rdbuf()->sgetn(buffer, 1024);
            if (readed != 1024)
                file.setstate(ifstream::eofbit | ifstream::failbit);
            else
                file.setstate(ifstream::goodbit);

            CRC32_PROCESS_BUFFER(crc, buffer, readed);
        }

        *crc = CRC32_FINAL(*crc);

        return !file.fail();
    }

    funchook_t* CreateAndEnableHook(void* target_func, void* hook_func, void** trampoline_func)
    {
        funchook_t* hook = funchook_create();
        if (hook == nullptr)
        {
            MF_Log("CHealthNext::SetupMessagesHooks: funchook_create returns null.");
            return nullptr;
        }

        void* target_func_internal = target_func;
        int status = funchook_prepare(hook, &target_func_internal, hook_func);
        if (status != FUNCHOOK_ERROR_SUCCESS)
        {
            MF_Log("CreateAndEnableHook: funchook_prepare returns error status %d.", status);
            funchook_destroy(hook);
            return nullptr;
        }
        *trampoline_func = target_func_internal;

        status = funchook_install(hook, 0);
        if (status != FUNCHOOK_ERROR_SUCCESS)
        {
            MF_Log("CreateAndEnableHook: funchook_install returns error status %d.", status);
            funchook_destroy(hook);
            return nullptr;
        }

        return hook;
    }
}
