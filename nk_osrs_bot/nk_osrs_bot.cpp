#include <windows.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

// Prototypes //////////////////////////////////////////////////////////////////
void                printMenu();
std::string      GetTimestamp();
LRESULT CALLBACK    MouseProc(int nCode, WPARAM wParam, LPARAM lParam);


// Globals /////////////////////////////////////////////////////////////////////
HHOOK         mouseHook;
std::ofstream logFile("clicks.txt");

////////////////////////////////////////////////////////////////////////////////
int main()
{
    printMenu();

    // Setup the mouse hook
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0);
    if (mouseHook == nullptr)
    {
        std::cerr << "Failed to install mouse hook!" << std::endl;
        return 1;
    }

    std::cout << "Recording clicks... Press Ctrl+C to stop.\n";

    // Message loop to keep the hook active
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook the mouse hook and close the log file
    UnhookWindowsHookEx(mouseHook);
    logFile.close();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
void printMenu()
{
    std::cout << "Nicola + Keith's ORSR Bot\n";
    std::cout << "[R]ecord\n";
    std::cout << "[P]lay\n";
    std::cout << "[S]ettings\n";
    std::cout << ">> ";

    std::string input;
    std::cin >> input;
}

////////////////////////////////////////////////////////////////////////////////
std::string GetTimestamp() 
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();
    long long duration = value.count();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    struct tm timeInfo;
    char buffer[100];

    localtime_s(&timeInfo, &now_time);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

    sprintf_s(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), ".%03ld", duration % 1000);

    return std::string(buffer);
}

////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        MSLLHOOKSTRUCT* mouseStruct = (MSLLHOOKSTRUCT*)lParam;
        if (mouseStruct != nullptr)
        {
            std::string timestamp = GetTimestamp();
            switch (wParam)
            {
            case WM_LBUTTONDOWN:
                logFile << timestamp << ": Left Down "   << mouseStruct->pt.x << "x " << mouseStruct->pt.y << "y" << std::endl;
                break;
            case WM_LBUTTONUP:
                logFile << timestamp << ": Left Up "     << mouseStruct->pt.x << "x " << mouseStruct->pt.y << "y" << std::endl;
                break;
            case WM_RBUTTONDOWN:
                logFile << timestamp << ": Right Down "  << mouseStruct->pt.x << "x " << mouseStruct->pt.y << "y" << std::endl;
                break;
            case WM_RBUTTONUP:
                logFile << timestamp << ": Right Up "    << mouseStruct->pt.x << "x " << mouseStruct->pt.y << "y" << std::endl;
                break;
            case WM_MBUTTONDOWN:
                logFile << timestamp << ": Middle Down " << mouseStruct->pt.x << "x " << mouseStruct->pt.y << "y" << std::endl;
                break;
            case WM_MBUTTONUP:
                logFile << timestamp << ": Middle Up "   << mouseStruct->pt.x << "x " << mouseStruct->pt.y << "y" << std::endl;
                break;
            }
        }
    }
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}