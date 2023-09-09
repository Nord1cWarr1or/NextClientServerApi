#pragma once

#include "nclm_proto.h"
#include "ICvarSandbox.h"
#include "IPrivatePrecache.h"
#include "IViewmodelFX.h"
#include "IDeprecatedAPI.h"

#define BIT(n)  (1<<(n))

enum class NextClientUsing {
    NOT_USING,
    DECLARE_USING,
    USING_VERIFICATED
};

enum NextClientFeatures {
    FEATURE_CVARS_SANDBOX = BIT(0),
    FEATURE_VIEWMODEL_FX = BIT(1),
    FEATURE_PRIVATE_PRECACHE = BIT(2),
    FEATURE_VERIFICATION = BIT(3)
};

struct NextClientVersion {
    size_t major;
    size_t minor;
    size_t patch;

    NextClientVersion(size_t maj = 0, size_t min = 0, size_t pat = 0) {
        major = maj;
        minor = min;
        patch = pat;
    }

    const inline bool operator>(const NextClientVersion& other) const {
        return major > other.major && minor > other.minor && patch > other.patch;
    }

    const inline bool operator>=(const NextClientVersion& other) const {
        return *this > other || *this == other;
    }

    const inline bool operator<(const NextClientVersion& other) const {
        return major < other.major && minor < other.minor && patch < other.patch;
    }

    const inline bool operator<=(const NextClientVersion& other) const {
        return *this < other || *this == other;
    }

    const inline bool operator==(const NextClientVersion& other) const {
        return major == other.major && minor == other.minor && patch == other.patch;
    }
};

class INextClientAPI : public ncl_deprecated::INextClientAPI {
public:
	virtual ~INextClientAPI() = default;

    virtual IViewmodelFX *ViewmodelFX() = 0;

    virtual IPrivatePrecache *PrivatePrecache() = 0;

    virtual ICvarSandbox *CvarSandbox() = 0;

    virtual bool ClientIsReady(int client) = 0;

    virtual void ClientSetFOV(int client, int fov, float lerpTime) = 0;

    virtual NextClientUsing ClientIsUsingNextClient(int client) = 0;
    virtual bool GetNextClientVersion(int client, NextClientVersion& version) = 0;
    virtual int GetSupportedFeatures(int client) = 0;

    virtual void SendHudSprite(
        int client,
        int channel,
        const char *spritePath,
        const uint8_t priteColor[3],
        uint8_t alpha,
        int frame,
        float frameRate,
        float inTime,
        float holdTime,
        float outTime,
        float x,
        float y,
        const int spriteRect[4],
        float scaleX,
        float scaleY,
        int renderMode
    ) = 0;

    virtual void SendHudSpriteFullScreen(
        int client,
        int channel,
        const char *spritePath,
        const uint8_t spriteColor[3],
        uint8_t alpha,
        int frame,
        float frameRate,
        float inTime,
        float holdTime,
        float outTime,
        int renderMode
    ) = 0;

    virtual void ClearHudSprite(int client, int channel) = 0;
};
