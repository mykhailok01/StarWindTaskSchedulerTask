# CMakeList.txt : CMake project for StarWindTaskSchedulerTask, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("StarWindTaskSchedulerTask")

# Add source to this project's executable.
add_library(StarWindTaskSchedulerTask   "TaskScheduler.hpp" "TaskScheduler.cpp")

# TODO: Add tests and install targets if needed.
