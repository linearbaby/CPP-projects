
extern "C" {
#include "../../libs/linear_lib/Linear_search.h"
#include "../vector/vector.h"
}
#include <gtest/gtest.h>
 
TEST(LinearLibTest, Test_vec_cmp_fast) { 
    vector *vec1 = InitVector();
	vector *vec2 = InitVector();
	for (int i = 0; i < 5; i++)
	{
		PushBack(vec1, i);
		PushBack(vec2, i);
	}
	ASSERT_NEAR(0, vec_cmp_fast(vec2, vec1, 5), 0.00001);
}

 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
