#include <gtk/gtk.h>
#include <unistd.h>
GtkWidget *window;
GtkWidget *fixed;
GtkWidget *g_image;
GtkWidget *g_btn;
GtkWidget *g_entry;
GdkPixbufAnimation* anim_pixbuf;
const GdkPixbuf *src_pixbuf;
GdkPixbuf *dest_pixbuf;
 
void destroy(GtkWidget *widget,gpointer data)
{
    gtk_main_quit();
}
gboolean showimage()
{
    src_pixbuf = gdk_pixbuf_new_from_file("output.png", NULL); 
    dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 300, 300, GDK_INTERP_HYPER);
    gtk_image_set_from_pixbuf (g_image,dest_pixbuf);
    return FALSE;
}
void make()
{
    char *text = gtk_entry_get_text (g_entry);
    char *cmd = "qrencode -l L -v 1 -s 11 -o output.png '";
    char *last = "'";
    char *qrcodecmd = (char *) malloc(strlen(cmd) + strlen(text)+ strlen(last));
    sprintf(qrcodecmd, "%s%s%s", cmd, text,last);
    system(qrcodecmd);
    GError *error = NULL;
    anim_pixbuf = gdk_pixbuf_animation_new_from_file("loading.gif", &error);
    if(error!=NULL)
    {
       printf("Error: %s\n", error->message);
       g_error_free(error);
       error = NULL;
    }
    gtk_image_set_from_animation (GTK_IMAGE(g_image),anim_pixbuf);
    g_timeout_add(1000, (GSourceFunc)showimage, NULL);
}

int main(int argc,char *argv[])
{
    
    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_window_set_default_size(GTK_WINDOW(window),320,380);
    gtk_window_set_resizable (GTK_WINDOW(window),FALSE);
    g_signal_connect(window, "destroy",G_CALLBACK (destroy), NULL);


    gtk_window_set_title(GTK_WINDOW(window),"qrencode GUI");
    gtk_window_set_keep_above(GTK_WINDOW (window), TRUE);

    fixed = gtk_fixed_new ();
    
    g_btn = gtk_button_new_with_mnemonic ("make");
    g_signal_connect(g_btn, "clicked",G_CALLBACK(make), NULL);
    gtk_widget_set_size_request(g_btn,80,50);
    g_entry = gtk_entry_new ();
    gtk_widget_set_size_request(g_entry,210,50);
    g_image = gtk_image_new ();
    gtk_widget_set_size_request(g_image,300,300);

    gtk_fixed_put (fixed,g_image,0,0);
    gtk_fixed_put (fixed,g_entry,0,320);
    gtk_fixed_put (fixed,g_btn,220,320);

    gtk_container_add (GTK_CONTAINER (window), fixed);
    
    gtk_widget_show_all (window);
    gtk_widget_set_opacity (GTK_WIDGET (window), 0.9);
    src_pixbuf = gdk_pixbuf_new_from_file("img.png", NULL); 
    dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, 300, 300, GDK_INTERP_HYPER);
    gtk_image_set_from_pixbuf (g_image,dest_pixbuf);
    gtk_main ();

    return 0;
}


