project(QmlTest)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE _src *.h *.cpp)

add_executable(${PROJECT_NAME} ${_src} ../../lyric-tab_res.qrc)

#if (WIN32)
#    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
#endif ()

find_package(language-manager CONFIG REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets Qml Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets Qml Quick REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Quick
        lyric-tab::lyric-tab
        language-manager::language-manager
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# copy cpp-pinyin dict

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/cpp-pinyin/dict
        ${CMAKE_BINARY_DIR}/bin/dict
)

install(DIRECTORY ${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/cpp-pinyin/dict
        DESTINATION bin
)