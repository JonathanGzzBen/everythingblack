#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include "everythingblack/implementation_windows.h"
#else
#include "everythingblack/implementation_linux_x11.h"
#endif

int main(int argc, char *argv[]) {
#ifdef WIN32
  return windows_main(argc, argv);
#else
  if (argc != 2) {
    puts("Usage:\n"
         "\teverythingblackexcept <MONITOR>\n\n"
         "Displays blackscreen on all monitors except MONITOR.\n"
         "Use Alt+F4 to escape the blackness.");
    return EXIT_SUCCESS;
  }

  int monitor_to_keep_on = atoi(argv[1]);
  if (monitor_to_keep_on == 0) {
    puts("MONITOR is 1-based indexed");
    return EXIT_FAILURE;
  }

  return linux_x11_display_black_on_all_screens_except(monitor_to_keep_on);
#endif
}
