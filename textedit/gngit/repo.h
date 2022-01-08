#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <assert.h>

#include "auth.h"

struct merge_options {
	const char **heads;
	size_t heads_count;

	git_annotated_commit **annotated;
	size_t annotated_count;

	int no_commit : 1;
};

// This are all from libgit2/examples
static int resolve_heads_s(git_repository *repo, merge_options *opts);
int resolve_heads(git_repository *repo, merge_options *opts);
int resolve_refish(git_annotated_commit **commit, git_repository *repo, const char *refish);
static int perform_fastforward_s(git_repository *repo, const git_oid *target_oid, int is_unborn);
int perform_fastforward(git_repository *repo, const git_oid *target_oid, int is_unborn);

namespace gitnote {

    enum index_mode {
	    INDEX_NONE,
	    INDEX_ADD
    };

    struct index_options {
	    int dry_run;
	    int verbose;
	    git_repository *repo;
	    enum index_mode mode;
	    int add_update;
    };

    struct remoteInfo {
        std::string name;
        std::string fetchUrl;
        std::string pushUrl;
    };

    struct authSSLInfo {
        std::string username;
        std::string password;
    };


    class Repo {
        public:
            Repo();
            ~Repo();
            int init(std::string path);
            int open(std::string path);

            // status
            void getStatus(git_status_list **stat);

            // add and commit
            void addAll();
            int commit();

            // remote
            std::vector<std::string> getRemotesList();
            int remoteAdd(std::string name, std::string url);
            int remoteRemove(std::string name);
            remoteInfo getRemoteInfo(std::string name);
            int rename(std::vector<char *> &problems, std::string oldName, std::string newName);
            int setFetchUrl(std::string name, std::string url);
            int setPushUrl(std::string name, std::string url);

            // push and fetch 
            int push(authSSLInfo auth);
            int fetch();
            int merge(std::string commit);

            //auth
        private: 
            git_repository *repo = nullptr;

            // index
            git_index *index = nullptr;

            std::string cwd;
    };
};