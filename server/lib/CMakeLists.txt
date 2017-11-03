# General
project(lib)

# Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebSockets REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Network Qt5::WebSockets Qt5::Widgets)

# Input
file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories("src/")

# Target
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${LIBS})
