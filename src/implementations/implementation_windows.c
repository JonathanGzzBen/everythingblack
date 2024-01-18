#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

static HINSTANCE g_instance;
static int g_monitor_to_keep_on = 0;
static ATOM g_window_class_atom;

LRESULT OnEraseBkgnd(HWND hwnd, HDC hdc) {
  RECT rect;
  GetClientRect(hwnd, &rect);

  HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // Black color
  FillRect(hdc, &rect, hBrush);
  DeleteObject(hBrush);

  return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_ERASEBKGND:
    return OnEraseBkgnd(hwnd, (HDC)wParam);
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

BOOL CALLBACK Monitorenumproc(HMONITOR hmon, HDC hdc, LPRECT rect,
                              LPARAM param) {
  static int monitor_number = 0;
  monitor_number++;
  if (monitor_number == g_monitor_to_keep_on)
    return true;
  MONITORINFO mi = {sizeof(mi)};
  if (!GetMonitorInfo(hmon, &mi))
    return false;

  CreateWindow((LPCTSTR)g_window_class_atom, "", WS_POPUP | WS_VISIBLE,
               mi.rcMonitor.left, mi.rcMonitor.top,
               mi.rcMonitor.right - mi.rcMonitor.left,
               mi.rcMonitor.bottom - mi.rcMonitor.top, NULL, NULL, g_instance,
               0);

  return true;
}

int windows_main(int argc, char *argv[]) {

  if (argc != 2) {
    puts("Usage:\n"
         "\teverythingblackexcept <MONITOR>\n\n"
         "Displays blackscreen on all monitors except MONITOR.\n"
         "Use Alt+F4 to escape the blackness.");
    return 0;
  }
  g_monitor_to_keep_on = atoi(argv[1]);

  if (g_monitor_to_keep_on == 0) {
    puts("MONITOR is 1-based indexed");
    return 1;
  }

  g_instance = GetModuleHandle(NULL);
  WNDCLASSEX wc = {0};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.hInstance = g_instance;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszClassName = "Everything Black Except";

  ATOM window_class_atom = RegisterClassEx(&wc);
  g_window_class_atom = window_class_atom;

  EnumDisplayMonitors(NULL, NULL, Monitorenumproc, 0);

  // Message loop
  MSG msg = {0};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}