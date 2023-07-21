#include "EventManager.h"

void EventManager::OnPlayerPostThink(int client)
{
    for (const auto& listener : player_post_think_listeners_)
        listener->OnPlayerPostThink(client);
}

void EventManager::OnClientDisconnect(int client)
{
    for (const auto& listener : client_disconnect_listeners_)
        listener->OnClientDisconnect(client);
}

void EventManager::OnClientConnect(int client)
{
    for (const auto& listener : client_connect_listeners_)
        listener->OnClientConnect(client);
}

void EventManager::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    for (const auto& listener : server_activated_listeners_)
        listener->OnServerActivated(pEdictList, edictCount, clientMax);
}

void EventManager::OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode)
{
    for (const auto& listener : handle_ncl_message_listeners_)
        listener->OnHandleNCLMessage(client, opcode);
}

void EventManager::OnMessageBeginPost(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed)
{
    for (const auto& listener : message_begin_post_listeners_)
        listener->OnMessageBeginPost(msg_dest, msg_type, pOrigin, ed);
}

void EventManager::OnMessageEndPost()
{
    for (const auto& listener : message_end_post_listeners_)
        listener->OnMessageEndPost();
}
