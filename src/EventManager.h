#pragma once

#include <vector>
#include <memory>
#include "events.h"

class EventManager : public IEventPlayerPostThink,
                     public IEventClientDisconnect,
                     public IEventClientConnect,
                     public IEventServerActivated,
                     public IEventHandleNCLMessage,
                     public IEventMessageBeginPost,
                     public IEventMessageEndPost
{
    std::vector<std::shared_ptr<IEventPlayerPostThink>> player_post_think_listeners_;
    std::vector<std::shared_ptr<IEventClientDisconnect>> client_disconnect_listeners_;
    std::vector<std::shared_ptr<IEventClientConnect>> client_connect_listeners_;
    std::vector<std::shared_ptr<IEventServerActivated>> server_activated_listeners_;
    std::vector<std::shared_ptr<IEventHandleNCLMessage>> handle_ncl_message_listeners_;
    std::vector<std::shared_ptr<IEventMessageBeginPost>> message_begin_post_listeners_;
    std::vector<std::shared_ptr<IEventMessageEndPost>> message_end_post_listeners_;

public:
    template<typename T>
    void AddEventListener(std::shared_ptr<T> listener)
    {
        if constexpr (std::is_base_of_v<IEventPlayerPostThink, T>) player_post_think_listeners_.push_back(listener);
        if constexpr (std::is_base_of_v<IEventClientDisconnect, T>) client_disconnect_listeners_.push_back(listener);
        if constexpr (std::is_base_of_v<IEventClientConnect, T>) client_connect_listeners_.push_back(listener);
        if constexpr (std::is_base_of_v<IEventServerActivated, T>) server_activated_listeners_.push_back(listener);
        if constexpr (std::is_base_of_v<IEventHandleNCLMessage, T>) handle_ncl_message_listeners_.push_back(listener);
        if constexpr (std::is_base_of_v<IEventMessageBeginPost, T>) message_begin_post_listeners_.push_back(listener);
        if constexpr (std::is_base_of_v<IEventMessageEndPost, T>) message_end_post_listeners_.push_back(listener);
    }

    void OnPlayerPostThink(int client) override;
    void OnClientDisconnect(int client) override;
    void OnClientConnect(int client) override;
    void OnServerActivated(edict_t* pEdictList, int edictCount, int clientMax) override;
    void OnHandleNCLMessage(edict_t* client, NCLM_C2S opcode) override;
    void OnMessageBeginPost(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed) override;
    void OnMessageEndPost() override;
};
