#include "pch.h"
#include "../UtilityLib/Utility/asyncContainer.hpp"
#include <list>
#include <thread>
#include <random>

using namespace UtilityLib::Threading;

TEST(AsyncContainerTest, PushPopTest)
{
	Deque<int, std::deque> container;
	container.pushBack(10);
	container.pushBack(20);
	container.pushFront(0);
	EXPECT_EQ(container.size(), 3);

	container.pushFront(-10);
	container.pushFront(-20);
	EXPECT_EQ(container.size(), 5);

	container.popBack();
	EXPECT_EQ(container.size(), 4);

	container.popFront();
	EXPECT_EQ(container.size(), 3);
}

TEST(AsyncContainerTest, TableOperatorTest)
{
	Deque<int, std::deque> container;
	container.pushBack(10);
	container.pushBack(20);
	container.pushFront(0);
	EXPECT_EQ(container[0], 0);
	EXPECT_EQ(container[2], 20);

	container.pushFront(-10);
	container.pushFront(-20);
	EXPECT_EQ(container[0], -20);

	container.popBack();
	EXPECT_EQ(container[3], 10);

	container.popFront();
	EXPECT_EQ(container[0], -10);
}

TEST(AsyncContainerTest, SortTest)
{
	Deque<int, std::deque> container({ 10, 20, -10, -20, 0, 5, 10 });
	std::sort(std::begin(container), std::end(container));
	EXPECT_TRUE(std::is_sorted(std::begin(container), std::end(container)));

	container.pushFront(100);
	container.pushBack(-100);
	std::sort(std::begin(container), std::end(container));
	EXPECT_TRUE(std::is_sorted(std::begin(container), std::end(container)));
}

TEST(AsyncContainerTest, MultithreadSortTest)
{
	Container<int, std::deque> container;
	bool done{ false };
	std::thread pusher([&]()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(-10000, 10000);
			for (int i{ 0 }; i < 100000; ++i)
				container.pushBack(dist(gen));
			done = true;
		});
	std::thread sorter([&]()
		{
			while (!done)
			{
				container.sort();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
			container.sort();
		});
	pusher.join();
	sorter.join();
	EXPECT_TRUE(std::is_sorted(std::begin(container), std::end(container)));
}