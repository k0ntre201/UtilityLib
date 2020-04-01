#ifndef UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP
#define UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP

#include "utility.hpp"

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <memory>
#include <deque>
#include <queue>

namespace UtilityLib::Threading
{
	template <typename _Ty, template<typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class _AbstractInstructions
	{
	protected:

		template <typename F, typename... Args>
		void doSmth(F&& f, Args&&... args)
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			is_busy = true;
			f(args...);
			is_busy = false;
			cv.notify_one();
		}

		template <typename F, typename... Args>
		decltype(auto) getSmth(F&& f, Args&&... args)
		{
			std::unique_lock<std::mutex> lock(m);
			while (is_busy)
				cv.wait(lock);
			is_busy = true;
			auto smth = f(args...);
			is_busy = false;
			cv.notify_one();
			return smth;
		}

	protected:
		std::mutex m;
		std::condition_variable cv;
		bool is_busy{ false };

	protected:
		_Container<_Ty, _Alloc> _container;
	};

	template <typename _Ty, template<typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class _ContainerIterators : public _AbstractInstructions<_Ty, _Container, _Alloc>
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
		iterator begin() noexcept
		{
			return this->getSmth([&]() { return std::begin(this->_container); });
		}

		const iterator begin() const noexcept
		{
			return this->getSmth([&]() { return std::begin(this->_container); });
		}

		iterator end() noexcept
		{
			return this->getSmth([&]() { return std::end(this->_container); });
		}

		const iterator end() const noexcept
		{
			return this->getSmth([&]() { return std::end(this->_container); });
		}
	};

	template <typename _Ty, template<typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class Queue : public _AbstractInstructions<_Ty, _Container, _Alloc>
	{
	public:
		using value_type = _Ty;
		using const_pointer = typename _Container<value_type>::const_pointer;
		using reference = _Ty&;
		using const_reference = const _Ty&;
		using size_type = typename _Container<value_type>::size_type;
		
	public:
		Queue() {}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		Queue(_Iter first, _Iter last, const _Alloc& alloc = _Alloc())
		{
			this->_container = _Container<_Ty, _Alloc>(first, last, alloc);
		}

		Queue(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc())
		{
			this->_container = _Container<_Ty, _Alloc>(list, alloc);
		}

		const_reference front() noexcept
		{
			return this->getSmth([&]() {return this->_container.front(); });
		}

		const_reference back() noexcept
		{
			return this->getSmth([&]() {return this->_container.back(); });
		}

		void push(const_reference val) noexcept
		{
			this->doSmth([&]() {this->container.push(val); });
		}

		void pop() noexcept
		{
			this->doSmth([&]() {this->container.pop(); });
		}

		template <typename... _Vals>
		decltype(auto) emplace(_Vals&&... vals) noexcept
		{
			this->doSmth([&]() { this->_container.emplace(vals...); });
		}
	};

	template <typename _Ty, template<typename, typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class Container : public _ContainerIterators<_Ty, _Container, _Alloc>
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
			this->_container = _Container<_Ty, _Alloc>(first, last, alloc);
		}
		Container(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc())
		{
			this->_container = _Container<_Ty, _Alloc>(list, alloc);
		}

	public:
		template <typename... _Vals>
		decltype(auto) emplace(const_iterator _where, _Vals&&... vals) noexcept
		{
			this->doSmth([&]() { this->_container.emplace(_where, vals...); });
		}

		template <typename... _Vals>
		decltype(auto) emplaceBack(_Vals&&... vals) noexcept
		{
			this->doSmth([&]() { this->_container.emplace_back(vals...); });
		}

		void pushBack(const _Ty& val) noexcept
		{
			this->doSmth([&]() { this->_container.push_back(val); });
		}

		void popBack() noexcept
		{
			this->doSmth([&]() { this->_container.pop_back(); });
		}

		void erase() noexcept
		{
			this->doSmth([&]() { this->_container.erase(); });
		}

		void resize(size_type val) noexcept
		{
			this->doSmth([&]() { this->_container.resize(val); });
		}

	public:
		bool empty() noexcept
		{
			return this->getSmth([&]() { return this->_container.empty(); });
		}

		size_type size() noexcept
		{
			return this->getSmth([&]() { return this->_container.size(); });
		}

	public:

		template <typename Algorithm>
		decltype(auto) algorithm(Algorithm&& alg)
		{
			this->doSmth([&]() {alg(std::begin(this->_container), std::end(this->_container)); });
			return *this;
		}

		template <typename Algorithm, typename UnaryPredicate >
		decltype(auto) algorithm(Algorithm&& alg, UnaryPredicate&& pred)
		{
			this->doSmth([&]() {alg(std::begin(this->_container), std::end(this->_container), pred); });
			return *this;
		}

		decltype(auto) sort() noexcept
		{
			return algorithm(std::sort<iterator>);
		}

		template <typename UnaryPredicate >
		decltype(auto) sort(UnaryPredicate& pred) noexcept
		{
			return algorithm(std::sort<iterator>, pred);
		}
	};

	template <typename _Ty, template<typename, typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class Deque : public Container<_Ty, _Container, _Alloc>
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
		Deque():Container<_Ty, _Container, _Alloc>(){}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		Deque(_Iter first, _Iter last, const _Alloc& alloc = _Alloc()) : Container<_Ty, _Container, _Alloc>(first, last, alloc)
		{
		}

		Deque(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc()) :Container<_Ty, _Container, _Alloc>(list, alloc)
		{
		}
		void pushFront(const _Ty& val) noexcept
		{
			this->doSmth([&]() { this->_container.push_front(val); });
		}

		void popFront() noexcept
		{
			this->doSmth([&]() { this->_container.pop_front(); });
		}

	public:
		decltype(auto) operator[](const size_type pos) noexcept
		{
			return this->getSmth([&]() {return std::move(this->_container[pos]); });
		}

		const _Ty& operator[](const size_type pos) const noexcept
		{
			return this->getSmth([&]() {return std::move(this->_container[pos]); });
		}
		
	};

	template <typename T, typename Alloc = std::allocator<T>>
	using deque = Deque<T, std::deque, Alloc>;

	template <typename T>
	using queue = Queue<T, std::queue>;
}

#endif // !UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP

