//
// Created by artem on 15.10.2020.
//

extern "C" {
#include "ship_header.h"
#include "mylist.h"
}

#include <gtest/gtest.h>
#define testingFILE "test_examp.txt"

const char * ex_for_test = NULL;

TEST(find, positive) {

    FILE *fp;
    if ((fp = fopen(ex_for_test, "r")) == NULL) {
        std::cout << "WRONG FILE" << std::endl;
        exit(1);
    }

    node *list;
    list = load(fp);

    char patt1[5] = "2019";
    char patt2[5] = "2021";

    EXPECT_EQ(1, find(3, patt1, list->data));
    ASSERT_NE(1, find(3, patt2, list->data));

    fclose(fp);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    if (argc < 2) {
        ex_for_test = testingFILE;
    } else
        ex_for_test = argv[1];

    return RUN_ALL_TESTS();
}