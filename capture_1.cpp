#include <windows.h>
#include <fstream>
#include <vector>
#include <string>
namespace h {
    inline auto baseStyle(WNDPROC wndproc, LPCWSTR name) {
        WNDCLASS winc;
        winc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
        winc.cbClsExtra = winc.cbWndExtra = 0;
        winc.hInstance = (HINSTANCE)GetModuleHandle(0);
        winc.hIcon = LoadIcon(nullptr,IDI_APPLICATION);
        winc.hCursor = LoadCursor(NULL, IDC_ARROW);
        winc.lpszMenuName = NULL;
        winc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        winc.lpfnWndProc = wndproc;
        winc.lpszClassName = name;
        return RegisterClass(&winc);
    }
    template <class T>
    inline auto find(std::string str, const std::string cut, T func) {
        for (auto pos = str.find(cut); pos != std::string::npos; pos = str.find(cut)) {
            func(str.substr(0, pos));
            str = str.substr(pos + cut.size());
        }
        return str;
    }
    inline auto split(std::string str, const std::string cut) noexcept(false) {
        std::vector<std::string> data;
        str = find(str, cut, [&](std::string str) {
            data.push_back(str);
            });
        if (!str.empty())data.push_back(str);
        return data;
    }
    class File {
    private:
        std::string name,
            content;
    public:
        inline File(const std::string name)noexcept(true) :name(name) {
            read();
        }
        inline auto& setName(const std::string name)noexcept(true) {
            this->name = name;
            return *this;
        }
        inline auto& getName() {
            return name;
        }
        inline auto& getContent()const noexcept(true) {
            return content;
        }
        inline File& read() noexcept(false) {
            std::fstream file(name);
            if (!file.is_open()) {
                return *this;
            }
            content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            return *this;
        }
        inline auto write(const std::string str, const bool reset = false) const noexcept(false) {
            std::ofstream file(name, reset ? std::ios_base::trunc : std::ios_base::app);
            file << str;
            file.close();
            return *this;
        }
        inline  auto& replace(const std::string data, const bool reset = false)const noexcept(false) {
            std::ofstream file(name, reset ? std::ios_base::out : std::ios_base::app);
            if (file.fail())return *this;
            file << data;
            file.close();
            return *this;
        }
    };
};
VOID CALLBACK TimerProc(HWND hwnd, UINT msg, UINT id, DWORD time) {
    SendMessage(hwnd, WM_PAINT, 0,0);
    //InvalidateRect(hwnd, nullptr, TRUE);
    //UpdateWindow(hwnd);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        SetTimer(hwnd, 4649, std::stoi("0" + h::File("speed").read().getContent().substr(0, 9)),(TIMERPROC)TimerProc);//file(setting).read
        break;
    case WM_WINDOWPOSCHANGED:
        SetWindowPos(hwnd,HWND_BOTTOM,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
        break;
    case WM_PAINT:
    {
        RECT hwndSize;
        GetClientRect(hwnd, &hwndSize);
        HDC wndHdc = GetDC(hwnd),deskHdc = GetDC(nullptr);
        StretchBlt(wndHdc, 0,0, hwndSize.right, hwndSize.bottom, deskHdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY);
        ReleaseDC(GetDesktopWindow(), deskHdc);
        ReleaseDC(hwnd, wndHdc);
    }
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	MSG msg;
    h::baseStyle(WndProc, L"capture");
    SetLayeredWindowAttributes(
    CreateWindowEx(WS_EX_LAYERED,TEXT("capture"),TEXT("capture"),WS_VISIBLE| WS_POPUP|WS_MAXIMIZE,0,0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),nullptr,nullptr,hInstance,nullptr)
    ,RGB(-4649,-4649,-4649),std::stoi("0" + h::File("alpha").read().getContent().substr(0,3)), LWA_ALPHA);
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}