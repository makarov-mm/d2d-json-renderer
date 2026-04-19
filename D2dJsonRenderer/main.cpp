#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <windows.h>
#include <windowsx.h>
#include <d2d1.h>

#include "sceneView.h"
#include "jsonSceneLoader.h"

#define IDM_FILE_OPEN 101
#define IDM_FILE_EXIT 102
#define IDM_HELP_ABOUT 201
#define IDM_VIEW_ZOOMIN 301
#define IDM_VIEW_ZOOMOUT 302
#define IDM_VIEW_RESET 303

static Presentation::SceneView g_sceneView;
static std::unique_ptr<Domain::Scene> g_scene;
static Infrastructure::JsonSceneLoader g_loader;

static void UpdateTitle(HWND hwnd)
{
    wchar_t buf[64];
    int pct = static_cast<int>(g_sceneView.GetZoom() * 100.0);
    swprintf_s(buf, L"Direct2D JSON Renderer  —  Zoom: %d%%", pct);
    SetWindowText(hwnd, buf);
}

static std::wstring ShowOpenFileDialog(HWND hwnd)
{
    wchar_t filePath[MAX_PATH] = {};
    OPENFILENAMEW ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = L"json";

    if (GetOpenFileNameW(&ofn))
    {
        return std::wstring(filePath);
    }

    return {};
}

static HMENU CreateAppMenu()
{
    HMENU hMenuBar = CreateMenu();

    HMENU hFile = CreatePopupMenu();
    AppendMenu(hFile, MF_STRING, IDM_FILE_OPEN, L"Open JSON...\tCtrl+O");
    AppendMenu(hFile, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hFile, MF_STRING, IDM_FILE_EXIT, L"Exit");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");

    HMENU hView = CreatePopupMenu();
    AppendMenu(hView, MF_STRING, IDM_VIEW_ZOOMIN, L"Zoom In\tCtrl++");
    AppendMenu(hView, MF_STRING, IDM_VIEW_ZOOMOUT, L"Zoom Out\tCtrl+-");
    AppendMenu(hView, MF_SEPARATOR, 0, nullptr);
    AppendMenu(hView, MF_STRING, IDM_VIEW_RESET, L"Reset Zoom\tCtrl+0");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hView, L"View");

    HMENU hHelp = CreatePopupMenu();
    AppendMenu(hHelp, MF_STRING, IDM_HELP_ABOUT, L"About");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hHelp, L"Help");

    return hMenuBar;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_FILE_OPEN:
        {
            std::wstring path = ShowOpenFileDialog(hwnd);
            if (path.empty()) return 0;

            try
            {
                g_scene = g_loader.Load(path);
                g_sceneView.ResetZoom();
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateTitle(hwnd);
            }
            catch (const std::exception& ex)
            {
                int size = MultiByteToWideChar(CP_UTF8, 0, ex.what(), -1, nullptr, 0);
                std::wstring wmsg(size, L'\0');
                MultiByteToWideChar(CP_UTF8, 0, ex.what(), -1, &wmsg[0], size);
                MessageBox(hwnd, wmsg.c_str(), L"Loading Error", MB_OK | MB_ICONERROR);
            }
            return 0;
        }

        case IDM_FILE_EXIT:
            DestroyWindow(hwnd);
            return 0;

        case IDM_VIEW_ZOOMIN:
            g_sceneView.ZoomIn();
            InvalidateRect(hwnd, nullptr, FALSE);
            UpdateTitle(hwnd);
            return 0;

        case IDM_VIEW_ZOOMOUT:
            g_sceneView.ZoomOut();
            InvalidateRect(hwnd, nullptr, FALSE);
            UpdateTitle(hwnd);
            return 0;

        case IDM_VIEW_RESET:
            g_sceneView.ResetZoom();
            InvalidateRect(hwnd, nullptr, FALSE);
            UpdateTitle(hwnd);
            return 0;

        case IDM_HELP_ABOUT:
            MessageBox(hwnd,
                L"Direct2D JSON Renderer (C++ / Direct2D)\n\nCreated by Mykhailo Makarov\nm.m.makarov@gmail.com",
                L"About",
                MB_OK | MB_ICONINFORMATION);
            return 0;
        }
        break;

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        RECT rc;
        GetClientRect(hwnd, &rc);
        g_sceneView.ZoomAtPoint(delta);
        InvalidateRect(hwnd, nullptr, FALSE);
        UpdateTitle(hwnd);
        return 0;
    }

    case WM_KEYDOWN:
        if (GetKeyState(VK_CONTROL) & 0x8000)
        {
            switch (wParam)
            {
            case VK_OEM_PLUS: case VK_ADD:
                g_sceneView.ZoomIn();
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateTitle(hwnd);
                return 0;

            case VK_OEM_MINUS: case VK_SUBTRACT:
                g_sceneView.ZoomOut();
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateTitle(hwnd);
                return 0;

            case '0': case VK_NUMPAD0:
                g_sceneView.ResetZoom();
                InvalidateRect(hwnd, nullptr, FALSE);
                UpdateTitle(hwnd);
                return 0;
            }
        }
        break;

    case WM_PAINT:
        g_sceneView.Render(g_scene.get());
        ValidateRect(hwnd, nullptr);
        return 0;

    case WM_SIZE:
        g_sceneView.Resize(LOWORD(lParam), HIWORD(lParam));
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszClassName = L"Direct2D JSON Renderer";
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        L"Direct2D JSON Renderer",
        L"Direct2D JSON Renderer — Zoom: 100%",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1200, 900,
        nullptr,
        CreateAppMenu(),
        hInstance,
        nullptr);

    if (!hwnd) return 1;

    if (FAILED(g_sceneView.Initialize(hwnd)))
        return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
