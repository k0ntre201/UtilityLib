#include <iostream>
#include "Utility/AsyncContainers/deque.hpp"
#include "Utility/AsyncContainers/queue.hpp"
#include "Utility/AsyncContainers/list.hpp"
#include "Utility/AsyncContainers/vector.hpp"

#include <random>

using namespace UtilityLib::Threading;

int main()
{
	list<int> container;
	bool done{ false };
	std::thread pusher([&]()
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(-10000, 10000);
			for (int i{ 0 }; i < 10000; ++i)
				container.pushBack(dist(gen));
			done = true;
		});
	std::thread sorter([&]()
		{
			while (!done)
			{
				container.sort();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			container.sort();
		});
	pusher.join();
	sorter.join();

	std::cout << std::is_sorted(std::begin(container), std::end(container));

	queue<int> q;
	vector<int> v({ 1,2,3 });
	list<int> l({ 1,2,3 });

	return 0;
}