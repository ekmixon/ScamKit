#include <stdio.h>
#include <Windows.h>

#pragma warning (disable : 4996)

#define IDVERSION_MAJOR	0
#define IDVERSION_MINOR	1
#define IDVERSION_BUILD	2
#define IDVERSION_UBR	3

#define NQUERY_KEYS		4

typedef struct _WinVerInfo {
	unsigned char majorVer[16];
	unsigned char minorVer[16];
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

	if (nResult != EXIT_SUCCESS) { return EXIT_FAILURE; }

	for (int i = 0; i < NQUERY_KEYS; i++) {
		switch (i) {
		
		case IDVERSION_MAJOR:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_MAJOR], NULL, NULL, (LPBYTE) wvInfo->majorVer, &readSize);
			break;
		
		case IDVERSION_MINOR:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_MINOR], NULL, NULL, (LPBYTE) wvInfo->minorVer, &readSize);
			break;

		case IDVERSION_BUILD:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_BUILD], NULL, NULL, (LPBYTE) wvInfo->currentBuild, &readSize);
			break;

		case IDVERSION_UBR:
			nResult = RegQueryValueExA(hKey, sKeys[IDVERSION_UBR], NULL, NULL, (LPBYTE) wvInfo->UBR, &readSize);
			break;

		default:
			break;
		}

		if (nResult != EXIT_SUCCESS) { 
			RegCloseKey(hKey);
			return EXIT_FAILURE;
		}
	}
	
	RegCloseKey(hKey);
	return EXIT_SUCCESS;
}

int main(void) {
	WinVerInfo wvInfo;


	ReadVersionInfo(&wvInfo);

	printf("%s.%s.%s.%lu\n", wvInfo.majorVer, wvInfo.minorVer, wvInfo.currentBuild, wvInfo.UBR);


	/* Get Windows Version */
	//while (1) {

	//}


	return EXIT_SUCCESS;
}