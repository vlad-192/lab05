# CMake generated Testfile for 
# Source directory: /home/vboxuser/vlad-192/workspace/projects/lab05
# Build directory: /home/vboxuser/vlad-192/workspace/projects/lab05/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(banking_tests "/home/vboxuser/vlad-192/workspace/projects/lab05/build/banking_tests")
set_tests_properties(banking_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/vboxuser/vlad-192/workspace/projects/lab05/CMakeLists.txt;35;add_test;/home/vboxuser/vlad-192/workspace/projects/lab05/CMakeLists.txt;0;")
subdirs("third-party/gtest")
