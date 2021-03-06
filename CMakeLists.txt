cmake_minimum_required(VERSION 3.18)
project(CppExcel)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_NAME CppExcel)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# More warnings
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# optimizing options
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(HEADERS
        src/Alignment.h
        src/Cell.h
        src/CommandProcessor.h
        src/Input.h
        src/Range.h
        src/Row.h
        src/SortDirection.h
        src/Table.h
        src/utils.h
        src/CellContent.h
        src/BarchartDiagram.h
        src/Color.h
        src/TableContainer.h
        )

set(SOURCES
        src/Cell.cpp
        src/CommandProcessor.cpp
        src/Input.cpp
        src/main.cpp
        src/Range.cpp
        src/Row.cpp
        src/Table.cpp
        src/utils.cpp
        src/CellContent.cpp
        src/BarchartDiagram.cpp
        src/Color.cpp
        src/TableContainer.cpp
        )

add_executable(${EXECUTABLE_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PUBLIC src)
