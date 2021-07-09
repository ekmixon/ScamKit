#include <stdio.h>
#include <Windows.h>

BOOL FileExists(const char* szPath) {
	DWORD dwAttrib = GetFileAttributesA(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

char* GetErrorMessage(DWORD dwErrorCode, LPSTR pBuffer, DWORD cchBufferLength) {
	char* ptr = NULL;
	char buffer[4096];
	if (pBuffer == NULL) {
		ptr = buffer;
		cchBufferLength = 4095;
	} else {
		ptr = pBuffer;
	}
	if (cchBufferLength == 0) {	return NULL; }

	DWORD cchMsg = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), ptr, cchBufferLength, NULL);
	return ptr;
}

int replace_sysexe(const char* srcPath, const char* destPath, const char *bckpPath) {
	const char *ptr = NULL;
	if (FileExists(destPath)) {
		/* Backup */
		if (bckpPath != NULL) {
			printf("[+] Backing up '%s' to '%s'\n", destPath, bckpPath);
			if (!MoveFileA(destPath, bckpPath)) {
				fprintf(stderr, "Unable to backup '%s' to '%s' - E:%d\n", destPath, bckpPath, GetLastError());
				return EXIT_FAILURE;
			}
		}
	}
	
	if (!MoveFileA(srcPath, destPath)) {
		fprintf(stderr, "Unable to move '%s' to '%s' - E:%d\n", srcPath, destPath, GetLastError());
		return EXIT_FAILURE;
	}
		
	ptr = strrchr(destPath, '\\');
	if (ptr == NULL) {
		return EXIT_SUCCESS;
	}

	char* msg = bckpPath == NULL ? "Restored" : "Fake";

	printf("[+] %s '%s'\n", msg, ptr + 1);

	return EXIT_SUCCESS;
}

int main(int argc, const char **argv) {
	int nFakeItems = 4;

	unsigned char cwdir[MAX_PATH - 16];
	char **fakeExes = malloc(sizeof(char*) * 4);

	if (fakeExes == NULL) {
		fprintf(stderr, "Unable to allocate memory - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
		return EXIT_FAILURE;
	
	}
	{
		const char *tmpExes[] = {
			"FakeCMD.exe",
			"FakeTREE.com",
			"FakeSYSKEY.exe",
			"FakeNETSTAT.exe"
		};
		
		if (GetCurrentDirectoryA(MAX_PATH - 17, cwdir) == 0) {
			fprintf(stderr, "[-] Error reading current directory: %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
			return EXIT_FAILURE;
		}

		for (int i = 0; i < 4; i++) {
			fakeExes[i] = malloc(sizeof(char*) * MAX_PATH);
			if (fakeExes[i] == NULL) {
				fprintf(stderr, "Unable to allocate memory - %s\n", GetErrorMessage(GetLastError(), NULL, 0xfff));
				return EXIT_FAILURE;
			}
			snprintf(fakeExes[i], MAX_PATH - 1, "%s\\%s", cwdir, tmpExes[i]);
		}
	}
	
	const char* backupPaths[] = {
		"C:\\Windows\\System32\\bxcmd.exe",
		"C:\\Windows\\System32\\bxtree.com",
		"C:\\Windows\\System32\\bxsyskey.exe",
		"C:\\Windows\\System32\\BXNETSTAT.exe"
	};

	const char* destPaths[] = {
		"C:\\Windows\\System32\\cmd.exe",
		"C:\\Windows\\System32\\tree.com",
		"C:\\Windows\\System32\\syskey.exe",
		"C:\\Windows\\System32\\NETSTAT.exe"
	};

	const char* nullarray[] = {
		NULL,
		NULL,
		NULL,
		NULL
	};
	
	const char **fakePtr = NULL;
	const char **destPtr = NULL;
	const char **bckpPtr = NULL;
	
	int restore = 0;
	if (argc > 1) {
		if (strncmp(argv[1], "--restore", 0xf) == 0) {
			printf("[+] Restoring System Executables..\n");
			destPtr = destPaths;
			fakePtr = backupPaths;
			bckpPtr = nullarray;
			restore = 1;
		}
		else {
			fprintf(stderr, "[-] Unknown argument\n");
			return EXIT_FAILURE;
		}
	} else {
		fakePtr = fakeExes;
		destPtr = destPaths;
		bckpPtr = backupPaths;
	}

	printf("[+] Installing AntiScam Toolkit..\n");

	for (int i = 0; i < nFakeItems; i++) {
		if (restore == 1) { DeleteFileA(destPtr[i]); }
		if (FileExists(fakePtr[i])) {
			replace_sysexe(fakePtr[i], destPtr[i], bckpPtr[i]);
		} else {
			printf("[-] Missing '%s'. Omitting..\n", fakePtr[i]);
		}
	}

	return EXIT_SUCCESS;
}