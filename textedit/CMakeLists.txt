project(textedit)

include_directories(gngit)
include_directories(${CMAKE_SOURCE_DIR}/libgit2/include)
add_subdirectory(gngit)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.12.11/5.12.11/msvc2017_64/lib/cmake")
find_package(Qt5 COMPONENTS Core Widgets LinguistTools REQUIRED)


set(SRC_FILE 
    main.cpp 
    textedit.cpp 
    dialoggitstatus.cpp 
    dialoggitremote.cpp 
    dialoggitremoteadd.cpp 
    dialoggitremoteedit.cpp 
    dialogauthssl.cpp 
    )

set(HEADER_FILE 
    textedit.h 
    dialoggitstatus.h 
    dialoggitremote.h 
    dialoggitremoteadd.h 
    dialoggitremoteedit.h 
    dialogauthssl.h 
    )

set(FORM_FILE 
    textedit.ui 
    dialoggitstatus.ui 
    dialoggitremote.ui 
    dialoggitremoteadd.ui 
    dialoggitremoteedit.ui 
    dialogauthssl.ui 
    )

set(RESOURCES_FILE 
    textedit.qrc)

add_executable(textedit 
                WIN32 
                ${SRC_FILE} 
                ${HEADER_FILE} 
                ${FORM_FILE} 
                ${RESOURCES_FILE}
                )

target_link_libraries(${PROJECT_NAME}
                        PRIVATE 
                        Qt5::Core                       
                        Qt5::Widgets 
                        PUBLIC 
                        gngit 
                        )


