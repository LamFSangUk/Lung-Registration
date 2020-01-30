#include "FileHelper.h"
#include <Windows.h>
#include <filesystem>
#include <shlwapi.h>  
#pragma comment(lib, "shlwapi.lib")



std::string FileHelper::FileToString(const std::wstring & directory) {
    std::ifstream in(directory, std::ios::in | std::ios::ate);
    std::string str;

    str.reserve(in.tellg());
    in.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
    return str;
}

std::pair<char*, uint32_t> FileHelper::FileToBuffer(const std::wstring& directory) {
    std::ifstream in(directory, std::ios::in | std::ios::ate | std::ios::binary);
    // get its size
    std::streampos fileSize = in.tellg();
    in.seekg(0, std::ios::beg);
    if (!in) {
        throw std::runtime_error("File " + std::string(directory.begin(), directory.end()) + " not exists");
    }
	char* buffer(new char[(uint32_t)fileSize]);
    in.read(buffer, fileSize);
    return std::make_pair(std::move(buffer), (uint32_t)fileSize);
}

std::wstring FileHelper::GetExecutablePath(void) {
    wchar_t ownPth[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL) {
        // Use GetModuleFileName() with module handle to get the path
        GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
        PathRemoveFileSpec(ownPth);
    }
    else {
        throw std::runtime_error("Not able to get module handle");
    }
    return std::wstring(ownPth) + L"\\";
}

// opens file dialog and get filename 
std::wstring FileHelper::OpenLoadDialog(void) {
    wchar_t currentdir[MAX_PATH];
    GetCurrentDirectory(sizeof(currentdir), currentdir);

    std::string filename;
    OPENFILENAME ofn;
    char szFile[MAX_PATH];       // buffer for file name
    //const int bufferSize = 100;
    //char buffer[bufferSize];
    /// open a file name
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = (LPWSTR)szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = currentdir;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    GetOpenFileName(&ofn);
    SetCurrentDirectory(currentdir);

    return ofn.lpstrFile;
}

std::wstring FileHelper::OpenSaveDialog(void) {
    wchar_t currentdir[MAX_PATH];
    GetCurrentDirectory(sizeof(currentdir), currentdir);

    std::string filename;
    OPENFILENAME ofn;
    char szFile[MAX_PATH];       // buffer for file name
    //const int bufferSize = 100;
    //char buffer[bufferSize];
    /// open a file name
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = (LPWSTR)szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    GetSaveFileName(&ofn);
    SetCurrentDirectory(currentdir);

    return ofn.lpstrFile;
}