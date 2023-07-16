#pragma once

#include "main.h"
#include "helpers.h"

#include <string>

constexpr auto MAX_HUD_SPRITES = 32;

void AddNatives_NextClient();
void AddNatives_CvarSandbox();
void AddNatives_PrivatePrecache();
void AddNatives_ViewmodelFX();

void AddNatives_All();
