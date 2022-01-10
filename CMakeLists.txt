cmake_minimum_required(VERSION 3.1.0)
project(gitnote VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Git)

execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE GIT_VERSION 
                OUTPUT_STRIP_TRAILING_WHITESPACE 
                )

configure_file(version.h.in include/version.h)

add_subdirectory(textedit)
add_subdirectory(libgit2)

install(TARGETS textedit git2 
        CONFIGURATIONS Debug 
        RUNTIME DESTINATION bin 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib
        )
install(FILES 
        textedit/textedit_zh.qm 
        textedit/qt_zh_CN.qm 
        CONFIGURATIONS Debug 
        DESTINATION bin 
        )

install(TARGETS textedit git2 
        CONFIGURATIONS Release
        RUNTIME DESTINATION bin 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib
        )
install(FILES 
        textedit/textedit_zh.qm 
        textedit/qt_zh_CN.qm 
        CONFIGURATIONS Release
        DESTINATION bin 
        )

if(WIN32)
        if(CMAKE_BUILD_TYPE EQUAL Debug)
                file(GLOB LIBS ${CMAKE_BINARY_DIR}/textedit/Debug/Qt5*.dll)
        elseif(CMAKE_BUILD_TYPE EQUAL Release)
                file(GLOB LIBS ${CMAKE_BINARY_DIR}/textedit/Release/Qt5*.dll)
        endif()
        install(PROGRAMS 
                ${CMAKE_BINARY_DIR}/textedit/Debug/Qt5Cored.dll 
                ${CMAKE_BINARY_DIR}/textedit/Debug/Qt5Guid.dll 
                ${CMAKE_BINARY_DIR}/textedit/Debug/Qt5Svgd.dll 
                ${CMAKE_BINARY_DIR}/textedit/Debug/Qt5Widgetsd.dll 
                CONFIGURATIONS Debug 
                DESTINATION bin 
                )
        install(DIRECTORY 
                ${CMAKE_BINARY_DIR}/textedit/Debug/iconengines 
                ${CMAKE_BINARY_DIR}/textedit/Debug/imageformats 
                ${CMAKE_BINARY_DIR}/textedit/Debug/platforms 
                ${CMAKE_BINARY_DIR}/textedit/Debug/styles 
                ${CMAKE_BINARY_DIR}/textedit/Debug/translations 
                DESTINATION bin 
                )

        install(PROGRAMS 
                ${CMAKE_BINARY_DIR}/textedit/Release/Qt5Core.dll 
                ${CMAKE_BINARY_DIR}/textedit/Release/Qt5Gui.dll 
                ${CMAKE_BINARY_DIR}/textedit/Release/Qt5Svg.dll 
                ${CMAKE_BINARY_DIR}/textedit/Release/Qt5Widgets.dll 
                CONFIGURATIONS Release 
                DESTINATION bin 
                )
        install(DIRECTORY 
                ${CMAKE_BINARY_DIR}/textedit/Release/iconengines 
                ${CMAKE_BINARY_DIR}/textedit/Release/imageformats 
                ${CMAKE_BINARY_DIR}/textedit/Release/platforms 
                ${CMAKE_BINARY_DIR}/textedit/Release/styles 
                ${CMAKE_BINARY_DIR}/textedit/Release/translations 
                DESTINATION bin 
                )
endif()
