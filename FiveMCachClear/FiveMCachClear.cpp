#include <iostream>
#include <windows.h>
#include <urlmon.h>
#include <string>
#include <fstream>



std::wstring getLocalAppDataPath() {
    wchar_t* localAppData;
    size_t len;
    _wdupenv_s(&localAppData, &len, L"LOCALAPPDATA");
    std::wstring path(localAppData);
    free(localAppData);
    return path;
}

void showMessage(const std::wstring& title, const std::wstring& message, UINT type) {
    MessageBox(NULL, message.c_str(), title.c_str(), type);
}

void deleteFiles(const std::wstring& path) {
    std::wstring searchPath = path + L"\\*";
    WIN32_FIND_DATAW fdFile;
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &fdFile);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (wcscmp(fdFile.cFileName, L".")!= 0 && wcscmp(fdFile.cFileName, L"..")!= 0) {
            std::wstring fullPath = path + L"\\" + fdFile.cFileName;

            if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                deleteFiles(fullPath);
                RemoveDirectoryW(fullPath.c_str());
            }
            else {
                DeleteFileW(fullPath.c_str());
            }
        }
    } while (FindNextFileW(hFind, &fdFile));

    FindClose(hFind);
}

void handleUpdate() {
    std::wstring localAppData = getLocalAppDataPath();
    std::wstring cachePath = localAppData + L"\\FiveM\\FiveM.app\\data\\cache";
    const std::wstring version = L"1.0.1.0";
    std::wstring versionFile = localAppData + L"\\version.txt";

    HRESULT hr = URLDownloadToFile(NULL, L"https://raw.githubusercontent.com/xProxyRed/FiveM-Cache-Clear/main/version", versionFile.c_str(), 0, NULL);
    if (hr!= S_OK) {
        showMessage(L"Fehler", L"Fehler beim Herunterladen der Datei.", MB_OK | MB_ICONERROR);
        return;
    }

    std::wifstream file(versionFile);
    if (!file.is_open()) {
        showMessage(L"Fehler", L"Fehler beim Lesen der Version.", MB_OK | MB_ICONERROR);
        DeleteFileW(versionFile.c_str()); // Lösche die temporäre Datei im Fehlerfall
        return;
    }

    std::wstring remoteVersion;
    std::getline(file, remoteVersion);
    file.close();
    DeleteFileW(versionFile.c_str()); // Lösche die temporäre Datei nach dem Lesen der Version

    if (remoteVersion!= version) {
        int result = MessageBox(NULL, (L"Eine neue Version (" + remoteVersion + L") ist bereit. Willst du sie jetzt herunterladen?").c_str(), L"Aktualisierung", MB_YESNO | MB_ICONQUESTION);
        if (result == IDYES) {
            ShellExecute(NULL, L"open", L"https://github.com/xProxyRed/FiveM-Cache-Clear/releases", NULL, NULL, SW_SHOWNORMAL);
        }
        else {
            result = MessageBox(NULL, (L"Sind Sie sicher, dass Sie alle Dateien entfernen wollen? '" + cachePath + L"'").c_str(), L"Sicher?", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES) {
                deleteFiles(cachePath);
                showMessage(L"OK", L"Die Dateien wurden entfernt.", MB_OK | MB_ICONINFORMATION);
            }
            else {
                showMessage(L"Abgebrochen", L"Das entfernen der Dateien wurde abgebrochen.", MB_OK | MB_ICONINFORMATION);
            }
        }
    }
    else {
        int result = MessageBox(NULL, (L"Sind Sie sicher, dass Sie alle Dateien entfernen wollen? '" + cachePath + L"'").c_str(), L"Sicher?", MB_YESNO | MB_ICONQUESTION);
        if (result == IDYES) {
            deleteFiles(cachePath);
            showMessage(L"OK", L"Die Dateien wurden entfernt.", MB_OK | MB_ICONINFORMATION);
        }
        else {
            showMessage(L"Abgebrochen", L"Das entfernen der Datei wurde abgebrochen.", MB_OK | MB_ICONINFORMATION);
        }
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    handleUpdate();
    return 0;
}