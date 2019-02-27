#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <stdlib.h>

#include "findwin.h"

#define APP_CACHE "~/.cache/applications.cache"
#define APP_CACHE_SEPARATOR '/'

#define free_not_null(p) {if (p != NULL) { free(p); p = NULL; }}

char* str_trim(char *str)
{
   char *p = str;
   while ((*p == ' ' || *p == '\t' || *p == '\n') && *p != '\0')
      p ++;
   char *p_end = &p[strlen(p)];
   while (p_end > p && (*p_end == ' ' || *p_end == '\t' || *p_end == '\n' || *p_end == '\0'))
   {
      *p_end = '\0';
      p_end --;
   }

   return p;
}

char* run_dmenu(const char *prompt, char **variants)
{
   char *retstr = NULL;

   int p_in[2];
   int p_out[2];

   pipe(p_in);
   pipe(p_out);

   pid_t pid = fork();
   if (pid == -1) goto cleanup;

   if (pid == 0)
   {
      close(p_in[1]);
      close(p_out[0]);

      if (dup2(p_in[0], STDIN_FILENO) == -1) goto cleanup;
      if (dup2(p_out[1], STDOUT_FILENO) == -1) goto cleanup;

      execlp("dmenu", "dmenu", "-i", "-p", prompt, NULL);
      exit(1);
   }

   close(p_in[0]);
   close(p_out[1]);

   char buf[256 + 1];
   buf[256] = '\0';

   char **v;
   for (v = variants; *v; v ++)
   {
      strncpy(buf, *v, 256);
      write(p_in[1], buf, strlen(buf));
      write(p_in[1], "\n", 1);
   }

   close(p_in[1]);

   ssize_t size = 0;
   size = read(p_out[0], buf, 256);
   if (size == -1) goto cleanup;
   buf[size] = '\0';
   retstr = (char*) malloc(strlen(buf) + 1);
   if (!retstr) goto cleanup;
   strcpy(retstr, buf);

cleanup:
   close(p_in[1]);
   close(p_out[0]);
   return retstr;
}

void findwin(const Arg *arg)
{
   Client *c;

   char *variants[256 + 1];
   variants[256] = NULL;
   unsigned vi = 0;

   /* Create a list of all client titles */
   for (c = selmon->clients; c; c = c->next)
   {
      if (vi >= 256) break;
      variants[vi] = c->name;
      vi ++;
   }
   variants[vi] = NULL;

   /* Get the user input */
   char *selection = run_dmenu("windows>", variants);
   if (!selection) goto cleanup;
   char *sel = str_trim(selection);

   /* Find the selected window and give it a focus */
   for (c = selmon->clients; c; c = c->next)
   {
      if (strcmp(c->name, sel) == 0)
      {
         unsigned first_tag = 1;
         while (!(first_tag & c->tags) && first_tag < LENGTH(tags))
            first_tag <<= 1;

         selmon->tagset[selmon->seltags] = first_tag;
         focus(c);
         arrange(selmon);
      }
   }

cleanup:
   free_not_null(selection);
}

void run_app(const Arg *arg)
{
   // Run dmenu to get the user's input
   FILE *pp = popen("dmenu -i -l 10 -p 'applications>' < " APP_CACHE, "r");
   if (!pp) goto cleanup;

   char buf[1024 + 1];
   buf[1024] = '\0';

   fgets(buf, 1024, pp);

   // Find the last entry (the .desktop file name)
   char *p = rindex(buf, APP_CACHE_SEPARATOR);
   if (!p) goto cleanup;
   p ++;
   p = str_trim(p);

   // Execute the entry
   if (fork() == 0)
   {
      execlp("gtk-launch", "gtk-launch", p, NULL);
      exit(1);
   }

cleanup:
   if (pp) fclose(pp);
}

#if 0
int main(int argc, char **argv)
{
   run_app();
   return 0;
}
#endif
