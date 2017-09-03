// Emanuel Durmaz

/*
Steps:
1. Rename the original crypt.dll to original_crypt.dll
2. Compile this as crypt.dll
3. Make sure that the binary is able to find the dll's

*/

#include <iostream>
#include <windows.h>

using namespace std;

extern "C" // avoid function name decoration
{
	__declspec(dllexport) int init(void);
	__declspec(dllexport) char* crypt(char*);
}

typedef char* (__cdecl *f_crypt)(char* arg);

f_crypt crypt_original;
HINSTANCE hCryptDll;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

int init()
{
	//loads the DLL into the memory of the current process
	hCryptDll = LoadLibraryA("original_crypt.dll");

	if (!hCryptDll)
	{
		cout << "could not load the dynamic library" << endl;
		return EXIT_FAILURE;
	}

	// resolve function address here
	crypt_original = (f_crypt)GetProcAddress(hCryptDll, "crypt");
	if (!crypt_original)
	{
		cout << "could not locate the function" << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

char* crypt(char* arg)
{
	if(EXIT_FAILURE == init() )
		exit(EXIT_FAILURE);

	cout << "Argument for original crypt(): " << arg << endl;
	char* solution = crypt_original(arg);
	cout << "Solution from original crypt() " << solution << endl;
	return solution;
}

