# CMakeList.txt : CMake project for StarWindTaskSchedulerTask, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("StarWindTaskSchedulerTask")

# Add source to this project's executable.
add_library("TaskSchedulerLib"   "src/TaskScheduler.hpp" "src/TaskScheduler.cpp")
add_executable("TaskSchedulerTest" "test/Main.cpp")
target_link_libraries("TaskSchedulerTest" "TaskSchedulerLib")
target_include_directories("TaskSchedulerTest" PRIVATE "src/")

