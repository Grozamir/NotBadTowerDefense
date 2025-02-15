cmake_minimum_required(VERSION 3.28)
project(NotBadTowerDefense CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 

set(PLATFORM "Win" CACHE STRING "TARGET PLATFORM")

include(FetchContent)
set(CMAKE_TLS_VERIFY TRUE)
set(FETCHCONTENT_QUIET FALSE)

include_directories(${CMAKE_SOURCE_DIR}/vendor/entt)


function(add_git_dependency libName gitURL gitTag)
    FetchContent_Declare(
        ${libName}
        GIT_REPOSITORY ${gitURL}
        GIT_TAG        ${gitTag}
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(${libName})
endfunction()



set(SDLIMAGE_WEBP OFF CACHE BOOL "Enable/disable WebP support in SDL_image" FORCE)
set(SDLIMAGE_AVIF OFF CACHE BOOL "Enable/disable AVIF support in SDL_image" FORCE)
set(SDLIMAGE_BMP OFF CACHE BOOL "Enable/disable BMP support in SDL_image" FORCE)
set(SDLIMAGE_GIF OFF CACHE BOOL "Enable/disable GIF support in SDL_image" FORCE)
set(SDLIMAGE_JXL OFF CACHE BOOL "Enable/disable JXL support in SDL_image" FORCE)
set(SDLIMAGE_STB OFF CACHE BOOL "Enable/disable STB support in SDL_image" FORCE)
set(SDLIMAGE_LBM OFF CACHE BOOL "Enable/disable LBM support in SDL_image" FORCE)
set(SDLIMAGE_PCX OFF CACHE BOOL "Enable/disable PCX support in SDL_image" FORCE)
set(SDLIMAGE_PNM OFF CACHE BOOL "Enable/disable PNM support in SDL_image" FORCE)
set(SDLIMAGE_QOI OFF CACHE BOOL "Enable/disable QOI support in SDL_image" FORCE)
set(SDLIMAGE_SVG OFF CACHE BOOL "Enable/disable SVG support in SDL_image" FORCE)
set(SDLIMAGE_TGA OFF CACHE BOOL "Enable/disable TGA support in SDL_image" FORCE)
set(SDLIMAGE_TIF OFF CACHE BOOL "Enable/disable TIF support in SDL_image" FORCE)
set(SDLIMAGE_XCF OFF CACHE BOOL "Enable/disable XCF support in SDL_image" FORCE)
set(SDLIMAGE_XPM OFF CACHE BOOL "Enable/disable XPM support in SDL_image" FORCE)
set(SDLIMAGE_XV OFF CACHE BOOL "Enable/disable XV support in SDL_image" FORCE)

set(SDL_VULKAN OFF)
set(SDL_TESTS OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_EXAMPLES OFF)


set(vendored_default TRUE)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_image EXCLUDE_FROM_ALL)
add_subdirectory(vendor/SDL_ttf EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE 
"src/main.cpp"
"src/Application.cpp"
"src/scenes/SceneManager.cpp"
"src/scenes/Scene_Base.cpp"
"src/scenes/Scene_TestGame.cpp"
"src/math/Vector.h"
"src/systems/MovementSystems.cpp"
"src/systems/Rendering.cpp"
"src/helpers/SpawnHelper.cpp")



if (${PLATFORM} STREQUAL "Web")
    
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_FREETYPE=1 -s ASSERTIONS=1 -s WASM=1 -O2 -Wall -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1  -s EXPORTED_FUNCTIONS=\"['_main', '_set_pause_callback', '_set_can_show_ads_callback']\" --preload-file assets/ ")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

        add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copying assets"
    )
else()
    add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            ${CMAKE_CURRENT_BINARY_DIR}/bin/assets
        COMMENT "Copying assets"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

add_dependencies(${PROJECT_NAME} copy_assets)

set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT ${target})

