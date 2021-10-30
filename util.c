/* See LICENSE file for copyright and license details. */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <stdlib.h>

#include "util.h"

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		die("calloc:");
	return p;
}

void
die(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	exit(1);
}

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

InOutPipeT dmenu_qry(const char *prompt, unsigned lines)
{
   InOutPipeT ret = {0, 0};

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

      if (lines == 0)
         execlp("dmenu", "dmenu", "-i", "-p", prompt, NULL);
         /*execlp("rofi", "rofi", "-dmenu", "-i", "-p", prompt, NULL);*/
      else
      {
         char str[12 + 1];
         str[12] = '\0';

         snprintf(str, 11, "%u", lines);

         execlp("dmenu", "dmenu", "-i", "-p", prompt, "-l", str, NULL);
         /*execlp("rofi", "rofi", "-dmenu", "-i", "-p", prompt, "-l", str, NULL);*/
      }
      exit(1);
   }

   close(p_in[0]);
   close(p_out[1]);

   ret.out = p_in[1];
   ret.in = p_out[0];

cleanup:
   return ret;
}

char* dmenu_rsp(InOutPipeT fd)
{
   char *retstr = NULL;
   char *trimmed = NULL;

   char buf[256 + 1];
   buf[256] = '\0';
   ssize_t size = 0;

   size = read(fd.in, buf, 256);
   if (size == -1) goto done;
   buf[size] = '\0';
   trimmed = str_trim(buf);
   retstr = strdup(trimmed);

done:
   close(fd.in);
   return retstr;
}
