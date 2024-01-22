#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "everythingblack/implementation_windows.h"
#else
#include "everythingblack/implementation_linux_x11.h"
#endif

int main(int argc, char *argv[]) {
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

  display_black_on_all_screens_except(monitor_to_keep_on);
}
