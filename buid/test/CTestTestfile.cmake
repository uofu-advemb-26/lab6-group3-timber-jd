# CMake generated Testfile for 
# Source directory: /Users/juanramirez/Source/5785labs/lab6/lab6-group3-timber-jd/test
# Build directory: /Users/juanramirez/Source/5785labs/lab6/lab6-group3-timber-jd/buid/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(simulate_mytest "RENODE-NOTFOUND" "--disable-xwt" "--port" "-2" "--pid-file" "renode.pid" "--console" "-e" [[$ELF=@/Users/juanramirez/Source/5785labs/lab6/lab6-group3-timber-jd/buid/test/mytest.elf; $WORKING=@/Users/juanramirez/Source/5785labs/lab6/lab6-group3-timber-jd; include @/Users/juanramirez/Source/5785labs/lab6/lab6-group3-timber-jd/test/simulate.resc; start]])
set_tests_properties(simulate_mytest PROPERTIES  _BACKTRACE_TRIPLES "/Users/juanramirez/Source/5785labs/lab6/lab6-group3-timber-jd/test/CMakeLists.txt;39;add_test;/Users/juanramirez/Source/5785labs/lab6/lab6-group3-timber-jd/test/CMakeLists.txt;0;")
