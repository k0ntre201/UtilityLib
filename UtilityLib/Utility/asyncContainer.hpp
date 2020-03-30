#ifndef UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP
#define UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP

#include "utility.hpp"

#include <atomic>
#include <mutex>
#include <vector>
#include <memory>
#include <deque>
#include <queue>

namespace UtilityLib::Threading
{
	template <typename _Ty, template<typename, typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class Container
	{
	public:
		using value_type = _Ty;
		using allocator_type = _Alloc;
		using pointer = typename _Container<value_type, allocator_type>::pointer;
		using const_pointer = typename _Container<value_type, allocator_type>::const_pointer;
		using reference = _Ty&;
		using const_reference = const _Ty&;
		using size_type = typename _Container<value_type, allocator_type>::size_type;
		using difference_type = typename _Container<value_type, allocator_type>::difference_type;

	public:
		using iterator = typename _Container<value_type, allocator_type>::iterator;
		using const_iterator = typename _Container<value_type, allocator_type>::const_iterator;
		using reverse_iterator = typename _Container<value_type, allocator_type>::reverse_iterator;
		using const_reverse_iterator = typename _Container<value_type, allocator_type>::const_reverse_iterator;

	public:
		Container() {}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		Container(_Iter first, _Iter last, const _Alloc& alloc = _Alloc())
		{
			_container = _Container<_Ty, _Alloc>(first, last, alloc);
		}
		Container(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc())
		{
			_container = _Container<_Ty, _Alloc>(list, alloc);
		}

	public:

		template <typename... _Vals>
		decltype(auto) emplace(const_iterator _where, _Vals&&... vals) noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			is_busy = true;
			_container.emplace(_where, vals...);
			is_busy = false;
			cv.notify_all();
		}

		template <typename... _Vals>
		decltype(auto) emplaceBack(_Vals&&... vals) noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			is_busy = true;
			_container.emplace_back(val...);
			is_busy = false;
			cv.notify_all();
		}

		void pushBack(const _Ty& val) noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			is_busy = true;
			_container.push_back(val);
			is_busy = false;
			cv.notify_all();
		}

		void popBack() noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			_container.pop_back();
		}

		void erase() noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (!is_busy)
				cv.wait(lock);
			_container.erase();
		}

		void resize(size_type val) noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			is_busy = true;
			_container.resize(val);
			is_busy = false;
			cv.notify_all();
		}

	public:
		bool empty() noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return _container.empty();
		}
		size_type size() noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return _container.size();
		}

	public:
		iterator begin() noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return std::begin(_container);
		}

		const iterator begin() const noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return std::begin(_container);
		}

		iterator end() noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return std::end(_container);
		}

		const iterator end() const noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return std::end(_container);
		}

	protected:
		_Container<_Ty, _Alloc> _container;

	protected:
		std::mutex m;
		std::condition_variable cv;
		bool is_busy{ false };
	};

	template <typename _Ty, template<typename, typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class Deque : public Container<_Ty, _Container, _Alloc>
	{
	public:
		Deque() {}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		Deque(_Iter first, _Iter last, const _Alloc& alloc = _Alloc()) : Container<_Ty, _Container, _Alloc>(first, last, alloc)
		{
		}

		Deque(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc()) :Container<_Ty, _Container, _Alloc>(list, alloc)
		{
		}
		void pushFront(const _Ty& val) noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			is_busy = true;
			_container.push_front(val);
			is_busy = false;
			cv.notify_all();
		}

		void popFront() noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			_container.pop_front();
		}

	public:
		_Ty& operator[](const size_type pos) noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return _container[pos];
		}

		_Ty& operator[](const size_type pos) const noexcept
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			return _container[pos];
		}
	};

	template <template<typename, typename...> typename _Container, typename _Ty, typename _Alloc = std::allocator<_Ty>>
	class Queue
	{
	};
}

#endif // !UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP

