#include "auth.h"

char user_name[100];
char passwd[100];

int credsSSL(git_credential **out, 
                        const char *url, 
                        const char *username_from_url, 
                        unsigned int allowed_types, 
                        void *payload) {

    return git_cred_userpass_plaintext_new(out, user_name, passwd);
}