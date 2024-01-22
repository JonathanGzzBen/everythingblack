#ifndef IMPLEMENTATION_WINDOWS_H
#define IMPLEMENTATION_WINDOWS_H

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

LRESULT OnEraseBkgnd(HWND hwnd, HDC hdc);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Monitorenumproc(HMONITOR hmon, HDC hdc, LPRECT rect,
                              LPARAM param);
int display_black_on_all_screens_except(int screen_to_skip);

#endif // IMPLEMENTATION_WINDOWS_H