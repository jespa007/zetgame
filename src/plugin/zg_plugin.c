void* plugin_helper_load_plugin(const gchar *filename)
{
    gchar *pathname = NULL;
    void *plugin = NULL;
    if (!filename || !filename[0])
        return NULL;
    pathname = plugin_helper_find_plugin_file (filename);
    if (!pathname)
    {
        g_warning (_("Couldn't find plugin file: %s"), filename);
        return NULL;
    }
    plugin = (void*)LoadLibrary(pathname);
    g_free (pathname);
    return plugin;
}

void load_all_plugins(GtkWidget *widget, gpointer user_data)
{
    LPPLUGINSTRUCT pls = NULL;
    CREATEPROC create = NULL;
    MENUPROC menuproc = NULL;
    MENUCATPROC menucatproc = NULL;
    RUNPROC runproc = NULL;
    …
    …
    void* handle = NULL;
    GtkWidget* mw = plugin_app_get_mainwindow();
    editmenuitem = lookup_widget(mw, "editmenuitem");
    insertmenuitem = lookup_widget(mw, "insertmenuitem");
    elem = plugin_helper_get_plugin_list();
    while (elem)
    {
        filename = (gchar*)elem->data;
        handle = plugin_helper_load_plugin(filename);
        if(handle)
        {
            create = (CREATEPROC) GetProcAddress(handle, "plugin_app_create_plugin");
            if ((error = GetLastError()) != 0)
            {
                …
                …
            }
            else
            {
                /* store the handle for later use */
                plugin_handles = g_list_prepend(plugin_handles, handle);
                /* Create an instance of the plugin struct */
                pls = create();
                if(pls && pls->menuProc && pls->runProc)
                {
                    /* store the plugin struct for deletion at exit */
                    plugin_structs = g_list_prepend(plugin_structs, pls);
                    menuproc = (MENUPROC) pls->menuProc;
                    menuName = (gchar*)menuproc();
                    menucatproc = (MENUCATPROC) pls->menuCatProc;
                    menuCategory = (gchar*)menucatproc();
                    g_message (_("Creating menu item: %s"), menuName);
                    menu = gtk_image_menu_item_new_with_label (menuName);
                    gtk_widget_set_name (menu, menuName);
                    gtk_widget_show (menu);
                    g_object_set_data_full (G_OBJECT (mw), menuName,
                    gtk_widget_ref (menu), (GDestroyNotify) gtk_widget_unref);
                    if(g_strcmp0(menuCategory, "edit") == 0)
                    {
                        gtk_container_add (GTK_CONTAINER (editmenuitem), menu);
                    }
                    else
                    {
                        gtk_container_add (GTK_CONTAINER (insertmenuitem), menu);
                    }
                    runproc = (RUNPROC)pls->runProc;
                    g_signal_connect(G_OBJECT(menu), "activate", G_CALLBACK(runproc), NULL);
                }
                else
                {
                    g_printerr("Invalid Plugin Structure!");
                }
            }
        }
        elem = elem->next;
    }
    gtk_widget_show_all(widget);
}

static GtkWidget* pDlg1 = NULL;
...
LPPLUGINSTRUCT plugin_app_create_plugin()
{
    LPPLUGINSTRUCT PLS = (LPPLUGINSTRUCT)malloc(sizeof(PluginStruct));
    g_debug("TorusDlg::plugin_app_create_plugin");
    if(!PLS)
    {
        return NULL;
    }
    PLS->nameProc = plugin_app_get_plugin_name;
    PLS->providerProc = plugin_app_get_plugin_provider;
    PLS->menuProc = plugin_app_get_menu_name;
    PLS->menuCatProc = plugin_app_get_menu_category;
    PLS->runProc = plugin_app_run_proc;
    PLS->destProc = plugin_app_destroy_plugin;
    return PLS;
}
void plugin_app_run_proc()
{
    pDlg1 = create_TorusDlg();
    gtk_window_set_transient_for(GTK_WINDOW(pDlg1), GTK_WINDOW(plugin_app_get_mainwindow()));
    gtk_widget_show(pDlg1);
}
