/*
 * Copyright (C) 2026 Igalia S.L.
 * Author: Mario Sánchez Prada <mario@igalia.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <wpe/wpe-platform.h>
#include <wpe/webkit.h>

static void
print_toplevel_state (WPEToplevelState state)
{
    g_print ("Toplevel state:\n"
             " - Fullscreen: %s\n"
             " - Maximized: %s\n"
             " - Active: %s\n\n",
             (state & WPE_TOPLEVEL_STATE_FULLSCREEN) ? "Yes" : "No",
             (state & WPE_TOPLEVEL_STATE_MAXIMIZED) ? "Yes" : "No",
             (state & WPE_TOPLEVEL_STATE_ACTIVE) ? "Yes" : "No");
}

static gboolean
on_view_event (WPEView *view, WPEEvent *event, gpointer user_data)
{
    if (wpe_event_get_event_type (event) != WPE_EVENT_KEYBOARD_KEY_DOWN)
        return FALSE;

    if ((wpe_event_get_modifiers (event) & WPE_MODIFIER_KEYBOARD_CONTROL)) {
        WPEToplevel *toplevel = wpe_view_get_toplevel (view);
        WPEToplevelState toplevel_state = wpe_toplevel_get_state (toplevel);

        if (wpe_event_keyboard_get_keyval (event) == WPE_KEY_r) {
            WebKitWebView *web_view =
                g_object_get_data (G_OBJECT (view), "webkit-web-view");
            webkit_web_view_reload (web_view);
            return TRUE;
        }

        if (wpe_event_keyboard_get_keyval (event) == WPE_KEY_f) {
            if (toplevel_state & WPE_TOPLEVEL_STATE_FULLSCREEN) {
                wpe_toplevel_unfullscreen (toplevel);
            } else {
                wpe_toplevel_fullscreen (toplevel);
            }
            return TRUE;
        }

        if (wpe_event_keyboard_get_keyval (event) == WPE_KEY_m) {
            if (toplevel_state & WPE_TOPLEVEL_STATE_MAXIMIZED) {
                wpe_toplevel_unmaximize (toplevel);
            } else {
                wpe_toplevel_maximize (toplevel);
            }
            return TRUE;
        }

        if (wpe_event_keyboard_get_keyval (event) == WPE_KEY_q) {
            g_main_loop_quit ((GMainLoop *) user_data);
            return TRUE;
        }
    }
    return FALSE;
}

static void
on_view_toplevel_state_changed (WPEView *view,
                                WPEToplevelState prev_state,
                                gpointer user_data)
{
    WPEToplevel *toplevel = wpe_view_get_toplevel (view);
    print_toplevel_state (wpe_toplevel_get_state (toplevel));
}

static void
on_web_view_load_changed (WebKitWebView *web_view,
                          WebKitLoadEvent load_event,
                          gpointer user_data)
{
    const char *uri = webkit_web_view_get_uri (web_view);

    switch (load_event) {
        case WEBKIT_LOAD_STARTED:
            g_print ("Load started: %s\n", uri);
            break;
        case WEBKIT_LOAD_FINISHED:
            g_print ("Load finished: %s\n", uri);
            break;
        default:
            break;
    }
}

static void
on_view_closed (WPEView *view, gpointer user_data)
{
    g_print ("Closing view.\n");
    g_main_loop_quit ((GMainLoop *) user_data);
}

int
main (int argc, char *argv[])
{
    const char *uri = (argc > 1) ? argv[1] : "https://wpewebkit.org";

    WPEDisplay *display = wpe_display_get_default ();
    if (!display) {
        g_printerr ("Could not connect to a WPE display.\n");
        return 1;
    }

    g_autoptr(WebKitWebView) web_view =
        WEBKIT_WEB_VIEW (g_object_new (WEBKIT_TYPE_WEB_VIEW,
                                       "display", display, NULL));

    // Attach the WebKitWebView to the WPEView to use it from callbacks.
    WPEView *wpe_view = webkit_web_view_get_wpe_view (web_view);
    g_object_set_data (G_OBJECT (wpe_view), "webkit-web-view", web_view);

    // Set a title to the launcher Window.
    WPEToplevel *wpe_toplevel = wpe_view_get_toplevel (wpe_view);
    wpe_toplevel_set_title (wpe_toplevel, "OSS EU 2026 demo");
    print_toplevel_state (wpe_toplevel_get_state (wpe_toplevel));

    // Create main loop and connect callbacks.
    g_autoptr(GMainLoop) loop = g_main_loop_new (NULL, FALSE);

    g_signal_connect (web_view, "load-changed",
                      G_CALLBACK (on_web_view_load_changed), NULL);

    g_signal_connect (wpe_view, "event", G_CALLBACK (on_view_event), loop);
    g_signal_connect (wpe_view, "toplevel-state-changed",
                      G_CALLBACK (on_view_toplevel_state_changed), NULL);
    g_signal_connect (wpe_view, "closed",
                      G_CALLBACK (on_view_closed), loop);

    // Load the URI and run the main loop.
    webkit_web_view_load_uri (web_view, uri);
    g_main_loop_run (loop);

    return 0;
}
