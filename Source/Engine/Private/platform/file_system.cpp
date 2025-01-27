#define _CRT_SECURE_NO_WARNINGS
#include <engine/platform/file_system.h>
#include <engine/platform/platform_id.h>
#include <engine/platform/project_system.h>

#ifdef _WIN32
constexpr size_t MAX_PATH = 260;
constexpr int CSIDL_LOCAL_APPDATA = 0x001c;
typedef long HRESULT;
typedef struct HWND__ *HWND;
typedef void *HANDLE;
typedef unsigned long DWORD;
typedef wchar_t *LPWSTR;
typedef struct HINSTANCE__ *HMODULE;
extern "C" __declspec(dllimport) HRESULT __stdcall SHGetFolderPathW(HWND hwnd, int csidl, HANDLE hToken, DWORD dwFlags, LPWSTR pszPath);
extern "C" __declspec(dllimport) DWORD __stdcall GetModuleFileNameW(HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
#endif

const std::fs::path &engine::get_write_directory_path() {
    static std::fs::path s_writeDirectory{};
    static bool s_pathFound = false;
    if (!s_pathFound) {
        auto &projectName = project_system().get_project_name();
        std::string projectSubdir{};
        projectSubdir.reserve(projectName.size());
        for (auto c : projectName) {
            if (std::isalnum(c)) {
                projectSubdir.push_back(c);
            }
            if (c == '_' || c == ' ' || c == '-') {
                projectSubdir.push_back('_');
            }
        }
        if (projectSubdir.empty()) {
            projectSubdir = std::format("{:016X}", std::hash<std::fs::path>{}(get_current_binary_folder()));
        }
        if constexpr (currentPlatform == EPlatformId::Windows) {
            wchar_t buffer[MAX_PATH];
            if (SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, buffer) == 0) {
                s_pathFound = true;
                s_writeDirectory = buffer;
                s_writeDirectory /= "game_engine";
                s_writeDirectory /= projectSubdir;
                std::fs::create_directories(s_writeDirectory);
            } else {
                abort();
            }
        } else if constexpr (currentPlatform == EPlatformId::Linux) {
            s_writeDirectory = getenv("HOME");
            s_writeDirectory /= "game_engine";
            s_writeDirectory /= projectSubdir;
            std::fs::create_directories(s_writeDirectory);
            s_pathFound = true;
        }
    }
    return s_writeDirectory;
}

const std::fs::path &engine::get_current_binary_folder() {
    static std::fs::path s_binaryFolder{};
    static bool s_pathFound = false;
    if (!s_pathFound) {
        if constexpr (currentPlatform == EPlatformId::Windows) {
            std::wstring buffer{};
            buffer.resize(MAX_PATH);
            buffer.resize(GetModuleFileNameW(nullptr, buffer.data(), buffer.size()));
            if (buffer.size() > MAX_PATH) {
                GetModuleFileNameW(nullptr, buffer.data(), buffer.size());
            }
            if (buffer.empty()) {
                abort();
            }
            s_binaryFolder = buffer;
            s_binaryFolder = s_binaryFolder.parent_path();
            s_pathFound = true;
        } else if constexpr (currentPlatform == EPlatformId::Linux) {
            s_binaryFolder = std::fs::canonical("/proc/self/exe");
            s_binaryFolder = s_binaryFolder.parent_path();
            s_pathFound = true;
        }
    }
    return s_binaryFolder;
}
