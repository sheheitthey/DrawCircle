#include<gtk/gtk.h>

static void
DrawPixel(int x, int y);

static void
DrawCircle(int r)
{
    int i;

    for (i = -r; i <= r; i++)
    {
        DrawPixel(r, i);
        DrawPixel(-i, r);
        DrawPixel(-r, -i);
        DrawPixel(i, -r);
    }
}

static void
DrawCircles()
{
    int r;

    for (r = 20; r <= 300; r += 20)
        DrawCircle(r);
}


/* You should not need to touch anything that follows. */

static GtkWidget *window;
static GtkWidget *drawing_area = NULL;
static GdkPixmap *pixmap = NULL;
static gdouble center_x, center_y;

static void
DrawPixel(int x, int y)
{
    gdk_draw_point(pixmap, drawing_area->style->black_gc,
                   center_x + x, center_y - y);
}

static gint
configure_event(GtkWidget *widget, GdkEventConfigure *event)
{
    const int width = widget->allocation.width;
    const int height = widget->allocation.height;
    if(pixmap)
        gdk_pixmap_unref(pixmap);

    pixmap = gdk_pixmap_new(widget->window, width, height, -1);
    gdk_draw_rectangle(pixmap, widget->style->white_gc, TRUE, 0, 0,
                       width, height);
    center_x = width/2;
    center_y = width/2;

    DrawCircles();

    return TRUE;
}

static gboolean
expose_event(GtkWidget *widget, GdkEventExpose *event)
{
    gdk_draw_drawable(widget->window,
                      widget->style->fg_gc[GTK_WIDGET_STATE(widget)], pixmap,
                      event->area.x, event->area.y,
                      event->area.x, event->area.y,
                      event->area.width, event->area.height);
    return FALSE;
}

static gboolean
delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    return FALSE;
}

static void
destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    drawing_area = gtk_drawing_area_new();

    gtk_window_resize(GTK_WINDOW(window), 800, 800);

    g_signal_connect(G_OBJECT(window), "delete_event",
                     G_CALLBACK(delete_event), NULL);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK(destroy), NULL);

    g_signal_connect(G_OBJECT(drawing_area), "configure_event",
                     G_CALLBACK(configure_event), NULL);

    g_signal_connect(G_OBJECT(drawing_area), "expose_event",
                     G_CALLBACK(expose_event), NULL);

    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    gtk_widget_show(window);
    gtk_widget_show(drawing_area);

    gtk_main();

    return 0;
}
