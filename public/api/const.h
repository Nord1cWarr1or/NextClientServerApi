#pragma once

#define clc_stringcmd			3
#define SVC_NCL_MESSAGE			28
#define NCLM_C2S_HEADER			0x6D6C636E // nclm

/*
	byte		Message header
	string		Client version in SemVer notation 
	string		Prefered RSA public key version
*/
#define NCLM_C2S_VERIFICATION_REQUEST	0x01

/*
	byte		Message header
	256 bytes	Decrypted message payload
*/
#define NCLM_C2S_VERIFICATION_RESPONSE	0x02


/*
	byte		Message header
	256 bytes	Encrypted message payload
*/
#define NCLM_S2C_VERIFICATION_PAYLOAD	0x01