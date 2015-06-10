
#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "user32.lib")

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
    )
{
    wchar_t fileName[MAX_PATH];
    DWORD dwError;
    wchar_t errorMsg[MAX_PATH];
    wchar_t* errorTitle = L"Redirector Error";

    DWORD fileNameLen = GetModuleFileName(NULL, fileName, MAX_PATH);
    if (fileNameLen == 0)
    {
        dwError = GetLastError();
        swprintf_s(errorMsg, MAX_PATH, L"%s:0x%x", L"GetModuleFileName failed", dwError);
        
        MessageBox(NULL, errorMsg, L"Redirector Error", MB_OK|MB_ICONERROR);
        return 0;
    }

    wchar_t* configFileName = fileName;
    wcscat_s(configFileName, MAX_PATH, L".config");


    FILE* config = _wfopen(configFileName, L"r");
    if (config == NULL)
    {
        dwError = GetLastError();
        swprintf_s(errorMsg, MAX_PATH, L"%s:0x%x", L"Open config file failed", dwError);
        
        MessageBox(NULL, errorMsg, L"Redirector Error", MB_OK|MB_ICONERROR);
        
        return 0;
    }

    wchar_t configStr[MAX_PATH];
    if (fgetws(configStr, MAX_PATH, config) == NULL)
    {
        dwError = GetLastError();

        return 0;
    }

    // only the first line is used
    wchar_t* firstLineEnd = wcschr(configStr, L'\n');
    if (firstLineEnd != NULL)
    {
        *firstLineEnd = L'\0';
    }

    wchar_t wzCmdLine[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, lpCmdLine, -1, wzCmdLine, MAX_PATH);

    wcscat(configStr, L" ");
    wcscat(configStr, wzCmdLine);

    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    BOOL success = CreateProcess(NULL,
                                 configStr,
                                 NULL,
                                 NULL,
                                 FALSE,
                                 0,
                                 NULL,
                                 NULL,
                                 &si,
                                 &pi);

    if (!success)
    {
        dwError = GetLastError();

        swprintf_s(errorMsg, MAX_PATH, L"%s:0x%x", L"Create new process failed", dwError);
        
        MessageBox(NULL, errorMsg, L"Redirector Error", MB_OK|MB_ICONERROR);
        
        return 0;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
