# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app_time_utils_test)

target_sources(app PRIVATE
  src/main.c
  ../../../app/src/time_utils.c
)
target_include_directories(app PRIVATE ../../../app/include)
