#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <gdk/gdkkeysyms.h>
#include <X11/Xlib.h>
#include <thread>
#include <iostream>

#include "browser.hpp"

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);
static gboolean context_menu_callback(GtkWidget*, GtkWidget*, WebKitHitTestResult*, gboolean, gpointer);

const int display_width = 1200;
const int display_height = 600;

bool fullscreen_state = true;

Display* d = XOpenDisplay(NULL);
Screen*  s = DefaultScreenOfDisplay(d);

void swichFullscreenMode(GtkWidget *widget){
    GdkGeometry geom;
    if (fullscreen_state) {
        printf("Set sized\n");
        gtk_window_unfullscreen(GTK_WINDOW(widget));
        // set window size
        geom.max_width = display_width;
        geom.max_height = display_height;
    } else {
        printf("Set fullscreen\n");
        gtk_window_fullscreen(GTK_WINDOW(widget));
        geom.max_width = s->width;
        geom.max_height = s->height;
    }
    gtk_window_set_geometry_hints(GTK_WINDOW(widget), NULL, &geom, GDK_HINT_MAX_SIZE);
    fullscreen_state = !fullscreen_state;
    printf("fullscreeen state: '%s'\n", (fullscreen_state) ? "true" : "false" );
    printf("screen size: %dx%d \n", s->width, s->height);
}

gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  // suppress warnings
  (void) widget;
  (void) user_data;
  // nadle reys
  switch (event->keyval)
  {
    case GDK_KEY_p:
      printf("key release: %s\n", "p");
      swichFullscreenMode(widget);
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

int start_browser()
{
  // Initialize GTK+
//std::thread([&] (){
  gdk_threads_init();
  gtk_init(nullptr, nullptr);

  // Create window that will contain the browser instance
  GtkWidget* main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(main_window), display_width, s->height);

  // Define max geometry of window
  gtk_window_set_resizable(GTK_WINDOW(main_window), TRUE);

  // Create a browser instance
  WebKitWebView* webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
//  if (nullptr == webView) {
    std::cout << "WebKitWebView: " << webView << std::endl;
//  }

//  webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
  // Put the browser area into the main window
  gtk_container_add(GTK_CONTAINER(main_window), GTK_WIDGET(webView));

  gtk_window_fullscreen(GTK_WINDOW(main_window));

  // Set up callbacks so that if either the main window or the browser instance is
  // closed, the program will exit
  g_signal_connect(main_window, "key_release_event", G_CALLBACK(on_key_release), NULL);
  g_signal_connect(main_window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
  g_signal_connect(webView, "close", G_CALLBACK(closeWebViewCb), main_window);

  // disable context menu
  g_signal_connect(webView, "context-menu", G_CALLBACK(context_menu_callback), NULL);

  WebKitSettings* settings = webkit_web_view_get_settings(webView);
  webkit_settings_set_enable_write_console_messages_to_stdout(settings, TRUE);
  webkit_web_view_set_settings(webView, settings);

  // Load a web page into the browser instance
//  webkit_web_view_load_uri(webView, "http://localhost:8080/");
  webkit_web_view_load_uri(webView, "http://ya.ru");

  // Make sure that when the browser area becomes visible, it will get mouse
  // and keyboard events
  gtk_widget_grab_focus(GTK_WIDGET(webView));

  // Make sure the main window and all its contents are visible
  gtk_widget_show_all(main_window);

  // Run the main GTK+ event loop
  gtk_main();
//}).join();

  return 0;
}


static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
  // suppress warnings
  (void) widget;
  (void) window;
  gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
  // suppress warnings
  (void) webView;
  gtk_widget_destroy(window);
  return TRUE;
}

static gboolean context_menu_callback(GtkWidget*, GtkWidget*, WebKitHitTestResult*, gboolean, gpointer) {
    return TRUE;
}
