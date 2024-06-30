# cmake_minimum_required(VERSION 3.10)

# project(DashboardCenter C)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 11)

# find_package(SDL2 REQUIRED SDL2)
# include_directories(
#     ${SDL2_INCLUDE_DIRS}
#     ${SDL2_INCLUDE_DIRS}/../
#     ${PROJECT_SOURCE_DIR}
#     ${PROJECT_SOURCE_DIR}/lvgl
#     ${PROJECT_SOURCE_DIR}/lv_drivers
#     ${PROJECT_SOURCE_DIR}/ui
#     ${PROJECT_SOURCE_DIR}/myapi  # Thêm thư mục random vào danh sách include
# )

# FILE(GLOB_RECURSE LVGL_Sources CONFIGURE_DEPENDS lvgl/*.c)
# FILE(GLOB_RECURSE LV_DRIVERS_Sources CONFIGURE_DEPENDS lv_drivers/*.c)
# FILE(GLOB_RECURSE UI_Sources CONFIGURE_DEPENDS ui/*.c)

# # Thêm random.c vào danh sách các file nguồn
# set(RANDOM_Sources myapi/myapi.c)

# add_executable(${PROJECT_NAME} main.c ${LVGL_Sources} ${LV_DRIVERS_Sources} ${UI_Sources})

# string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

# target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

# find_package(CURL REQUIRED)
# find_package(cJSON REQUIRED)

# target_link_libraries(DashboardCenter PRIVATE CURL::libcurl cjson)


# cmake_minimum_required(VERSION 3.10)

# project(DashboardCenter C)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 11)

# # Tìm kiếm SDL2
# find_package(SDL2 REQUIRED SDL2)
# include_directories(
#     ${SDL2_INCLUDE_DIRS}
#     ${SDL2_INCLUDE_DIRS}/../
#     ${PROJECT_SOURCE_DIR}
#     ${PROJECT_SOURCE_DIR}/lvgl
#     ${PROJECT_SOURCE_DIR}/lv_drivers
#     ${PROJECT_SOURCE_DIR}/ui
#     ${PROJECT_SOURCE_DIR}/myapi  
# )

# # Tìm kiếm các file nguồn
# FILE(GLOB_RECURSE LVGL_Sources CONFIGURE_DEPENDS lvgl/*.c)
# FILE(GLOB_RECURSE LV_DRIVERS_Sources CONFIGURE_DEPENDS lv_drivers/*.c)
# FILE(GLOB_RECURSE UI_Sources CONFIGURE_DEPENDS ui/*.c)

# # Thêm myapi.c vào danh sách các file nguồn
# set(RANDOM_Sources myapi/myapi.c myapi/weatherapi.c myapi/showmyapi.c myapi/control_button.c myapi/refresh_token.c) 
# #set(RANDOM_Sources myapi/weatherapi.c)


# add_executable(${PROJECT_NAME} main.c ${LVGL_Sources} ${LV_DRIVERS_Sources} ${UI_Sources} ${RANDOM_Sources})

# string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

# target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

# # Tìm kiếm thư viện CURL
# find_package(CURL REQUIRED)

# # Tìm kiếm thư viện cJSON
# find_path(CJSON_INCLUDE_DIR cJSON.h PATHS /usr/include/cjson)
# find_library(CJSON_LIBRARY NAMES cjson PATHS /usr/lib/x86_64-linux-gnu)

# if (CJSON_INCLUDE_DIR AND CJSON_LIBRARY)
#     message(STATUS "Found cJSON: ${CJSON_LIBRARY}")
#     include_directories(${CJSON_INCLUDE_DIR})
#     target_link_libraries(${PROJECT_NAME} PRIVATE ${CJSON_LIBRARY})
# else()
#     message(FATAL_ERROR "Could not find cJSON library")
# endif()

# # Liên kết thư viện CURL và cJSON
# target_link_libraries(DashboardCenter PRIVATE CURL::libcurl)

# # Nếu bạn muốn in đường dẫn để kiểm tra
# message(STATUS "CURL include dir: ${CURL_INCLUDE_DIRS}")
# message(STATUS "CURL libraries: ${CURL_LIBRARIES}")
# message(STATUS "cJSON include dir: ${CJSON_INCLUDE_DIR}")
# message(STATUS "cJSON library: ${CJSON_LIBRARY}")


cmake_minimum_required(VERSION 3.10)

project(SquareLine_Project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Tìm kiếm SDL2
find_package(SDL2 REQUIRED SDL2)
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}/../
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/lvgl
    ${PROJECT_SOURCE_DIR}/lv_drivers
    ${PROJECT_SOURCE_DIR}/ui
    ${PROJECT_SOURCE_DIR}/myapi  
)

# Tìm kiếm các file nguồn
FILE(GLOB_RECURSE LVGL_Sources CONFIGURE_DEPENDS lvgl/*.c)
FILE(GLOB_RECURSE LV_DRIVERS_Sources CONFIGURE_DEPENDS lv_drivers/*.c)
FILE(GLOB_RECURSE UI_Sources CONFIGURE_DEPENDS ui/*.c)

# Thêm myapi.c vào danh sách các file nguồn
set(RANDOM_Sources myapi/myapi.c myapi/weatherapi.c myapi/showmyapi.c myapi/control_button.c myapi/refresh_token.c) 

add_executable(${PROJECT_NAME} main.c ${LVGL_Sources} ${LV_DRIVERS_Sources} ${UI_Sources} ${RANDOM_Sources})

string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

# Tìm kiếm thư viện CURL
find_package(CURL REQUIRED)

# Tìm kiếm thư viện cJSON
find_path(CJSON_INCLUDE_DIR cJSON.h PATHS /usr/include/cjson)
find_library(CJSON_LIBRARY NAMES cjson PATHS /usr/lib/x86_64-linux-gnu)

if (CJSON_INCLUDE_DIR AND CJSON_LIBRARY)
    message(STATUS "Found cJSON: ${CJSON_LIBRARY}")
    include_directories(${CJSON_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CJSON_LIBRARY})
else()
    message(FATAL_ERROR "Could not find cJSON library")
endif()

# Liên kết thư viện CURL và cJSON
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)

# Tìm kiếm thư viện JWT
find_path(JWT_INCLUDE_DIR NAMES jwt.h PATHS /usr/include /usr/local/include)
find_library(JWT_LIBRARY NAMES jwt PATHS /usr/lib /usr/local/lib)

if(JWT_INCLUDE_DIR AND JWT_LIBRARY)
    message(STATUS "Found JWT: include at ${JWT_INCLUDE_DIR}, library at ${JWT_LIBRARY}")
    include_directories(${JWT_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${JWT_LIBRARY})
else()
    message(FATAL_ERROR "Could not find JWT library")
endif()

# Nếu bạn muốn in đường dẫn để kiểm tra
message(STATUS "SDL2 include dir: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries: ${SDL2_LIBRARIES}")
message(STATUS "CURL include dir: ${CURL_INCLUDE_DIRS}")
message(STATUS "CURL libraries: ${CURL_LIBRARIES}")
message(STATUS "cJSON include dir: ${CJSON_INCLUDE_DIR}")
message(STATUS "cJSON library: ${CJSON_LIBRARY}")
message(STATUS "JWT include dir: ${JWT_INCLUDE_DIR}")
message(STATUS "JWT library: ${JWT_LIBRARY}")