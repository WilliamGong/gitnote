#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <functional>

#include "auth.h"

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

            // push and pull
            int push(authSSLInfo auth);

            //auth
        private: 
            git_repository *repo = nullptr;

            // index
            git_index *index = nullptr;

            std::string cwd;
    };
};