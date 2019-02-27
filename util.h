/* See LICENSE file for copyright and license details. */

#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define BETWEEN(X, A, B)        ((A) <= (X) && (X) <= (B))

#define free_not_null(p) {if (p != NULL) { free(p); p = NULL; }}

void die(const char *fmt, ...);
void *ecalloc(size_t nmemb, size_t size);
char* str_trim(char *str);
char* run_dmenu(const char *prompt, char **variants);
