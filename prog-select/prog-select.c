// Author: Jasper den Ouden
//Placed in public domain.

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include <gtk/gtk.h>

void destroy (void)
{
  gtk_main_quit ();
}

typedef struct Freq /* Frequencies of various strings. */
{
  int cnt; /* How often seen. */
  time_t t; /* Last time seen. */
  char* str; /* The string seen */
} Freq;

int freq_cnt=0;
Freq* freq;

int freq_match(Freq* f, char* str)
{
  assert(str!=NULL); 
  int i=0;
  while( f->str[i]!='\0' && str[i]!='\0' )
    { if( str[i]!=f->str[i] ){ return 0; }
      i++;
    }
  return 1;
}

GtkWidget* text; GtkWidget* vbox; GtkWidget* topbox;
int match_cnt_max= 64; int* match;

void do_system(const char* str)
{
  const int len= strlen(str);
  char* tmp=malloc(len+2);
  strcpy(tmp, str);
  tmp[len]='&'; tmp[len+1]='\0';
  system(tmp);
  free(tmp);
}

void freq_choice (int* index)
{
  assert( index!=NULL );
  const int i= *index;
  assert( i>=0 ); assert( i< freq_cnt );
  freq[i].cnt++;  /* Register use. */
  freq[i].t= time(NULL);
  do_system(freq[i].str); /* Do call. */
  app_finish();
}

int match_compare(void* vi,void* vj)
{
  const int* i=(int*)vi, *j=(int*)vj;
  return freq[*i].cnt < freq[*j].cnt;
}

char* prev_str= NULL;

GtkWidget* window;

void enter_button (GtkWidget* button)
{
  GtkButton* b= GTK_BUTTON(button);
  gtk_button_set_relief(b,GTK_RELIEF_NORMAL);
//  gtk_button_set_use_underline(b, TRUE);
}
void leave_button (GtkWidget* button)
{
  GtkButton* b= GTK_BUTTON(button);
  gtk_button_set_relief(b,GTK_RELIEF_NONE);
//  gtk_button_set_use_underline(b, FALSE);
}

void continue_with_button(int* index)
{
  gtk_widget_grab_focus(GTK_WIDGET(text));
  
  assert( index!=NULL );
  const int i= *index;
  assert( i>=0 ); assert( i< freq_cnt );
  
  gtk_entry_set_text(GTK_ENTRY(text), freq[i].str );
  
  text_change();
}

void text_change()
{
  char* str= (char*)gtk_entry_get_text(GTK_ENTRY(text));
  if( prev_str==NULL ? 0 : !strcmp( str, prev_str ) )
    { return; }
  prev_str= malloc(strlen(str)+1);
  strcpy(prev_str,str);
    
  gtk_container_remove (GTK_CONTAINER (topbox), vbox);
  vbox= gtk_vbox_new(FALSE,0);
  gtk_box_set_spacing(GTK_BOX(vbox),0);
  
  gtk_box_pack_start(GTK_BOX(topbox), vbox, FALSE,FALSE,0);
  
  int i=0, match_cnt= 0;
  while( i< freq_cnt && match_cnt< match_cnt_max )
    { if( freq_match(freq + i, str) )
	{ match[match_cnt++]= i; }
      i++;
    }
  qsort((void*)match,match_cnt,sizeof(int), match_compare);
  i=0;
  while( i< match_cnt )
    {
      GtkWidget* b= gtk_button_new_with_label(freq[match[i]].str);
      gtk_button_set_relief(b,GTK_RELIEF_NONE);
      gtk_box_pack_start(GTK_BOX(vbox), b, FALSE,FALSE,0); 
      gtk_signal_connect_object
	(GTK_OBJECT (b), "clicked", GTK_SIGNAL_FUNC(freq_choice), 
	 match+ i);
      gtk_signal_connect_object
	( GTK_OBJECT(b), "enter-notify-event",
	  GTK_SIGNAL_FUNC(enter_button), b );
      gtk_signal_connect_object
	( GTK_OBJECT(b), "leave-notify-event",
	  GTK_SIGNAL_FUNC(leave_button), b );
      i++;
    }
  
  gtk_widget_show_all(window);
}

void entry_accept () /* Accepted the command as in entry, using it.*/
{
  const char* str= (char*)gtk_entry_get_text(GTK_ENTRY(text));
  int i=0;
  while( i< freq_cnt )
    {
      if( !strcmp(freq[i].str,str) )
	{ freq_choice(&i);return; }
      i++;
    }
  const int len= strlen(str);
  if( len==0 )
    { app_finish(); return; }
  
  Freq* f= freq + freq_cnt++;
  f->str= malloc(len+2);
  strcpy(f->str, str);
  f->cnt=1; f->t= time(NULL);
  
  do_system(str);
  
  app_finish();
}

char* filepos=NULL;

void init ()
{
  struct passwd* pw= getpwuid( geteuid() );
  assert( pw!=NULL );

  filepos= pw->pw_dir;
  assert( filepos!=NULL );
  filepos= strncat( filepos, "/.prog-select",strlen(filepos)+12 );
  
  FILE* f= fopen(filepos,"r");
  if( f==NULL )
    { freq_cnt=0; freq= malloc(sizeof(Freq)); }
  else
    {
      fscanf(f, "%d", &freq_cnt); /* Number of elements. */
      int i=0;
    /* Create one extra incase we find a new one. */
      freq= (Freq*)malloc(sizeof(Freq)*(freq_cnt+1));
      char tmp[1024];
      while( i< freq_cnt )
	{
	  fscanf(f,"%d %ld", &freq[i].cnt, &freq[i].t);
/* Need to get string manually; that should stop only at newlines. */
	  char ch= getc(f); 
	  while( ch=='\t' || ch == ' ' )
	    {
	      assert( !feof(f) ); assert( !ferror(f) );
	      assert( ch!= '\n' );
	      ch=getc(f);
	    }
	  int j=0;
	  while( j<1024 && ch!='\n' && !feof(f) && !ferror(f) )
	    { tmp[j++]= ch;
	      ch= getc(f);
	    }
	  tmp[j]='\0';
	  printf("%s\n", tmp);
	  freq[i].str=(char*)malloc(strlen(tmp)+1);
	  strcpy(freq[i].str ,tmp);
	  i++;
	}
      fclose(f);
    }
  match= (int*)malloc(sizeof(int)*match_cnt_max);
}

/* Function to open a dialog box displaying the message provided. */
void quick_message (gchar *message) {
   GtkWidget *dialog, *label, *content_area;
   /* Create the widgets */
   dialog = gtk_dialog_new_with_buttons 
     ("Message", window, GTK_DIALOG_DESTROY_WITH_PARENT,
      GTK_STOCK_OK, GTK_RESPONSE_NONE, NULL);
   content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
   label = gtk_label_new (message);
   /* Ensure that the dialog box is destroyed when the user responds. */
   g_signal_connect_swapped 
     (dialog,"response", G_CALLBACK (gtk_widget_destroy), dialog);
   /* Add the label, and show everything we've added to the dialog. */
   gtk_container_add (GTK_CONTAINER (content_area), label);
   gtk_widget_show_all (dialog);
}

void app_finish ()
{
  /* Writing while freeing. */
  FILE* f= fopen(filepos,"w");
  if( f==NULL )
    { char tmp[1024];
      sprintf(tmp,"Error, couldn't open/create file %s\nThis is probably bugreport worthy.",
	      filepos);
      quick_message(tmp);
      return;
    }
  int i=0;
  fprintf(f,"%d\n", freq_cnt);
  while( i< freq_cnt )
    {
      const Freq* fr= freq+ i;
      fprintf(f,"%d\t%ld\t%s\n", fr->cnt, fr->t, fr->str);
      free(fr->str);
      i++;
    }
  fprintf(f,"\n");
  fclose(f);
  free(freq);
  
  if( prev_str!=NULL ){ free( prev_str ); }
  
  free(match);
  gtk_widget_destroy(window);
}

void entry_connect(const char* signal)
{
  gtk_signal_connect_full
    (G_OBJECT(text), signal,
     GTK_SIGNAL_FUNC(text_change), NULL, NULL, NULL, 0,
     GTK_RUN_BOTH);
}

int main (int argc, char *argv[])
{
  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(window, "run select");
  gtk_window_set_gravity(window, GDK_GRAVITY_NORTH_WEST);

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (destroy), NULL);
  gtk_container_border_width (GTK_CONTAINER (window), 10);

  topbox= gtk_vbox_new(FALSE,0);
  vbox= gtk_vbox_new(1,0);
 /* TODO tooltip/informative bit explaining what it does, about button. */
  text = gtk_entry_new();
  init();

  gtk_signal_connect( GTK_OBJECT(text), "activate",
		      GTK_SIGNAL_FUNC(entry_accept), NULL );  

  /* TODO doesn't activate in every case.. */
  /* TODO key for: best choice in first space.
                   on the choices: fill with that choice and continue
use
GtkAccelGroup*      gtk_accel_group_new()
gtk_accel_group_connect(accel_group, GDK_Tab,
0,GTK_ACCEL_VISIBLE, g_cclosure_new() );

get the current with gtk_window_get_focus(GtkWindow *window)

You could change the activating via button bit to do the same. 
(GDK_KP_Enter) saving having to add it to each button
   */
  entry_connect("key-press-event");
  entry_connect("insert-at-cursor");
  entry_connect("cut-clipboard");
  entry_connect("paste-clipboard");

  gtk_container_add (GTK_CONTAINER (window), topbox);

  gtk_box_pack_start(GTK_BOX(topbox), vbox, FALSE,FALSE,0);  
  gtk_box_pack_start(GTK_BOX(topbox), text, FALSE,FALSE,0);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
