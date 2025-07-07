cmake_minimum_required(VERSION 3.26)
project(single_client_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "/opt/boost_1_82_0")
find_package(Boost REQUIRED COMPONENTS system thread)  # add more if needed

set(SOURCES
    main.cpp
    Client/client.cpp
    Server/server.cpp    
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
