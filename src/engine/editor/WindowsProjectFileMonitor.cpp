#include "engine/editor/WindowsProjectFileMonitor.hpp"
#include "engine/util/Print.hpp"
#include <stdio.h>
#include <Windows.h>
#include <codecvt>
#include <filesystem>

WindowsProjectFileMonitor *WindowsProjectFileMonitor::instance = nullptr;

WindowsProjectFileMonitor::WindowsProjectFileMonitor( const std::string &projectRootPath)
    : projectRootPath(projectRootPath) {
    Init(projectRootPath);
}

WindowsProjectFileMonitor::WindowsProjectFileMonitor() {}

WindowsProjectFileMonitor *WindowsProjectFileMonitor::Get() {
    if (instance == nullptr) {
        instance = new WindowsProjectFileMonitor();
    }
    return instance;
}

void WindowsProjectFileMonitor::Update() {
    if (INVALID_HANDLE_VALUE == hFile)
        return;

    char buffer[1024];
    DWORD bytesReturned;
    OVERLAPPED overlapped = { 0 };

    while (threadRunning){
        DWORD bytesReturned;
        char buffer[4096]; // Buffer to store directory change notifications

        // 同步读取目录变化
        if (ReadDirectoryChangesW(
                hFile,
                buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_ATTRIBUTES |
                    FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_LAST_ACCESS |
                    FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SECURITY,
                &bytesReturned,
                NULL,
                NULL
                )) {
            // 处理目录变化通知
            FILE_NOTIFY_INFORMATION* pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buffer);
            std::string fileName;
            std::wstring fileName_(pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR));
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            fileName = converter.to_bytes(fileName_);
            int action = pNotify->Action;

            onChange(fileName, action);

        } else {
            util::Println( "ERROR:Reading directory changes failed.");
        }

        // 暂停一段时间，以免CPU占用过高
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    CloseHandle(hFile);
}
void WindowsProjectFileMonitor::SetOnChangeCallback(
    std::function<void(const std::string &, int)> function) {
    onChange = function;
}


void WindowsProjectFileMonitor::Init(const std::string &projectRootPath_) {
    std::filesystem::path curath = std::filesystem::current_path();
    std::filesystem::path abPath = curath / projectRootPath_;
    const char *pszDirectory = projectRootPath_.c_str();
    projectRootPath = projectRootPath_;
    hFile = CreateFile(pszDirectory,
                       FILE_LIST_DIRECTORY,
                       FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                       NULL,
                       OPEN_EXISTING,
                       FILE_FLAG_BACKUP_SEMANTICS,
                       NULL);

    threadRunning = true;
    monitorThread = std::thread(&WindowsProjectFileMonitor::Update, this);
}


void WindowsProjectFileMonitor::Stop() {
    threadRunning = false;
}
