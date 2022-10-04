[![Build Status](https://app.travis-ci.com/ArtemPushPop/Technopark-C-C.svg?branch=IZ1)](https://app.travis-ci.com/ArtemPushPop/Technopark-C-C)<br>
[![codecov](https://codecov.io/gh/ArtemPushPop/Technopark-C-C/branch/IZ1/graph/badge.svg?token=MGS6L0WL3P)](https://codecov.io/gh/ArtemPushPop/Technopark-C-C)
<h3>Вариант #4</h3>
Создать структуру для хранения характеристик автомобилей: мощности двигателя, скорости, расхода топлива, формы кузова и модели. Составить с ее использованием программу поиска модели автомобиля, характеристики которой наиболее удовлетворяют запросу.


```
.
├── CMakeLists.txt
├── data
│   └── data.txt
├── project
│   ├── include
│   │   ├── main.h
│   │   └── main.h.in
│   ├── main.c
│   └── src
│       ├── cars
│       │   ├── CMakeLists.txt
│       │   ├── data_loader.c
│       │   ├── data_saver.c
│       │   ├── include
│       │   │   ├── cars_functions.h
│       │   │   ├── data_loader.h
│       │   │   ├── data_saver.h
│       │   │   ├── metric.h
│       │   │   ├── models_backs.h
│       │   │   └── UI_functions.h
│       │   ├── metric.c
│       │   └── UI_functions.c
│       ├── cars_low_level
│       │   ├── car_db_functions.c
│       │   ├── CMakeLists.txt
│       │   └── include
│       │       └── car_db_functions.h
│       └── CMakeLists.txt
├── readme.md
├── scripts_cmake
│   └── CodeCoverage.cmake
└── tests
    ├── data
    │   ├── data.txt
    │   └── prompt.txt
    ├── test_car_low_level.cpp
    └── test_cars_lib.cpp

11 directories, 26 files

```
