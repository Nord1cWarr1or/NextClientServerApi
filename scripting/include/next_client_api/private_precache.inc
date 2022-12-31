
#define PRECACHE_MAX_PATH 250
#define CRC32_MAX_STRING 10
#define FILESIZE_STR_MAX 9
#define DATA_ROW_SIZE ((PRECACHE_MAX_PATH + 1) * 2 + CRC32_MAX_STRING + 1 + FILESIZE_STR_MAX + 1 + 5)

static Trie:g_PrivateResources = Invalid_Trie;
static g_ResListWritten = false;
static g_ListLocationFile[190];
static g_ListLocationMsg[197] = { 2, 'n', 'c', 'l', 7 };

private_precache_init()
{
    register_native("ncl_precache_model", "native_ncl_precache_model");
    register_native("ncl_precache_sound", "native_ncl_precache_sound");
    register_native("ncl_precache_client_only", "native_ncl_precache_client_only");

    g_PrivateResources = TrieCreate();
    delete_private_resource_list();

    // make list path
    get_datadir(g_ListLocationFile, charsmax(g_ListLocationFile));
    add(g_ListLocationFile, charsmax(g_ListLocationFile), "/ncl_private_precache.txt");

    // make msg
    new len = copy(g_ListLocationMsg[5], charsmax(g_ListLocationMsg), g_ListLocationFile);
    g_ListLocationMsg[len + 5] = 7;
}

private_precache_send_res_location(id)
{
    if (TrieGetSize(g_PrivateResources) == 0)
        return;

    if (!g_ResListWritten)
    {
        g_ResListWritten = write_private_resources_list();

        if (!g_ResListWritten)
            return;
    }

    engfunc(EngFunc_MessageBegin, MSG_ONE, SVC_STUFFTEXT, Float:{0.0,0.0,0.0}, id);
    write_string(g_ListLocationMsg);
    message_end();
}

public native_ncl_precache_model(plugin_id, argc)
{
    new filepath[PRECACHE_MAX_PATH + 1];
    get_string(1, filepath, charsmax(filepath));

    new ncl_filepath[PRECACHE_MAX_PATH + 1];
    get_string(2, ncl_filepath, charsmax(ncl_filepath));

    add_private_resource(filepath, ncl_filepath, true);

    return precache_model(filepath);
}

public native_ncl_precache_sound(plugin_id, argc)
{
    new filepath[PRECACHE_MAX_PATH + 1];
    get_string(1, filepath, charsmax(filepath));

    new ncl_filepath[PRECACHE_MAX_PATH + 1];
    get_string(2, ncl_filepath, charsmax(ncl_filepath));

    add_private_resource(filepath, ncl_filepath, true);

    return precache_sound(filepath);
}

public native_ncl_precache_client_only(plugin_id, argc)
{
    new save_as_filepath[PRECACHE_MAX_PATH + 1];
    get_string(1, save_as_filepath, charsmax(save_as_filepath));

    new ncl_filepath[PRECACHE_MAX_PATH + 1];
    get_string(2, ncl_filepath, charsmax(ncl_filepath));

    add_private_resource(save_as_filepath, ncl_filepath, false);
}

static add_private_resource(const filepath[], const ncl_filepath[], bool:replace)
{
    if (TrieKeyExists(g_PrivateResources, filepath))
        return;

    new size = filesize(ncl_filepath);
    if (size == -1)
        log_error(AMX_ERR_GENERAL, "Can't find file '%s' (is a replacement for '%s')", ncl_filepath, filepath);
    
    new crc32[CRC32_MAX_STRING + 1];
    hash_file(ncl_filepath, Hash_Crc32, crc32, charsmax(crc32));

    new message[DATA_ROW_SIZE];
    formatex(message, charsmax(message), "%d:%s:%s:%s:%d^n", replace, filepath, ncl_filepath, crc32, size);

    TrieSetString(g_PrivateResources, filepath, message);
}

static bool:write_private_resources_list()
{
    new file = fopen(g_ListLocationFile, "w");
    if (!file)
    {
        delete_private_resource_list();
        log_error(AMX_ERR_GENERAL, "Can't open file %s to write private resources list", g_ListLocationFile);

        return false;
    }

    new TrieIter:it = TrieIterCreate(g_PrivateResources);

    while (!TrieIterEnded(it))
    {
        new data[DATA_ROW_SIZE];
        TrieIterGetString(it, data, charsmax(data));

        fwrite_blocks(file, data, strlen(data), BLOCK_CHAR);

        TrieIterNext(it);
    }

    TrieIterDestroy(it);
    fclose(file);

    return true;
}

static delete_private_resource_list()
{
    delete_file(g_ListLocationFile);
}

#undef PRECACHE_MAX_PATH
#undef CRC32_MAX_STRING