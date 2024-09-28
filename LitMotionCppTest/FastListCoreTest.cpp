#include "pch.h"
#include <FastListCore.h>

using namespace LitMotionCpp;

TEST(FastListCoreTest, Test_Add)
{
	FastListCore<int> list;

	list.add(0);
	list.add(1);
	list.add(2);

	EXPECT_EQ(3, list.size());
	EXPECT_EQ(0, list[0]);
	EXPECT_EQ(1, list[1]);
	EXPECT_EQ(2, list[2]);
}

TEST(FastListCoreTest, Test_RemoveAtSwapback)
{
	FastListCore<int> list;

	list.add(0);
	list.add(1);
	list.add(2);
	list.add(3);

	EXPECT_EQ(4, list.size());

	list.removeAtSwapback(1);

	EXPECT_EQ(3, list.size());
	EXPECT_EQ(0, list[0]);
	EXPECT_EQ(3, list[1]);
	EXPECT_EQ(2, list[2]);
}