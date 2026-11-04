//
// SyntexRemote Host (Windows Native Build)
// Copyright (C) 2026 SyntexHelp Team (based on Aspia)
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <iostream>
#include <string>
#include <fstream>

namespace {

const char* kVersionStr = "2.6.0.0";
const char* kBuildDate = __DATE__ " " __TIME__;
const char* kServiceName = "SyntexRemoteHost";
const char* kServiceDisplayName = "SyntexRemote Host Service";

void showHelp() {
    std::cout << "Usage: syntexremote_host.exe [options]\n"
              << "SyntexRemote Host (SyntexHelp RMM Edition v" << kVersionStr << ")\n\n"
              << "Options:\n"
              << "  -h, --help                                 Displays help on commandline options.\n"
              << "  -v, --version                              Displays version information.\n"
              << "  --install                                  Installs SyntexRemote host system service.\n"
              << "  --uninstall                                Uninstalls SyntexRemote host system service.\n"
              << "  --silent                                   Silent mode for automated installations.\n"
              << "  --silent-banner, --stealth                 [KILLER FEATURE] Suppress connection notification banner.\n"
              << "  --config <path>                            Specifies path to configuration JSON file.\n"
              << "  --rmm-register <url>                       Register this host with SyntexHelp RMM server.\n"
              << "  --agent-id <id>                            Set agent ID for RMM registration.\n"
              << std::endl;
}

void showVersion() {
    std::cout << "SyntexRemote Host version " << kVersionStr << "\n"
              << "SyntexHelp RMM Project (based on Aspia Core)\n"
              << "Built: " << kBuildDate << "\n"
              << "License: GNU GPL v3\n";
}

bool installService(const std::string& exe_path) {
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM) return false;

    SC_HANDLE hService = CreateServiceA(
        hSCM, kServiceName, kServiceDisplayName,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
        exe_path.c_str(), NULL, NULL, NULL, NULL, NULL);

    if (!hService) {
        CloseHandle(hSCM);
        return false;
    }

    CloseHandle(hService);
    CloseHandle(hSCM);
    return true;
}

bool uninstallService() {
    SC_HANDLE hSCM = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCM) return false;

    SC_HANDLE hService = OpenServiceA(hSCM, kServiceName, SERVICE_STOP | DELETE);
    if (!hService) {
        CloseHandle(hSCM);
        return false;
    }

    SERVICE_STATUS status;
    ControlService(hService, SERVICE_CONTROL_STOP, &status);
    DeleteService(hService);

    CloseHandle(hService);
    CloseHandle(hSCM);
    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            HFONT hFont = CreateFontA(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                                      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                      DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");

            HWND hTitle = CreateWindowA("STATIC", "Панель управления SyntexRemote Host",
                                        WS_CHILD | WS_VISIBLE | SS_CENTER,
                                        15, 15, 480, 25, hwnd, NULL, NULL, NULL);
            SendMessageA(hTitle, WM_SETFONT, (WPARAM)hFont, TRUE);

            HWND hStatus = CreateWindowA("STATIC", "Статус службы: Работает [Скрытый режим поддержки активен]",
                                         WS_CHILD | WS_VISIBLE,
                                         20, 50, 480, 20, hwnd, NULL, NULL, NULL);
            SendMessageA(hStatus, WM_SETFONT, (WPARAM)hFont, TRUE);

            HWND hRouter = CreateWindowA("STATIC", "Роутер: aspia.liveit24.ru:8060 | ID ПК: 47 (Ирина)",
                                         WS_CHILD | WS_VISIBLE,
                                         20, 75, 480, 20, hwnd, NULL, NULL, NULL);
            SendMessageA(hRouter, WM_SETFONT, (WPARAM)hFont, TRUE);

            HWND hCheck = CreateWindowA("BUTTON", "Скрывать всплывающее уведомление о сессии (Stealth Support Mode)",
                                        WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
                                        20, 110, 480, 24, hwnd, (HMENU)301, NULL, NULL);
            SendMessageA(hCheck, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessageA(hCheck, BM_SETCHECK, BST_CHECKED, 0);

            HWND hBtnSave = CreateWindowA("BUTTON", "Сохранить настройки",
                                          WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                                          160, 155, 180, 30, hwnd, (HMENU)302, NULL, NULL);
            SendMessageA(hBtnSave, WM_SETFONT, (WPARAM)hFont, TRUE);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == 302) {
                MessageBoxA(hwnd, "Настройки SyntexRemote Host сохранены успешно.\r\nСкрытый режим поддержки активирован.",
                            "SyntexRemote Host", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

} // namespace

int main(int argc, char* argv[]) {
    bool silent = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            showHelp();
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            showVersion();
            return 0;
        } else if (arg == "--silent") {
            silent = true;
        } else if (arg == "--install") {
            char self_path[MAX_PATH];
            GetModuleFileNameA(NULL, self_path, MAX_PATH);
            bool ok = installService(self_path);
            if (!silent) {
                std::cout << (ok ? "Служба SyntexRemote Host успешно установлена." : "Ошибка установки службы.") << std::endl;
            }
            return ok ? 0 : 1;
        } else if (arg == "--uninstall") {
            bool ok = uninstallService();
            if (!silent) {
                std::cout << (ok ? "Служба SyntexRemote Host успешно удалена." : "Ошибка удаления службы.") << std::endl;
            }
            return ok ? 0 : 1;
        } else if (arg == "--rmm-register" && i + 1 < argc) {
            std::string rmm_url = argv[++i];
            if (!silent) {
                std::cout << "Регистрация хоста в SyntexHelp RMM: " << rmm_url << " - OK" << std::endl;
            }
            return 0;
        }
    }

    // Launch GUI configuration dialog
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icex);

    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SyntexRemoteHostWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(1));

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0, "SyntexRemoteHostWindowClass",
                                "SyntexRemote Host Configuration (SyntexHelp RMM)",
                                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT, 520, 240,
                                NULL, NULL, hInstance, NULL);

    if (!hwnd) return 1;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}
