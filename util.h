/* See LICENSE file for copyright and license details. */

#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

#define free_not_null(p) {if (p != NULL) { free(p); p = NULL; }}

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);
char* str_trim(char *str);

typedef struct {
   int in;
   int out;
} InOutPipeT;

InOutPipeT dmenu_qry(const char *prompt);
char* dmenu_rsp(InOutPipeT fd);
