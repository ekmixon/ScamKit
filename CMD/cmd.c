#include <stdio.h>
#include <Windows.h>

#pragma warning (disable : 4996)

#define IDVERSION_MAJOR	0
#define IDVERSION_MINOR	1
#define IDVERSION_BUILD	2
#define IDVERSION_UBR	3

#define NQUERY_KEYS		4

typedef struct _WinVerInfo {
	DWORD majorVer;
	DWORD minorVer;
	unsigned char currentBuild[16];
	DWORD UBR;
} WinVerInfo;

int ReadVersionInfo(WinVerInfo *wvInfo) {
	if (wvInfo == NULL) { return EXIT_FAILURE; }
	HKEY hKey;
	DWORD readSize = 0xf;
	const char* sPath = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
	const char *sKeys[NQUERY_KEYS] = {
		"CurrentMajorVersionNumber",
		"CurrentMinorVersionNumber",
		"CurrentBuild",
		"UBR"
	};

	LSTATUS nResult = RegOpenKeyExA(HKEY_LOCAL_MACHINE, sPath, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
	
	DWORD regsz = REG_SZ;
	DWORD regdw = REG_DWORD;
	if (nResult != EXIT_SUCCESS) { return EXIT_FAILURE; }

	for (int i = 0; i < NQUERY_KEYS; i++) {
		switch (i) {
		
		case IDVERSION_MAJOR:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_MAJOR], NULL, &regdw, (LPBYTE) &wvInfo->majorVer, &readSize);
			break;
		
		case IDVERSION_MINOR:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_MINOR], NULL, &regdw, (LPBYTE) &wvInfo->minorVer, &readSize);
			break;

		case IDVERSION_BUILD:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_BUILD], NULL, &regsz, (LPBYTE)& wvInfo->currentBuild, &readSize);
			break;

		case IDVERSION_UBR:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_UBR], NULL, &regdw, (LPBYTE) &wvInfo->UBR, &readSize);
			break;

		default:
			break;
		}

		if (nResult != ERROR_SUCCESS) {
			RegCloseKey(hKey);
			return EXIT_FAILURE;
		}
		readSize = 0xf;
	}

	RegCloseKey(hKey);
	return EXIT_SUCCESS;
}

int main(void) {
	/* Get Windows Version */
	WinVerInfo wvInfo;
	
	/* Get Username */
	DWORD wunamelen = 64;
	unsigned char wusername[64];
	
	if (ReadVersionInfo(&wvInfo) == EXIT_SUCCESS && GetUserNameA(wusername, &wunamelen) == TRUE) {
		char tmp[64] = { 0 };
		strncpy(tmp, wusername, 63);
		snprintf(wusername, 63, "C:\\Users\\%s> ", tmp);
		printf("Microsoft Windows [Version %lu.%lu.%s.%lu]\n(c) Microsoft Corporation. All rights reserved.\n\n%s", wvInfo.majorVer, wvInfo.minorVer, wvInfo.currentBuild, wvInfo.UBR, wusername);
		fflush(stdout);
	}
	else {
		printf("Microsoft Windows [Version 10.0.19041.1052]\n(c) Microsoft Corporation. All rights reserved.\n\nC:\\Users\\homepc> ");
		strncpy(wusername, "C:\\Users\\homepc> ", 63);
		fflush(stdout);
	}

	unsigned char inputBuffer[256];
	while (1) {
		memset(inputBuffer, 0, 256);
		fgets(inputBuffer, 255, stdin);

		char* nlptr = strrchr(inputBuffer, '\n');
		if (nlptr != NULL) { *nlptr = 0; }

		if (strncmp(inputBuffer, "exit", 255) == 0) { break; }
		printf("'%s' is not recognized as an internal or external command,\noperable program or batch file.\n\n%s", inputBuffer, wusername);
		fflush(stdout);
	}


	//while (1) {

	//}

	return EXIT_SUCCESS;
}