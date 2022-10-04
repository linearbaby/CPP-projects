#include <iostream>
#include "gtest/gtest.h"


extern "C" {
#include "cars_functions.h"
}

#define TEST_DATA_FILE "../tests/data/data.txt"
#define PROMPT "../tests/data/prompt.txt"

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
        read_data(&db, TEST_DATA_FILE);
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


TEST(ReadData, CorrectData) {
    car_db db;
    _init_db(&db);
    read_data(&db, TEST_DATA_FILE);
    ASSERT_EQ(db.size, 4);

    ASSERT_STRCASEEQ(db.car_items[0].model, "nissan");
    ASSERT_STRCASEEQ(db.car_items[0].body_shape, "van");
    ASSERT_STRCASEEQ(db.car_items[1].model, "bugatty");
    ASSERT_STRCASEEQ(db.car_items[1].body_shape, "van");
    ASSERT_STRCASEEQ(db.car_items[2].model, "pejo");
    ASSERT_STRCASEEQ(db.car_items[2].body_shape, "limousine");
    ASSERT_STRCASEEQ(db.car_items[3].model, "audi");
    ASSERT_STRCASEEQ(db.car_items[3].body_shape, "hatchback");

    _clear(&db);
}


TEST_F(TestCarDBFixture, WriteData){
    ASSERT_EQ(write_data(&db, "wrong_place"), -1);
    ASSERT_EQ(write_data(&db, TEST_DATA_FILE), 0);
}


TEST_F(TestCarDBFixture, FindNearest){
    car c = db.car_items[db.size - 1];
    ASSERT_EQ(db.size - 1, find_nearest(db, c));
    c = db.car_items[db.size - 2];
    ASSERT_EQ(db.size - 2, find_nearest(db, c));
    c = db.car_items[db.size - 3];
    ASSERT_EQ(db.size - 3, find_nearest(db, c));
}


TEST_F(TestCarDBFixture, Validate){
    ASSERT_EQ(validate_car(c_1), 1);
    ASSERT_EQ(validate_car(c_2), 0);
    ASSERT_EQ(validate_car(db.car_items[0]), 1);
}


TEST_F(TestCarDBFixture, Add){
    size_t old_size = db.size;
    ASSERT_EQ(add(&db, c_1, 0), 0);
    ASSERT_EQ(old_size + 1, db.size);

    old_size = db.size;
    ASSERT_EQ(add(&db, c_2, 0), 2);
    ASSERT_EQ(old_size, db.size);

    ASSERT_EQ(add(&db, c_1, db.size + 1), -1);
    ASSERT_EQ(old_size, db.size);
}


TEST_F(TestCarDBFixture, Append){
    size_t old_size = db.size;
    ASSERT_EQ(append(&db, c_1), 0);
    ASSERT_EQ(old_size + 1, db.size);

    old_size = db.size;
    ASSERT_EQ(append(&db, c_2), 2);
    ASSERT_EQ(old_size, db.size);
}


TEST_F(TestCarDBFixture, DeleteCar){
    size_t old_size = db.size;
    ASSERT_EQ(delete_car_from_db(&db, db.size - 1), 0);
    ASSERT_EQ(old_size - 1, db.size);

    old_size = db.size;
    ASSERT_EQ(delete_car_from_db(&db, db.size + 1), -1);
    ASSERT_EQ(old_size, db.size);
}


TEST_F(TestCarDBFixture, StringLower) {
    char Nissan[] = "Nissan";
    char nissan[] = "nissan";
    ASSERT_STRNE(Nissan, nissan);
    _to_lower_string(Nissan);
    ASSERT_STREQ(Nissan, nissan);
}


TEST_F(TestCarDBFixture, TestPrompt) {
    FILE *fp;
    if ((fp = fopen(PROMPT, "r")) == NULL) {
        printf("Cannot open file.\n");
    }
    size_t old_size = db.size;
    add_prompt(&db, fp);
    ASSERT_EQ(old_size + 1, db.size);

    ASSERT_STRCASEEQ(db.car_items[db.size - 1].model, db.car_items[db.size - 2].model);
    ASSERT_STRCASEEQ(db.car_items[db.size - 1].body_shape, db.car_items[db.size - 2].body_shape);
    ASSERT_EQ(db.car_items[db.size - 1].enginev, db.car_items[db.size - 2].enginev);
    ASSERT_EQ(db.car_items[db.size - 1].fuel_consum, db.car_items[db.size - 2].fuel_consum);
    ASSERT_EQ(db.car_items[db.size - 1].speed, db.car_items[db.size - 2].speed);

    fclose(fp);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}