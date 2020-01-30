#include <Windows.h>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <memory>

#pragma once
namespace FileHelper {
    // returns buffer and its length
    std::string FileToString(const std::wstring& directory);
    std::pair<char*, uint32_t> FileToBuffer(const std::wstring& directory);
    std::wstring GetExecutablePath(void);
    std::wstring OpenLoadDialog(void);
    std::wstring OpenSaveDialog(void);
};
