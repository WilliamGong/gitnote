#pragma once

#include <string>
#include "git2.h"

namespace gitnote {
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

        private: 
            git_repository *repo = nullptr;

            // git status
            git_status_list *stat;

            std::string cwd;
    };
};