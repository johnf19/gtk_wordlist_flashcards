//----------------------头文件区
#include <stdio.h>
#include "sqlite3.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
//---------------------全局变量区
GtkWidget* window_main;
GtkWidget* window_add;
GtkWidget* window_delete;
GtkWidget* entry_w_m_id;
GtkWidget* entry_w_m_passwd;
GtkWidget *entry_inside;
GtkWidget *entry_inside2;
GtkWidget *entry_outside;
GtkWidget *window_menu;
GtkWidget *window_deck;
GtkWidget *window_test;
GtkWidget* label_word;
GtkWidget* label_exp;

char dbname[100];
char *elein;
enum {
  LIST_IN,
  LIST_OUT,
  N_COLUMNS
};

int n = 0;

struct _word_card
{
	char _word[100];
	char _explain[100];
	char _power[2];
	char _unique_id;
} word_array[500];


void my_window_deck_init(void);
void my_window_menu_init(void);
void my_window_add_init(void);
void my_window_delete_init(void);
void my_window_test_init(void);

int callback(void *, int, char **, char **);
//-------------------其他函数区————全是网上找到的函数
void my_sql_init(void)
{
    sqlite3* db;
    sqlite3_open("user.db",&db);
    char *sql = "create table users (id text,passwd text);";

    char* errmsg = NULL;
    sqlite3_exec(db,sql,NULL,NULL,&errmsg);
    if (errmsg!=NULL)
    {
        printf("User DB initialization failed：%s\n", errmsg);
    }
    sqlite3_close(db);
    
}


void sql_deck_init(const char* userid)
{
	sqlite3 *deckdb;
	
	strcpy(dbname, userid);
	strcat(dbname, ".db");
	puts(dbname);
	sqlite3_open(dbname, &deckdb);
	char *sql1 = "create table word (inside text ,outside text ,power int default 2);"; 

	char* errmsg = NULL;
	sqlite3_exec(deckdb, sql1, NULL, NULL, &errmsg);
	if (errmsg != NULL)
	{
		printf("Failed to load user deck database: %s\n", errmsg);
	}
	sqlite3_close(deckdb);

}

void sql_deck_read(GtkButton* button,gpointer data)
{
	sqlite3 *db;
        char *err_msg = 0;

        int rc = sqlite3_open(dbname, &db);

        if (rc != SQLITE_OK)
        {
                fprintf(stderr, "Cannot open database: %s\n",
                sqlite3_errmsg(db));
                sqlite3_close(db);
        }

        char *sql = "";
        sql = "SELECT * FROM word";
        rc = sqlite3_exec(db, sql, callback, data, &err_msg);

        if (rc != SQLITE_OK )
        {
                fprintf(stderr, "Failed to select data\n");
                fprintf(stderr, "SQL error: %s\n", err_msg);

                sqlite3_free(err_msg);
                sqlite3_close(db);
        }

         sqlite3_close(db);
}
void window_main_reg(GtkButton* button,gpointer user_data)
{
    sqlite3* db;
    sqlite3_open("user.db",&db);
    const char * id = gtk_entry_get_text(GTK_ENTRY(entry_w_m_id));
    const char * passwd = gtk_entry_get_text(GTK_ENTRY(entry_w_m_passwd));
    char sql1[100] = "";
    sprintf(sql1,"select * from users where id = \'%s\';",id);
    char *errmsg = NULL;
    int r = 0, c = 0;
    char ** table = NULL;
    sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
    if (errmsg!=NULL)
    {
        printf("%s\n", errmsg);
    }
    if (r!=0)
    {
        GtkWidget *dialog_tuifang_queding;
        dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"Id already existed", "title");
        gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
        gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
        gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
        gtk_widget_destroy(dialog_tuifang_queding);

        sqlite3_close(db);
        return;
    }
    char sql2[100] = "";
    sprintf(sql2,"insert into users values (\'%s\',\'%s\');",id,passwd);
    errmsg = NULL;
    sqlite3_exec(db,sql2,NULL,NULL,&errmsg);
    if (errmsg!=NULL)
    {
        printf("%s\n", errmsg);
        sqlite3_close(db);
        return;
    }
    errmsg = NULL;
    r = 0, c = 0;
    table = NULL;
    sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
    if (errmsg!=NULL)
    {
        printf("%s\n", errmsg);
    }
    if (r!=0)
    {
        GtkWidget *dialog_tuifang_queding;
        dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"Register completed", "title");
        gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
        gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
        gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
        gtk_widget_destroy(dialog_tuifang_queding);

        sqlite3_close(db);
        return;
    }
}
void window_main_log(GtkButton* button,gpointer user_data)
{
    sqlite3* db;
    sqlite3_open("user.db",&db);
    const char * id = gtk_entry_get_text(GTK_ENTRY(entry_w_m_id));
    const char * passwd = gtk_entry_get_text(GTK_ENTRY(entry_w_m_passwd));
    char sql1[100] = "";
    sprintf(sql1,"select * from users where id = \'%s\' and passwd = \'%s\';",id,passwd);
    char *errmsg = NULL;
    int r = 0, c = 0;
    char ** table = NULL;
    sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
    if (errmsg!=NULL)
    {
        printf("%s\n", errmsg);
    }
    if (r==0)
    {
        GtkWidget *dialog_tuifang_queding;
        dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"Wrong login elements", "title");
        gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
        gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
        gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
        gtk_widget_destroy(dialog_tuifang_queding);

        sqlite3_close(db);
        return;
    }
    else
    {
	sql_deck_init(id);
        my_window_deck_init();
    	my_window_menu_init();
	gtk_widget_show_all(window_menu);
        gtk_widget_hide_all(window_main);
	//execlp("mplayer","mplayer","-slave","-quiet",    "-idle",        "-input", "file=./cmd_fifo","爱如火.mp3",NULL);
    }

}

void do_sign_out(GtkButton* button,gpointer user_data)
{
        gtk_widget_hide_all(window_menu);
        gtk_widget_show_all(window_main);
}

void do_show_all(GtkButton* button,gpointer user_data)
{
	gtk_widget_hide_all(window_menu);
	gtk_widget_show_all(window_deck);
}

void do_test(GtkButton* button,gpointer user_data)
{
        gtk_widget_hide_all(window_menu);
        g_object_ref(window_deck);
	my_window_deck_init();
	g_object_ref(window_deck);
	my_window_test_init();
	gtk_widget_show_all(window_test);
}

void do_add_word(GtkButton* button,gpointer user_data)
{
        my_window_add_init();
	gtk_widget_show_all(window_add);
}


void do_deck_delete(GtkButton *button, gpointer user_data)
{
	my_window_delete_init();
	gtk_widget_show_all(window_delete);	
}

void do_delete_back(GtkButton *button, gpointer user_data)
{
        gtk_widget_hide_all(window_delete);
	g_object_ref(window_delete);
        gtk_widget_show_all(window_deck);
}

void do_deck_back(GtkButton *button, gpointer user_data)
{
        gtk_widget_hide_all(window_deck);
	g_object_ref(window_deck);
        gtk_widget_show_all(window_menu);
}

void do_add_back(GtkButton *button, gpointer user_data)
{
        gtk_widget_hide_all(window_add);
	g_object_ref(window_add);
        gtk_widget_show_all(window_deck);
}

void do_add_words_look_up(GtkButton *button, gpointer user_data)
{
        sqlite3 *db;
        sqlite3_open(dbname, &db);
        const char * outside = gtk_entry_get_text(GTK_ENTRY(entry_outside));
        const char * inside = gtk_entry_get_text(GTK_ENTRY(entry_inside));
        char sql1[100] = "";
        sprintf(sql1,"select * from word where inside = \'%s\';",inside);
        char *errmsg = NULL;
        int r = 0, c = 0;
        char ** table = NULL;
        sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
        if (errmsg!=NULL)
        {
                printf("%s\n", errmsg);
        }
        if (r!=0)
        {
                char *exp = table[4];
                GtkWidget *dialog_tuifang_queding;
                dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,exp, "title");
                gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
                gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
                gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
                gtk_widget_destroy(dialog_tuifang_queding);
                gtk_widget_hide_all(window_add);
                sqlite3_close(db);
                return;
        }
	else
	{
                GtkWidget *dialog_tuifang_queding;
                dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"No Such Word!", "title");
                gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
                gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
                gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
                gtk_widget_destroy(dialog_tuifang_queding);
                gtk_widget_hide_all(window_add);
                sqlite3_close(db);
                return;
        }
	sqlite3_close(db);
}

void do_add_words_add(GtkButton *button, gpointer user_data)
{
	sqlite3 *db;
	sqlite3_open(dbname, &db);
	const char * outside = gtk_entry_get_text(GTK_ENTRY(entry_outside));
    	const char * inside = gtk_entry_get_text(GTK_ENTRY(entry_inside));
    	char sql1[100] = "";
    	sprintf(sql1,"select * from word where inside = \'%s\';",inside);
    	char *errmsg = NULL;
    	int r = 0, c = 0;
    	char ** table = NULL;
    	sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
    	if (errmsg!=NULL)
    	{
        	printf("%s\n", errmsg);
    	}	
    	if (r!=0)
    	{
        	GtkWidget *dialog_tuifang_queding;
        	dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"Already Existed", "title");
        	gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
        	gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
        	gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
        	gtk_widget_destroy(dialog_tuifang_queding);
		gtk_widget_hide_all(window_add);
        	sqlite3_close(db);
        	return;
    	}
    	char sql2[100] = "";
    	sprintf(sql2,"insert into word values (\'%s\',\'%s\',3);", inside, outside);
    	errmsg = NULL;
    	sqlite3_exec(db,sql2,NULL,NULL,&errmsg);
    	if (errmsg!=NULL)
    	{
        	printf("%s\n", errmsg);
        	sqlite3_close(db);
        	return;
    	}
	sqlite3_close(db);
	gtk_entry_set_text(GTK_ENTRY(entry_inside),"");
    	gtk_entry_set_text(GTK_ENTRY(entry_outside),"");
	gtk_widget_hide_all(window_add);

}

void do_delete_words_delete(GtkButton *button, gpointer user_data)
{
        sqlite3 *db;
        sqlite3_open(dbname, &db);
        const char * inside = gtk_entry_get_text(GTK_ENTRY(entry_inside2));
        char sql1[100] = "";
        sprintf(sql1,"select * from word where inside = \'%s\';",inside);
        char *errmsg = NULL;
        int r = 0, c = 0;
        char ** table = NULL;
        sqlite3_get_table(db,sql1,&table,&r,&c,&errmsg);
        if (errmsg!=NULL)
        {
                printf("%s\n", errmsg);
        }
        if (r!=0)
        {
                char sql2[100] = "";
                sprintf(sql2,"DELETE FROM word where inside = \'%s\';", inside);
                errmsg = NULL;
                sqlite3_exec(db,sql2,NULL,NULL,&errmsg);
                if (errmsg!=NULL)
                {
                        printf("%s\n", errmsg);
                        sqlite3_close(db);
                        gtk_widget_hide_all(window_delete);
                        return;
                }
        }
        else
        {
                GtkWidget *dialog_tuifang_queding;
                dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"Not Existed", "title");
                gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
                gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
                gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
		gtk_widget_destroy(dialog_tuifang_queding);
                gtk_widget_hide_all(window_delete);
                sqlite3_close(db);
                return;
        }
        sqlite3_close(db);
        gtk_entry_set_text(GTK_ENTRY(entry_inside2), "");
        gtk_entry_set_text(GTK_ENTRY(entry_inside), inside);
        gtk_widget_hide_all(window_delete);
	g_object_unref(window_delete);
	gtk_widget_hide_all(window_deck);
        g_object_ref(window_deck);
        my_window_deck_init();
        gtk_widget_show_all(window_deck);
	
}

void do_deck_refresh(GtkButton *button, gpointer user_data)
{
        gtk_widget_hide_all(window_deck);
        g_object_ref(window_deck);
	my_window_deck_init();
        gtk_widget_show_all(window_deck);
}

void do_test_back(GtkButton *button, gpointer user_data)
{
        gtk_widget_hide_all(window_test);
	g_object_ref(window_test);
        gtk_widget_show_all(window_menu);
}


void do_test_easy(GtkButton *button, gpointer user_data)
{
	GtkWidget *dialog_tuifang_queding;
        dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"WIP", "title");
	gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
        gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
        gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
        gtk_widget_destroy(dialog_tuifang_queding);
        gtk_widget_hide_all(window_add);
}

void do_test_normal(GtkButton *button, gpointer user_data)
{
        GtkWidget *dialog_tuifang_queding;
        dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"WIP", "title");
        gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
        gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
        gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
        gtk_widget_destroy(dialog_tuifang_queding);
        gtk_widget_hide_all(window_add);
}

void do_test_hard(GtkButton *button, gpointer user_data)
{
	/*char word[100];
        strcpy(word, word_array[i]._word);
        char explain[100];
        strcpy(explain, word_array[i]._explain);
	gtk_widget_hide(label_word);
	gtk_widget_hide(label_exp);
	g_object_ref(label_word);
	g_object_ref(label_exp);
	window_test = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_widget_set_size_request(window_test,350,150);
        int i, i_old;
        int m = n - 1;
        i = rand() % (m + 1 - 0) + 0;
        i_old = i;
	label_word = gtk_label_new(word);
        char command[100];
        strcpy(command, "<span foreground = 'blue' font_desc = '20'>");
        strcat(command, word);
        strcat(command, "</span>");
        puts(command);
        gtk_label_set_markup(GTK_LABEL(label_word), command) ;
        gtk_table_attach_defaults(GTK_TABLE(table),label_word,1,3,0,2);
        label_exp = gtk_label_new(explain);
        strcpy(command, "<span foreground = 'green' font_desc = '14'>");
        strcat(command, explain);
        strcat(command, "</span>");
        puts(command);
        gtk_label_set_markup(GTK_LABEL(label_exp), command);
        gtk_table_attach_defaults(GTK_TABLE(table),label_exp,1,3,2,3);

	gtk_widget_show(label_word);
        gtk_widget_show(label_exp);
	*/
	gtk_widget_hide_all(window_test);
	g_object_ref(window_test);
	my_window_test_init();
	gtk_widget_show_all(window_test);
	
}






gboolean my_keypress_function (GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_space){
	GtkWidget *dialog_tuifang_queding;
        dialog_tuifang_queding = gtk_message_dialog_new( (gpointer)window_main,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_OTHER, GTK_BUTTONS_OK,"This menu is still work in progress", "title");
        gtk_window_set_title(GTK_WINDOW(dialog_tuifang_queding), "Message");
        gtk_widget_set_size_request(dialog_tuifang_queding,450,100);
        gtk_dialog_run(GTK_DIALOG(dialog_tuifang_queding));
        gtk_widget_destroy(dialog_tuifang_queding);
	return TRUE;
    }
    return FALSE;
}


//--------------------界面初始化区

void my_window_main_init(void)
{

    	window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window_main), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window_main), "helloWORlD GUI v0.1.0");
	GtkWidget* table = gtk_table_new(4,3,TRUE);
    	gtk_container_add(GTK_CONTAINER(window_main),table);
    	GtkWidget* label_title = gtk_label_new("hello-WORlD GUI");
    	gtk_table_attach_defaults(GTK_TABLE(table),label_title,0,3,0,1);

    	GtkWidget* label_id = gtk_label_new("User id：");
    	gtk_table_attach_defaults(GTK_TABLE(table),label_id,0,1,1,2);
    	GtkWidget* label_passwd = gtk_label_new("Password：");
    	gtk_table_attach_defaults(GTK_TABLE(table),label_passwd,0,1,2,3);

    	entry_w_m_id = gtk_entry_new();
    	gtk_table_attach_defaults(GTK_TABLE(table),entry_w_m_id,1,3,1,2);
    	entry_w_m_passwd = gtk_entry_new();
	gtk_entry_set_visibility(entry_w_m_passwd, FALSE);
    	gtk_table_attach_defaults(GTK_TABLE(table),entry_w_m_passwd,1,3,2,3);

    	GtkWidget* button_register = gtk_button_new_with_label("Register");
    	gtk_table_attach_defaults(GTK_TABLE(table),button_register,2,3,3,4);
    	GtkWidget* button_login = gtk_button_new_with_label("Login");
    	gtk_table_attach_defaults(GTK_TABLE(table),button_login,0,1,3,4);
    	g_signal_connect(button_register,"pressed",G_CALLBACK(window_main_reg),NULL);
    	g_signal_connect(button_login,"pressed",G_CALLBACK(window_main_log),NULL);

}

void my_window_menu_init(void)
{
    	window_menu = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_add_events(window_main, GDK_KEY_PRESS_MASK);    
	gtk_window_set_title(GTK_WINDOW(window_menu), "Start Page");
	gtk_window_set_position(GTK_WINDOW(window_menu), GTK_WIN_POS_CENTER); 
    	GtkWidget* table = gtk_table_new(3,3,TRUE);
    	gtk_container_add(GTK_CONTAINER(window_menu),table);
    	GtkWidget* label_title = gtk_label_new("hello-WORlD GUI");
    	gtk_table_attach_defaults(GTK_TABLE(table),label_title,0,3,0,1);

    
    	GtkWidget* button_show_all = gtk_button_new_with_label("Show All");
    	gtk_table_attach_defaults(GTK_TABLE(table),button_show_all,0,1,2,3);
    	GtkWidget* button_start_test = gtk_button_new_with_label("Start Test");
    	gtk_table_attach_defaults(GTK_TABLE(table),button_start_test,1,2,2,3);
    	GtkWidget* button_quit = gtk_button_new_with_label("Sign Out");
    	gtk_table_attach_defaults(GTK_TABLE(table),button_quit,2,3,2,3);

    	g_signal_connect(button_quit, "pressed", G_CALLBACK(do_sign_out), NULL);
    	g_signal_connect(button_start_test, "pressed", G_CALLBACK(do_test), NULL);
    	g_signal_connect(button_show_all, "pressed", G_CALLBACK(do_show_all), NULL);


}


void load_image(GtkWidget *image, const char *file_path, const int w, const int h )
{
	gtk_image_clear( GTK_IMAGE(image) );        // 清除图像
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);	// 创建图片资源
	GdkPixbuf *dest_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR); // 指定大小
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), dest_pixbuf); // 图片控件重新设置一张图片(pixbuf)
	g_object_unref(src_pixbuf);		// 释放资源
	g_object_unref(dest_pixbuf);	// 释放资源
}

void change_background(GtkWidget *widget, int w, int h, const gchar *path)
{
	gtk_widget_set_app_paintable(widget, TRUE);		//允许窗口可以绘图
	gtk_widget_realize(widget);	
 
	/* 更改背景图时，图片会重叠
	 * 这时要手动调用下面的函数，让窗口绘图区域失效，产生窗口重绘制事件（即 expose 事件）。
	 */
	gtk_widget_queue_draw(widget);
 
	GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(path, NULL);	// 创建图片资源对象
	// w, h是指定图片的宽度和高度
	GdkPixbuf *dst_pixbuf = gdk_pixbuf_scale_simple(src_pixbuf, w, h, GDK_INTERP_BILINEAR);
 
	GdkPixmap *pixmap = NULL;
 
	/* 创建pixmap图像; 
	 * NULL：不需要蒙版; 
	 * 123： 0~255，透明到不透明
	 */
	gdk_pixbuf_render_pixmap_and_mask(dst_pixbuf, &pixmap, NULL, 128);
	// 通过pixmap给widget设置一张背景图，最后一个参数必须为: FASLE
	gdk_window_set_back_pixmap(widget->window, pixmap, FALSE);
 
	// 释放资源
	g_object_unref(src_pixbuf);
	g_object_unref(dst_pixbuf);
	g_object_unref(pixmap);
}

void my_window_test_init(void)
{	

	window_test = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window_test,350,150);
	int i, i_old;
	int m = n - 1;
	char word[100];
        char explain[100];
	if(m > -1)
	{
		i = rand() % (m + 1 - 0) + 0;
		i_old = i;
	/*if(*word_array[i]._power == 3)
		return;
	if(*word_array[i]._power == 2)
		i = rand(0, m);
		if(*word_array[i]._power == 3)
			return;
		else
			i = i_old;
	if(*word_array[i]._power == 1)
		i = rand(0, m);
                if(*word_array[i]._power >= 2)
                  	i_old = i;
			i = rand(0, m);
			if(*word_array[i]._power == 3)
				return;
			else
				i = i_old;
  		else
			i = rand(0, m);
			if(*word_array[i]._power >= 2)
                                i_old = i;
                                i = rand(0, m);
                                if(*word_array[i]._power == 3)
                                        return;
                                else
                                        i = i_old;
	*/

		strcpy(word, word_array[i]._word);
		strcpy(explain, word_array[i]._explain);
	}
	else
	{
		strcpy(word, "No word!");
		strcpy(explain, "No definition");
	}
	gtk_widget_add_events(window_test, GDK_KEY_PRESS_MASK);
    	gtk_window_set_position(GTK_WINDOW(window_test), GTK_WIN_POS_CENTER);
    	change_background(window_test, 640, 360, "./bg.jpg");
	GtkWidget* table = gtk_table_new(4,4,TRUE);
    	gtk_container_add(GTK_CONTAINER(window_test),table);
	gtk_window_set_title(GTK_WINDOW(window_test), "helloTEST!");
    	label_word = gtk_label_new(word);
	char command[100];
	strcpy(command, "<span foreground = 'blue' font_desc = '20'>");
	strcat(command, word);
	strcat(command, "</span>");
	puts(command);
	gtk_label_set_markup(GTK_LABEL(label_word), command) ;
    	gtk_table_attach_defaults(GTK_TABLE(table),label_word,1,3,0,2);
	label_exp = gtk_label_new(explain);
	strcpy(command, "<span foreground = 'green' font_desc = '14'>");
        strcat(command, explain);
        strcat(command, "</span>");
        puts(command);
        gtk_label_set_markup(GTK_LABEL(label_exp), command);
        gtk_table_attach_defaults(GTK_TABLE(table),label_exp,1,3,2,3);
	GtkWidget* button_easy = gtk_button_new_with_label("Easy");
    	gtk_table_attach_defaults(GTK_TABLE(table),button_easy,0,1,3,4);
	GtkWidget* button_normal = gtk_button_new_with_label("Normal");
        gtk_table_attach_defaults(GTK_TABLE(table),button_normal,1,2,3,4);
	GdkColor color;
	GtkWidget* button_hard = gtk_button_new_with_label("Hard");
	GtkWidget* button_back = gtk_button_new_with_label("Back");
        gtk_table_attach_defaults(GTK_TABLE(table),button_hard,2,3,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),button_back,3,4,3,4);
	g_signal_connect(button_back, "pressed", G_CALLBACK(do_test_back), NULL);
	g_signal_connect(button_easy, "pressed", G_CALLBACK(do_test_easy), NULL);
	g_signal_connect(button_normal, "pressed", G_CALLBACK(do_test_normal), NULL);
	g_signal_connect(button_hard, "pressed", G_CALLBACK(do_test_hard), NULL);
	g_signal_connect(window_test,"destroy",G_CALLBACK(do_test_back),NULL);
	g_signal_connect (G_OBJECT (window_test), "key_press_event",
        G_CALLBACK (my_keypress_function), NULL);

}



void my_window_deck_init(void)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
   	GtkListStore *store;
   	GtkWidget *window;
   	GtkWidget *list;

   	GtkWidget *label;
   	GtkTreeSelection *selection;




   	store = gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
	

   	sqlite3 *db;
   	char *err_msg = 0;
   	int rc = sqlite3_open(dbname, &db);

   	if (rc != SQLITE_OK) 
	{	
		fprintf(stderr, "Cannot open database: %s\n",
               	sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	char *sql = "";
	sql = "SELECT * FROM word";
	rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   	if (rc != SQLITE_OK ) 
	{
		fprintf(stderr, "Failed to select data\n");
      		fprintf(stderr, "SQL error: %s\n", err_msg);

      		sqlite3_free(err_msg);
      		sqlite3_close(db);
       	}

  	sqlite3_close(db);

   	window_deck = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   	list = gtk_tree_view_new();

		
   	renderer = gtk_cell_renderer_text_new ();
   	column = gtk_tree_view_column_new_with_attributes("Inside", renderer, "text", LIST_IN, NULL);
   	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes("Outside", renderer, "text", LIST_OUT, NULL);
   	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);


   	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   	g_object_unref(store);
	
	gtk_window_set_title(GTK_WINDOW(window_deck), "helloWORD");
   	gtk_window_set_position(GTK_WINDOW(window_deck), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window_deck), 10);
   	gtk_window_set_default_size(GTK_WINDOW(window_deck), 270, 250);

   	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   	//GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	GtkWidget *s_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_add_with_viewport (s_window, list);
	GtkWidget *table = gtk_table_new(10, 5, TRUE);

   	
	GtkWidget *button_delete = gtk_button_new_with_label("Delete");
        gtk_table_attach_defaults(GTK_TABLE(table), button_delete, 3, 4, 0, 1);
	GtkWidget *button_add_new = gtk_button_new_with_label("Add");
        gtk_table_attach_defaults(GTK_TABLE(table), button_add_new, 4, 5, 0, 1);
	GtkWidget *button_back = gtk_button_new_with_label("Back");
        gtk_table_attach_defaults(GTK_TABLE(table), button_back, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(table), s_window, 0, 5, 1, 10);
	//gtk_box_pack_start(GTK_BOX(vbox), button_refresh, TRUE, TRUE, 0);
	//gtk_box_pack_start(GTK_BOX(vbox), button_add_new, TRUE, TRUE, 0);
	//gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
	
	//label = gtk_label_new("");
        //gtk_table_attach_defaults(GTK_TABLE(table), label);
        //gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

        gtk_container_add(GTK_CONTAINER(window_deck), table);

        g_signal_connect(button_add_new, "pressed", G_CALLBACK(do_add_word), NULL);
        g_signal_connect(button_delete, "pressed", G_CALLBACK(do_deck_delete), NULL);
        g_signal_connect(button_back, "pressed", G_CALLBACK(do_deck_back), NULL);
        g_signal_connect(window_deck,"destroy",G_CALLBACK(gtk_main_quit),NULL);


}
int callback(void *model, int argc, char **argv, char **azColName)
{
	GtkTreeIter iter;

   	for (int i = 0; i < argc; i++)    
	{
	}


   // AFTER PRINTING TO CONSOLE FILL THE MODEL WITH THE DATA
	strcpy(word_array[n]._word, argv[0]);
	strcpy(word_array[n]._explain, argv[1]);
	strcpy(word_array[n]._power, argv[2]);
	word_array[n]._unique_id = n;
	n++;


   	gtk_list_store_append (GTK_LIST_STORE(model), &iter);
   	gtk_list_store_set (GTK_LIST_STORE(model), &iter, LIST_IN, argv[0], LIST_OUT, argv[1], -1);

   return 0;
}

void my_window_add_init(void)
{
        window_add = gtk_window_new(GTK_WINDOW_TOPLEVEL);


        GtkWidget* table = gtk_table_new(3,10,TRUE);
	gtk_window_set_title(GTK_WINDOW(window_add), "WORD+");
        gtk_container_add(GTK_CONTAINER(window_add), table);
        gtk_window_set_position(GTK_WINDOW(window_add), GTK_WIN_POS_CENTER);
        GtkWidget* label_word = gtk_label_new("Word：");
        gtk_table_attach_defaults(GTK_TABLE(table),label_word,0,2,0,1);
        GtkWidget* label_exp = gtk_label_new("Explaination：");
        gtk_table_attach_defaults(GTK_TABLE(table),label_exp,0,2,1,2);

        entry_inside = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(table),entry_inside,2,9,0,1);
        entry_outside = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(table),entry_outside,2,9,1,2);

        GtkWidget *button_look_up = gtk_button_new_with_label("Look Up");
        gtk_table_attach_defaults(GTK_TABLE(table), button_look_up,8,9,2,3);
        GtkWidget *button_add = gtk_button_new_with_label("Add");
        gtk_table_attach_defaults(GTK_TABLE(table), button_add,0,8,2,3);
        GtkWidget *button_back = gtk_button_new_with_label("Back");
        gtk_table_attach_defaults(GTK_TABLE(table), button_back,9,10,2,3);

        g_signal_connect(button_add, "pressed", G_CALLBACK(do_add_words_add), NULL);
        g_signal_connect(button_look_up, "pressed", G_CALLBACK(do_add_words_look_up), NULL);
        g_signal_connect(button_add, "pressed", G_CALLBACK(do_deck_refresh), NULL);
        g_signal_connect(button_back, "pressed", G_CALLBACK(do_add_back), NULL);
        g_signal_connect(window_add,"destroy",G_CALLBACK(do_add_back),NULL);

}

void my_window_delete_init(void)
{
        window_delete = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(window_delete), "WORD-");
        GtkWidget* table = gtk_table_new(3,10,TRUE);
        gtk_container_add(GTK_CONTAINER(window_delete), table);
        gtk_window_set_position(GTK_WINDOW(window_delete), GTK_WIN_POS_CENTER);
        GtkWidget* label_word = gtk_label_new("Word：");
        gtk_table_attach_defaults(GTK_TABLE(table),label_word,0,2,0,1);
        //GtkWidget* label_exp = gtk_label_new("Explaination：");
        //gtk_table_attach_defaults(GTK_TABLE(table),label_exp,0,2,1,2);

        entry_inside2 = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(table),entry_inside2,2,9,0,1);

        GtkWidget *button_delete = gtk_button_new_with_label("Delete");
        gtk_table_attach_defaults(GTK_TABLE(table), button_delete,0,9,2,3);
        GtkWidget *button_back = gtk_button_new_with_label("Back");
        gtk_table_attach_defaults(GTK_TABLE(table), button_back,9,10,2,3);

        g_signal_connect(button_delete, "pressed", G_CALLBACK(do_delete_words_delete), NULL);
        g_signal_connect(button_back, "pressed", G_CALLBACK(do_delete_back), NULL);
        g_signal_connect(window_delete,"destroy",G_CALLBACK(do_delete_back),NULL);

}



int main(int argc, char  *argv[])
{
    my_sql_init();
    gtk_init(&argc,&argv);
	
    my_window_main_init();
    //my_window_deck_init();
    //my_window_menu_init();
    //my_window_test_init();

    gtk_widget_show_all(window_main);
    g_signal_connect(window_main,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    gtk_main();
    return 0;
}
