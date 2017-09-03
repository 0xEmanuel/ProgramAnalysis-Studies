/*
Emanuel Durmaz

*/


//#include "stdafx.h"
#include <windows.h>
//#pragma comment(lib, "urlmon.lib")

typedef int (__cdecl *f_printf)(char* _Format, ...);
typedef int (__cdecl *f_scanf)(char* _Format, ...);
typedef HRESULT (__stdcall *f_URLDownloadToFile)(_In_opt_ LPUNKNOWN, _In_ LPCWSTR, _In_opt_ LPCWSTR, DWORD, _In_opt_ LPBINDSTATUSCALLBACK);

f_scanf scanf_org;
f_printf printf_org;
f_URLDownloadToFile URLDownloadToFile_org;

HINSTANCE hMsvcrtDll;
HINSTANCE hUrlmonDll;

//dlls and function names
char crypted_msvcrt_dll[] = "\x4e\x62\x67\x40\x33\x35\x0d\x40\x1a\x4f";
char crypted_urlmon_dll[] = "\x56\x63\x7d\x4e\x2e\x2f\x0d\x40\x1a\x4f";
char crypted_printf[] = "\x53\x63\x78\x4d\x35\x27";
char crypted_scanf[] = "\x50\x72\x70\x4d\x27";
char crypted_URLDownloadToFileW[] = "\x76\x43\x5d\x67\x2e\x36\x4d\x48\x19\x42\x75\x45\x4c\x07\x28\x4f\x41\x21";

//menu
char crypted_taschenrechner[] = "\x77\x70\x62\x40\x29\x24\x4d\x56\x13\x40\x79\x7f\x46\x33\x61\x0e\x04\x20\x46\x63\x62\x4a\x2e\x2f\x03\x14\x0e\x60\x21\x55\x66\x05\x03\x62\x60\x7c";
char crypted_wahl[] = "\x61\x78\x65\x57\x24\x61\x54\x45\x13\x4b\x7d\x74\x4d\x61\x12\x4a\x41\x56\x42\x64\x62\x0f\x61\x36\x46\x48\x15\x4b\x74\x31\x62\x2a\x35\x4a\x4b\x18\x03\x75\x64\x51\x22\x29\x44\x41\x10\x56\x74\x79\x51\x35\x61\x54\x41\x04\x47\x74\x7f\x03\x32\x2e\x4f\x48\x4c\x29";
char crypted_add[] = "\x12\x38\x31\x62\x25\x25\x4a\x50\x1f\x4c\x7f\x1b";
char crypted_sub[] = "\x11\x38\x31\x70\x34\x23\x57\x56\x17\x48\x65\x78\x4c\x2f\x4b";
char crypted_update[] = "\x10\x38\x31\x73\x33\x2e\x44\x56\x17\x4e\x7c\x64\x53\x25\x20\x57\x41\x7c";
char crypted_ende[] = "\x17\x38\x31\x73\x33\x2e\x44\x56\x17\x4e\x7c\x31\x41\x24\x24\x4d\x40\x13\x4d\x1b";

//interaction
char crypted_dec_format[] = "\x06\x75";
char crypted_ersteZahl[] = "\x61\x78\x65\x57\x24\x61\x44\x41\x14\x46\x7f\x31\x70\x28\x24\x03\x40\x1f\x46\x31\x74\x51\x32\x35\x46\x04\x2c\x42\x79\x7d\x03\x24\x28\x4d\x1e\x7c";
char crypted_zweiteZahl[] = "\x61\x78\x65\x57\x24\x61\x44\x41\x14\x46\x7f\x31\x70\x28\x24\x03\x40\x1f\x46\x31\x6b\x54\x24\x28\x57\x41\x56\x79\x70\x79\x4f\x61\x24\x4a\x4a\x4c\x29";
char crypted_ergebnisPlus[] = "\x67\x70\x62\x03\x04\x33\x44\x41\x14\x4d\x78\x62\x03\x37\x2e\x4d\x04\x53\x47\x31\x3a\x03\x64\x25\x03\x4d\x05\x57\x31\x34\x47\x6f\x4b";
char crypted_ergebnisMinus[] = "\x67\x70\x62\x03\x04\x33\x44\x41\x14\x4d\x78\x62\x03\x37\x2e\x4d\x04\x53\x47\x31\x3c\x03\x64\x25\x03\x4d\x05\x57\x31\x34\x47\x6f\x4b";
char crypted_dErfolg[] = "\x67\x7e\x66\x4d\x2d\x2e\x42\x40\x56\x55\x7e\x7f\x03\x14\x31\x47\x45\x02\x46\x31\x74\x51\x27\x2e\x4f\x43\x04\x46\x78\x72\x4b\x6f\x4b";
char crypted_dFehler[] = "\x67\x7e\x66\x4d\x2d\x2e\x42\x40\x56\x55\x7e\x7f\x03\x14\x31\x47\x45\x02\x46\x31\x77\x46\x29\x2d\x44\x41\x05\x40\x79\x7d\x42\x26\x24\x4d\x0a\x7c";

//urls (changed them to some valid values, so you can see the downloaded file in the destination path as confirmation)
//char src[] = "https://rub.de/images/logo/logo-rub-102.gif";
char crypted_src[] = "\x4b\x65\x65\x53\x32\x7b\x0c\x0b\x04\x56\x73\x3f\x47\x24\x6e\x4a\x49\x17\x44\x74\x62\x0c\x2d\x2e\x44\x4b\x59\x4f\x7e\x76\x4c\x6c\x33\x56\x46\x5b\x12\x21\x23\x0d\x26\x28\x45";

//char dst[] = "C:/Users/syssec/Downloads/test.png";
char crypted_dst[] = "\x60\x2b\x3e\x76\x32\x24\x51\x57\x59\x50\x68\x62\x50\x24\x22\x0c\x60\x19\x54\x7f\x7d\x4c\x20\x25\x50\x0b\x02\x46\x62\x65\x0d\x31\x2f\x44";


void xor_crypt(char const key[], const int keyLen, char* text, int textLen)
{
	for (int i = 0; i<textLen; ++i)
		text[i] = (char)(key[i % keyLen] ^ text[i]);
}

int load_libs()
{
	//loads DLL into the memory of the current process
	hMsvcrtDll = LoadLibraryA(crypted_msvcrt_dll);
	hUrlmonDll = LoadLibraryA(crypted_urlmon_dll);

	if (!hMsvcrtDll)
		return EXIT_FAILURE;
	
	if (!hUrlmonDll)
		return EXIT_FAILURE;
	
	// resolve function address here
	printf_org = (f_printf)GetProcAddress(hMsvcrtDll, crypted_printf);
	if (!printf_org)
		return EXIT_FAILURE;

	scanf_org = (f_scanf)GetProcAddress(hMsvcrtDll, crypted_scanf);
	if (!scanf_org)
		return EXIT_FAILURE;
	
	URLDownloadToFile_org = (f_URLDownloadToFile)GetProcAddress(hUrlmonDll, crypted_URLDownloadToFileW);
	if (!URLDownloadToFile_org)
		return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}

void decrypt_all(char const key[], const int keyLen)
{
	int textLen;

	//dlls and function names
	textLen = sizeof(crypted_msvcrt_dll) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_msvcrt_dll, textLen);
	textLen = sizeof(crypted_urlmon_dll) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_urlmon_dll, textLen);
	textLen = sizeof(crypted_printf) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_printf, textLen);
	textLen = sizeof(crypted_scanf) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_scanf, textLen);
	textLen = sizeof(crypted_URLDownloadToFileW) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_URLDownloadToFileW, textLen);

	//menu
	textLen = sizeof(crypted_taschenrechner) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_taschenrechner, textLen);
	textLen = sizeof(crypted_wahl) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_wahl, textLen);
	textLen = sizeof(crypted_add) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_add, textLen);
	textLen = sizeof(crypted_sub) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_sub, textLen);
	textLen = sizeof(crypted_update) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_update, textLen);
	textLen = sizeof(crypted_ende) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_ende, textLen);

	//interaction
	textLen = sizeof(crypted_dec_format) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_dec_format, textLen);
	textLen = sizeof(crypted_ersteZahl) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_ersteZahl, textLen);
	textLen = sizeof(crypted_zweiteZahl) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_zweiteZahl, textLen);
	textLen = sizeof(crypted_ergebnisPlus) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_ergebnisPlus, textLen);
	textLen = sizeof(crypted_ergebnisMinus) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_ergebnisMinus, textLen);
	textLen = sizeof(crypted_dErfolg) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_dErfolg, textLen);
	textLen = sizeof(crypted_dFehler) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_dFehler, textLen);
	
	//urls
	textLen = sizeof(crypted_src) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_src, textLen);
	textLen = sizeof(crypted_dst) / sizeof(char) - 1;
	xor_crypt(key, keyLen, crypted_dst, textLen);
}


int main(void)
{
	int textLen;
	char key[] = "\x23\x11\x11\x23\x41\x41\x23\x24\x76";
	const int keyLen = sizeof(key) / sizeof(char) - 1;
	decrypt_all(key, keyLen);

	if(EXIT_FAILURE == load_libs() )
		return EXIT_FAILURE;

	int op1, op2, n;
	HRESULT res;

	printf_org(crypted_taschenrechner);
	printf_org(crypted_wahl);
	printf_org(crypted_add);
	printf_org(crypted_sub);
	printf_org(crypted_update);
	printf_org(crypted_ende);

	const int size_buf = 4096;
	wchar_t wsrc[size_buf];
	MultiByteToWideChar(CP_ACP, 0, crypted_src, -1, wsrc, size_buf);
	wchar_t wdst[size_buf];
	MultiByteToWideChar(CP_ACP, 0, crypted_dst, -1, wdst, size_buf); // dont need to load via LoadLibraryA, since its already loaded via kernel32.dll

	scanf_org(crypted_dec_format, &n);

	switch (n)
	{
	case 1:
	case 2:
		printf_org(crypted_ersteZahl);
		scanf_org(crypted_dec_format, &op1);

		printf_org(crypted_zweiteZahl);
		scanf_org(crypted_dec_format, &op2);

		if (n == 1)
			printf_org(crypted_ergebnisPlus, op1, op2, op1 + op2);		
		else
			printf_org(crypted_ergebnisMinus, op1, op2, op1 - op2);
		
		break;
	case 3:
		res = URLDownloadToFile_org(0, wsrc, wdst, 0, 0);
		if (res == S_OK)		
			printf_org(crypted_dErfolg);			
		else			
			printf_org(crypted_dFehler);
		
		break;
	case 4:
		break;
	}

	return 0;
}
