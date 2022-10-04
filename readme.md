.
├── CMakeLists.txt
├── data
│   └── data.txt
├── project
│   ├── include
│   │   ├── data_path.h
│   │   └── main.h.in
│   ├── lib
│   │   ├── CMakeLists.txt
│   │   ├── coding_features
│   │   │   ├── CMakeLists.txt
│   │   │   ├── include
│   │   │   │   └── CodingFeatures.h
│   │   │   └── src
│   │   │       └── CodingFeatures.c
│   │   ├── consistent
│   │   │   ├── CMakeLists.txt
│   │   │   ├── include
│   │   │   │   ├── Errors.h
│   │   │   │   ├── FileMapper.h
│   │   │   │   └── Search.h
│   │   │   └── src
│   │   │       ├── FileMapper.c
│   │   │       └── Search.c
│   │   ├── count_symbols_interface
│   │   │   ├── CMakeLists.txt
│   │   │   └── include
│   │   │       └── CountSymbols.h
│   │   └── parallel
│   │       ├── CMakeLists.txt
│   │       ├── include
│   │       │   ├── Errors.h
│   │       │   ├── FileMapper.h
│   │       │   ├── MyMsg.h
│   │       │   └── ProcessFunctions.h
│   │       └── src
│   │           ├── FileMapper.c
│   │           └── ProcessFunctions.c
│   └── main.c
├── readme.md
├── scripts
│   └── cmake
│       └── CodeCoverage.cmake
└── tests
    ├── CMakeLists.txt
    ├── data_for_tests
    │   ├── file1.txt
    │   ├── file2.txt
    │   ├── file3.txt
    │   └── file4.txt
    ├── stress-test
    │   ├── CMakeLists.txt
    │   ├── include
    │   │   ├── data_path.h
    │   │   ├── data_path.h.in
    │   │   ├── ExeclArgsCreater.h
    │   │   └── TimeItExecl.h
    │   └── src
    │       ├── ExeclArgsCreater.c
    │       ├── stress_test.c
    │       └── TimeItExecl.c
    └── unittests
        ├── CMakeLists.txt
        └── src
            ├── test_coding.cpp
            ├── test_consistent.cpp
            └── test_parallel.cpp

24 directories, 43 files
