include_directories(${CMAKE_SOURCE_DIR}/libgit2/include)

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTORCC OFF)
set(CMAKE_AUTOUIC OFF)

set(HEADER_FILES 
    repo.h 
    auth.h 
    )

set(SOURCE_FILES 
    repo.cpp 
    auth.c 
    )

add_library(gngit 
            STATIC 
            ${HEADER_FILES} 
            ${SOURCE_FILES} 
            )

target_link_libraries(gngit 
                        PRIVATE 
                        git2 
                        )