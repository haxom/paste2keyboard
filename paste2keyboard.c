#define WINVER 0x0500
#include <windows.h>
#include <stdio.h>
void pressKey(int);

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage : \n\t$ %s <filename>", argv[0]);
	}

	FILE* source = fopen(argv[1], "r");
	if(source == NULL)
	{
		printf("Bad filename !\n");
		return 1;
	}

	int current = 0;

	// we are just waiting for seconds
	Sleep(5000);

	while(current = fgetc(source), current != EOF)
		pressKey(current);

	fclose(source);

	return 0;
}

void pressKey(int c)
{
	char Buff[120] = {0};
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILANGUAGE, Buff, sizeof(Buff));
	HKL hKeyboardLayout = LoadKeyboardLayout( Buff, KLF_ACTIVATE );

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = 0; // Virtual-Key Code not used

	const SHORT Vk = VkKeyScanEx(c, hKeyboardLayout);
	const UINT VKey = MapVirtualKey(LOBYTE(Vk), 0);

	// Need to press shift key
	if(HIBYTE(Vk) == 1)
	{
		ip.ki.dwFlags = KEYEVENTF_SCANCODE;
		ip.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
		SendInput(1, &ip, sizeof(INPUT));
	}

	ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	ip.ki.wScan = VKey;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));


	// Need to release shift key
	if(HIBYTE(Vk) == 1)
	{
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		ip.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
		SendInput(1, &ip, sizeof(INPUT));
	}

	if(hKeyboardLayout)
		UnloadKeyboardLayout(hKeyboardLayout);
}
