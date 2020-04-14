cmake_minimum_required(VERSION 3.5)

project(prak LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES prak_en_US.ts)

qt5_add_resources(RESOURCE_FILES resources.qrc)

if(ANDROID)
  add_library(prak SHARED
    main.cpp
    gas_station.cpp
    gas_station.h
    gas_station.ui
    ${TS_FILES}
  )
else()
  add_executable(prak
    main.cpp
    gas_station.cpp
    gas_station.h
    common.cpp
    common.h
    options.h
    request.h
    request_generator.h
    state.h
    statistics.h
    gas_station.ui
    ${TS_FILES}
    ${RESOURCE_FILES}
  )
endif()

target_link_libraries(prak PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
