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

void EventManager::OnClientFirstFrame(int client)
{
    for (const auto& listener : client_first_frame_listeners_)
        listener->OnClientFirstFrame(client);
}

void EventManager::OnClientPutInServer(edict_t* pEntity)
{
    for (const auto& listener : client_putinserver_listeners_)
        listener->OnClientPutInServer(pEntity);
}

void EventManager::OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax)
{
    for (const auto& listener : server_activated_listeners_)
        listener->OnServerActivated(pEdictList, edictCount, clientMax);
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

void EventManager::OnClientVerificated(edict_t* client, std::string clientVersion, std::string rsaKeyVersion) {
    for (const auto& listener : client_verificated_listeners_)
        listener->OnClientVerificated(client, clientVersion, rsaKeyVersion);
}

void EventManager::OnNclmVerificationRequest(edict_t* client, std::string clientVersion, std::string rsaKeyVersion) 
{
    for (const auto& listener : nclm_verification_request_listeners_)
        listener->OnNclmVerificationRequest(client, clientVersion, rsaKeyVersion);
}

void EventManager::OnNclmVerificationResponse(edict_t* client, std::vector<uint8_t> payload) 
{
    for (const auto& listener : nclm_verification_response_listeners_)
        listener->OnNclmVerificationResponse(client, payload);
}