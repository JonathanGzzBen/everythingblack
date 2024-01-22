#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xinerama.h>
#include <X11/keysym.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool set_borderless_window_hints(Display *display, Window window) {
  struct MwmHints {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
  };
  enum { MWM_HINTS_DECORATIONS = (1L << 1) };

  Atom mwmHintsProperty = XInternAtom(display, "_MOTIF_WM_HINTS", true);
  if (mwmHintsProperty == 0) {
    fputs("Unable to get atom _MOTIF_WM_HINTS", stderr);
    return false;
  }
  struct MwmHints hints;
  hints.flags = MWM_HINTS_DECORATIONS;
  hints.decorations = 0;
  XChangeProperty(display, window, mwmHintsProperty, mwmHintsProperty, 32,
                  PropModeReplace, (unsigned char *)&hints, 5);
  return true;
}

bool set_on_top_always_window_hints(Display *display, Window window) {
  Atom wm_state = XInternAtom(display, "_NET_WM_STATE", True);
  Atom net_wm_state_above = XInternAtom(display, "_NET_WM_STATE_ABOVE", False);

  XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace,
                  (unsigned char *)&net_wm_state_above, 1);
  return true;
}

bool set_fullscreen_window_hints(Display *display, Window window) {
  Atom wm_state = XInternAtom(display, "_NET_WM_STATE", True);
  Atom wm_fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", True);

  XChangeProperty(display, window, wm_state, XA_ATOM, 32, PropModeReplace,
                  (unsigned char *)&wm_fullscreen, 1);
  return true;
}

bool set_window_hints(Display *display, Window window) {
  /* The following window hints should be applied in the order specified */

  if (!set_borderless_window_hints(display, window)) {
    fputs("Could not enable borderless window hints", stderr);
    return false;
  }

  if (!set_on_top_always_window_hints(display, window)) {
    fputs("Could not enable always on top window hints", stderr);
    return false;
  }

  if (!set_fullscreen_window_hints(display, window)) {
    fputs("Could not enable fullscreen window hints", stderr);
    return false;
  }

  return true;
}

int display_black_on_all_screens_except(int screen_to_skip) {

  Display *display = XOpenDisplay(NULL);
  if (display == NULL) {
    puts("Failed to open display!");
    return EXIT_FAILURE;
  }
  Window wnd = XDefaultRootWindow(display);

  int screen = DefaultScreen(display);

  int num_screens = 0;
  int error = 0;
  if (XineramaQueryExtension(display, &num_screens, &error)) {
    /* Multi monitor flow */

    XineramaScreenInfo *screens = XineramaQueryScreens(display, &num_screens);

    for (int i = 0; i < num_screens; i++) {
      if (screens[i].screen_number + 1 == screen_to_skip) {
        continue;
      }
      int target_screen_id = screens[0].screen_number;
      Window window = XCreateSimpleWindow(
          display, RootWindow(display, target_screen_id), screens[i].x_org,
          screens[i].y_org, screens[i].width, screens[i].height, 1,
          BlackPixel(display, screen), BlackPixel(display, screen));

      if (!set_window_hints(display, window)) {
        return EXIT_FAILURE;
      }

      XSelectInput(display, window, KeyPressMask);
      XMapWindow(display, window);
    }
  } else {
    /* Single monitor flow */

    /* Create window*/
    Window window = XCreateSimpleWindow(
        display, RootWindow(display, screen), 100, 100, 400, 400, 1,
        BlackPixel(display, screen), BlackPixel(display, screen));

    if (!set_window_hints(display, window)) {
      return EXIT_FAILURE;
    }

    XSelectInput(display, window, KeyPressMask);
    XMapWindow(display, window);
  }

  XEvent event;
  while (1) {
    XNextEvent(display, &event);
    if (event.type == Expose) {
      // Redraw window if necessary
    } else if (event.type == KeyPress) {
      KeySym keysym = XLookupKeysym(&event.xkey, 0);
      if (keysym == XK_Escape) {
        break;
      }
    }
  }

  XCloseDisplay(display);

  return 0;
}