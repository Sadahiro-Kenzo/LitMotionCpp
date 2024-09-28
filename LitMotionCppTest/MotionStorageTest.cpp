#include "pch.h"
#include <MotionStorage.h>

using namespace LitMotionCpp;

TEST(StorageEntryListTest, Test_AutoEnsureCapacity)
{
	StorageEntryList list(4);

	int entryIndex = -1;
	auto entry = list.alloc(0, &entryIndex);
	entry = list.alloc(1, &entryIndex);
	entry = list.alloc(2, &entryIndex);
	entry = list.alloc(3, &entryIndex);

	//	list is full
	entry = list[3];
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(3, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);

	//	call ensureCapacity in alloc
	entry = list.alloc(4, &entryIndex);

	EXPECT_EQ(4, entryIndex);
	entry = list[4];
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(4, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);

	entry = list[5];
	EXPECT_EQ(6, entry.Next);
	EXPECT_EQ(-1, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);

	entry = list[7];
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(-1, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
}

TEST(StorageEntryListTest, Test_Alloc)
{
	StorageEntryList list;

	int entryIndex = -1;
	auto entry = list.alloc(0, &entryIndex);
	EXPECT_EQ(0, entryIndex);
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(0, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);

	entry = list.alloc(1, &entryIndex);
	EXPECT_EQ(1, entryIndex);
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(1, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
}

TEST(StorageEntryListTest, Test_Free)
{
	StorageEntryList list;

	int entryIndex = -1;
	auto entry = list.alloc(0, &entryIndex);
	entry = list.alloc(1, &entryIndex);
	entry = list.alloc(2, &entryIndex);

	list.free(1);

	entry = list[0];
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(0, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
	entry = list[1];
	EXPECT_EQ(3, entry.Next);
	EXPECT_EQ(1, entry.DenseIndex);
	EXPECT_EQ(2, entry.Version);
	entry = list[2];
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(2, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
}

TEST(StorageEntryListTest, Test_AllocAfterFree)
{
	StorageEntryList list;

	int entryIndex = -1;
	auto entry = list.alloc(0, &entryIndex);
	entry = list.alloc(1, &entryIndex);
	entry = list.alloc(2, &entryIndex);

	list.free(1);
	entry = list.alloc(3, &entryIndex);

	EXPECT_EQ(1, entryIndex);
	entry = list[entryIndex];
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(3, entry.DenseIndex);
	EXPECT_EQ(2, entry.Version);

	entry = list.alloc(4, &entryIndex);
	EXPECT_EQ(3, entryIndex);
	entry = list[entryIndex];
	EXPECT_EQ(-1, entry.Next);
	EXPECT_EQ(4, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);

	entry = list[4];
	EXPECT_EQ(5, entry.Next);
	EXPECT_EQ(-1, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
}

TEST(StorageEntryListTest, Test_Reset)
{
	StorageEntryList list;

	int entryIndex = -1;
	auto entry = list.alloc(0, &entryIndex);
	entry = list.alloc(1, &entryIndex);
	entry = list.alloc(2, &entryIndex);

	list.reset();
	entry = list[0];
	EXPECT_EQ(1, entry.Next);
	EXPECT_EQ(-1, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
	entry = list[1];
	EXPECT_EQ(2, entry.Next);
	EXPECT_EQ(-1, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
	entry = list[2];
	EXPECT_EQ(3, entry.Next);
	EXPECT_EQ(-1, entry.DenseIndex);
	EXPECT_EQ(1, entry.Version);
}
