#include "impl.h"

CNextClientApi::CNextClientApi() {
}

IViewmodelFX* CNextClientApi::ViewmodelFX() {
	return nullptr;
}

IPrivatePrecache* CNextClientApi::PrivatePrecache() {
	return nullptr;
}

ICvarSandbox* CNextClientApi::CvarSandbox() {
	return nullptr;
}

bool CNextClientApi::ClientIsReady(int client) {
	return false;
}

bool CNextClientApi::ClientIsNextClient(int client) {
	return false;
}

void CNextClientApi::ClientSetFOV(int client, int fov, float lerpTime) {
}

INextClientAPI* NAPI() {
	static CNextClientApi impl;
	return &impl;
}