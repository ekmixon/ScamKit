#include <stdio.h>
#include <Windows.h>

BOOL FileExists(char* szPath) {
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int replace_sysexe(const char* srcPath, const char* destPath, const char *bckpPath) {
	const char *ptr = NULL;
	if (FileExists(destPath)) {
		/* Backup */
		if (bckpPath != NULL) {
			if (!MoveFileA(destPath, bckpPath)) {
				fprintf(stderr, "Unable to move '%s' to '%s' - E:%d\n", destPath, bckpPath, GetLastError());
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

	printf("[+] %s '%s'\n", msg, ptr);

	return EXIT_SUCCESS;
}

int main(int argc, const char **argv) {
	int nFakeItems = 4;

	const char* fakeExes[] = {
		"FakeCMD.exe",
		"FakeTREE.exe",
		"FakeSYSKEY.exe",
		"FakeNETSTAT.exe"
	};
	
	const char* backupPaths[] = {
		"C:\\Windows\\System32\\bxcmd.exe",
		"C:\\Windows\\System32\\bxtree.com",
		"C:\\Windows\\System32\\bxsyskey.exe",
		"C:\\Windows\\System32\\BXNETSTAT.exe"
	};

	const char* destPaths[] = {
		"C:\\Windows\\System32\\cmd.exe",
		"C:\\Windows\\System32\\tree.com",
		"C:\\Windows\\System32\\bxsyskey.exe",
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


	if (argc > 2) {
		if (strncmp(argv[1], "--restore", 0xf) == 0) {
			printf("[+] Restoring System Executables..\n");
			destPtr = destPaths;
			fakePtr = backupPaths;
			bckpPtr = nullarray;
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
		if (FileExists(fakePtr[i])) {
			replace_sysexe(fakePtr[i], destPtr[i], bckpPtr[i]);
		} else {
			printf("[+] Missing '%s'. Omitting..\n", fakePtr[i]);
		}
	}

	return EXIT_SUCCESS;
}