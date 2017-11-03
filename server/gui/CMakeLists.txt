# General
project(gui)

# Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebSockets REQUIRED)
find_package(Qt5Widgets REQUIRED)
set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Network Qt5::WebSockets Qt5::Widgets)

# Input
file(GLOB_RECURSE SOURCES
	"src/*.cpp"
	"src/*.ui"
	"resources/*.rc"
	"resources/*.qrc")
include_directories("src/" "../lib/src/")

# Target
if (WIN32)
	add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${FORMS} ${UTILS_SOURCES})
else()
	add_executable(${PROJECT_NAME} ${SOURCES} ${FORMS} ${UTILS_SOURCES})
endif()
target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${LIBS} lib)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "VN Streamer")
