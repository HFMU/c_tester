cmake_minimum_required(VERSION 2.6.0)

# here we specify that the project is C language only, so the default
# C compiler on the system will be used
project(hfmu_c_tester C)

add_executable(hfmu_c_tester hfmu_c_tester.c)
