project(textedit)

find_package(Qt5 COMPONENTS Core Widgets LinguistTools REQUIRED)

# get deployqt location
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(DEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${_qt_bin_dir}")

include_directories(gngit)
include_directories(${CMAKE_SOURCE_DIR}/libgit2/include)
include_directories(${CMAKE_BINARY_DIR}/include)
add_subdirectory(gngit)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

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


if(WIN32)
        add_custom_command(TARGET textedit 
                                POST_BUILD 
                                COMMAND ${DEPLOYQT_EXECUTABLE} "$<TARGET_FILE:textedit>" 
                                --$<LOWER_CASE:$<CONFIG>>
                                )
endif()