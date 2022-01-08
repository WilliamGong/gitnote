#pragma once

#include <string>
#include <vector>
#include <cstring>
#include "git2.h"

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
    class Repo {
        public:
            Repo();
            ~Repo();
            int init(std::string path);
            int open(std::string path);

            // status
            void statusUpdate();
            inline git_status_list* getStatus() {
                return this->stat;
            }

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

        private: 
            git_repository *repo = nullptr;

            // git status
            git_status_list *stat = nullptr;
            // index
            git_index *index = nullptr;

            std::string cwd;
    };
};