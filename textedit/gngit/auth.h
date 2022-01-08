#ifdef __cplusplus
extern "C" {
#endif

#include "git2.h"

extern char user_name[100];
extern char passwd[100];

int credsSSL(git_credential **out, 
                const char *url, 
                const char *username_from_url, 
                unsigned int allowed_types, 
                void *payload);

#ifdef __cplusplus
}
#endif