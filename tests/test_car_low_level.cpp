#include <iostream>
#include "gtest/gtest.h"


extern "C" {
#include "car_db_functions.h"
#include "data_loader.h"
#include "UI_functions.h"
}


class TestCarDBFixture : public testing::Test {
protected:
    void SetUp() override {
        c_1.model = (char *) malloc(4);
        strcpy(c_1.model, "bmw");
        c_1.body_shape = (char *) malloc(10);
        strcpy(c_1.body_shape, "limousine");
        c_1.enginev = 800;
        c_1.fuel_consum = 100;
        c_1.speed = 80;

        c_2.model = (char *) malloc(4);
        strcpy(c_2.model, "bNw");
        c_2.body_shape = (char *) malloc(10);
        strcpy(c_2.body_shape, "limURsine");
        c_2.enginev = 800;
        c_2.fuel_consum = 100;
        c_2.speed = 80;

        _init_db(&db);
        read_data(&db, "../tests/data/data.txt");
    }

    void TearDown() override {
        free(c_1.body_shape);
        free(c_1.model);
        free(c_2.body_shape);
        free(c_2.model);
        _clear(&db);
    }

    car c_1, c_2;
    car_db db;
};


TEST(Init, SizeEq0){
    car_db db;
    _init_db(&db);
    ASSERT_EQ(db.size, 0);
}


TEST_F(TestCarDBFixture, Clear) {
    _clear(&db);
    ASSERT_EQ(db.size, 0);
    ASSERT_EQ(db.car_items, nullptr);
}


TEST_F(TestCarDBFixture, DeleteCar) {
    car *c = &(db.car_items[db.size - 1]);
    _delete(&db, db.size-1);
    ASSERT_NE(c, &(db.car_items[db.size - 1]));
}


TEST_F(TestCarDBFixture, Append) {
    size_t old_size = db.size;
    _append(&db, c_1);
    ASSERT_NE(old_size, db.size);
    ASSERT_STRCASEEQ(db.car_items[db.size - 1].model, c_1.model);
    ASSERT_STRCASEEQ(db.car_items[db.size - 1].body_shape, c_1.body_shape);
    ASSERT_EQ(db.car_items[db.size - 1].enginev, c_1.enginev);
    ASSERT_EQ(db.car_items[db.size - 1].fuel_consum, c_1.fuel_consum);
    ASSERT_EQ(db.car_items[db.size - 1].speed, c_1.speed);
}


TEST_F(TestCarDBFixture, CalculateNormalization) {
    //проверим рассчитаны ли параметры расспределения при чтении:
    double old[2][3];
    for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 3; j++)
            old[i][j] = db.ozh_disp[i][j];

    _calclute_normalization(&db);
    for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_FLOAT_EQ(old[i][j], db.ozh_disp[i][j]);

    //проверим изменятся ли параматры после добавления элемента
    _append(&db, c_1);
    _calclute_normalization(&db);
    for (size_t i = 0; i < 2; i++)
        for (size_t j = 0; j < 3; j++)
            ASSERT_NE(old[i][j], db.ozh_disp[i][j]);
}


TEST_F(TestCarDBFixture, Add) {
    size_t old_size = db.size;
    _add(&db, c_1, 0);
    ASSERT_NE(old_size, db.size);
    ASSERT_STRCASEEQ(db.car_items[0].model, c_1.model);
    ASSERT_STRCASEEQ(db.car_items[0].body_shape, c_1.body_shape);
    ASSERT_EQ(db.car_items[0].enginev, c_1.enginev);
    ASSERT_EQ(db.car_items[0].fuel_consum, c_1.fuel_consum);
    ASSERT_EQ(db.car_items[0].speed, c_1.speed);
}



TEST_F(TestCarDBFixture, DeepCopy) {
    car c_test;
    _copy_car(&c_test, c_1);
    ASSERT_STRCASEEQ(c_test.model, c_1.model);
    ASSERT_STRCASEEQ(c_test.body_shape, c_1.body_shape);
    ASSERT_EQ(c_test.enginev, c_1.enginev);
    ASSERT_EQ(c_test.fuel_consum, c_1.fuel_consum);
    ASSERT_EQ(c_test.speed, c_1.speed);

    //проверка указывают ли структуры на одну и ту же область памяти
    ASSERT_FALSE(c_test.model == c_1.model);
    ASSERT_FALSE(c_test.body_shape == c_1.body_shape);
    free(c_test.model);
    free(c_test.body_shape);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}