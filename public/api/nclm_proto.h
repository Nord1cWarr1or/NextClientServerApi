#pragma once

#define clc_ncl_message			6			// clc_tmove
#define SVC_NCL_MESSAGE			57			// SVC_SENDCVARVALUE
#define NCLM_C2S_HEADER			0x6D6C636E 	// nclm
#define NCLM_VERIF_PAYLOAD_SIZE	196

enum NCLM_C2S {
	/*
		byte		Message header
		string		Client version in SemVer notation 
		string		Prefered RSA public key version
	*/
	VERIFICATION_REQUEST = 0x01,

	/*
		byte		Message header
		196 bytes	Decrypted message payload
	*/
	VERIFICATION_RESPONSE
};

enum NCLM_S2C {
	/*
		byte		Message header
	*/
	IS_SERVER_SUPPORT_NEXTCLIENT = 0x01,

	/*
		byte		Message header
		196 bytes	Encrypted message payload
	*/
	VERIFICATION_PAYLOAD
};