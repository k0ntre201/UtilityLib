#ifndef UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP
#define UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP

#include "../utility.hpp"

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <memory>
#include <deque>
#include <queue>
#include <list>

namespace UtilityLib::Threading::Container
{
	template <typename _Ty, template<typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class _AbstractInstructions
	{
	public:
		virtual ~_AbstractInstructions() = default;

	public:
		using value_type = _Ty;
		using reference = _Ty&;
		using const_reference = const _Ty&;
		//using size_type = typename _Container<value_type>::size_type;
	public:
		const_reference front() const noexcept
		{
			return this->getSmth([&]() {return this->_container.front(); });
		}

		const_reference back() const noexcept
		{
			return this->getSmth([&]() {return this->_container.back(); });
		}

		decltype(auto) front() noexcept
		{
			return this->getSmth([&]() {return this->_container.front(); });
		}

		decltype(auto) back() noexcept
		{
			return this->getSmth([&]() {return this->_container.back(); });
		}

	public:
		bool empty() noexcept
		{
			return this->getSmth([&]() { return this->_container.empty(); });
		}

		auto size() noexcept
		{
			return this->getSmth([&]() { return this->_container.size(); });
		}

	public:
		template <typename... _Vals>
		decltype(auto) emplace(_Vals&&... vals) noexcept
		{
			return this->getSmth([&]() { this->_container.emplace(vals...); });
		}

	public:
		template <typename Cont>
		decltype(auto) swap(Cont&& c)
		{
			this->doSmth([&]() {this->_container.swap(c._container); });
			return *this;
		}

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
		_Container<_Ty> _container;
	};

	template <typename _Ty, template<typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class _ContainerIterators : public _AbstractInstructions<_Ty, _Container, _Alloc>
	{
	public:
		virtual ~_ContainerIterators() = default;
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

		const iterator cbegin() const noexcept
		{
			return this->getSmth([&]() { return std::cbegin(this->_container); });
		}

		iterator end() noexcept
		{
			return this->getSmth([&]() { return std::end(this->_container); });
		}

		const iterator end() const noexcept
		{
			return this->getSmth([&]() { return std::end(this->_container); });
		}

		const iterator cend() const noexcept
		{
			return this->getSmth([&]() { return std::cend(this->_container); });
		}

		iterator rbegin() noexcept
		{
			return this->getSmth([&]() { return std::rbegin(this->_container); });
		}

		const iterator rbegin() const noexcept
		{
			return this->getSmth([&]() { return std::rbegin(this->_container); });
		}

		const iterator crbegin() const noexcept
		{
			return this->getSmth([&]() { return std::crbegin(this->_container); });
		}

		iterator rend() noexcept
		{
			return this->getSmth([&]() { return std::rend(this->_container); });
		}

		const iterator rend() const noexcept
		{
			return this->getSmth([&]() { return std::rend(this->_container); });
		}

		const iterator crend() const noexcept
		{
			return this->getSmth([&]() { return std::crend(this->_container); });
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

		Container(const _Container<_Ty>& c)
		{
			this->_container = std::move(c);
		}

		Container(const Container& cnt)
		{
			this->_container = cnt._container;
		}

		Container(Container&& cnt)
		{
			this->_container = std::move(cnt._container);
		}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		Container(_Iter first, _Iter last, const _Alloc& alloc = _Alloc())
		{
			this->_container = _Container<_Ty, _Alloc>(first, last, alloc);
		}
		Container(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc())
		{
			this->_container = _Container<_Ty, _Alloc>(list, alloc);
		}

		virtual ~Container() = default;

	public:
		template <typename... _Vals>
		decltype(auto) emplace(const_iterator _where, _Vals&&... vals) noexcept
		{
			return this->getSmth([&]() { this->_container.emplace(_where, vals...); });
		}

		template <typename... _Vals>
		decltype(auto) emplaceBack(_Vals&&... vals) noexcept
		{
			return this->getSmth([&]() { this->_container.emplace_back(vals...); });
		}

		decltype(auto) pushBack(const _Ty& val) noexcept
		{
			this->doSmth([&]() { this->_container.push_back(val); });
			return *this;
		}

		decltype(auto) popBack() noexcept
		{
			this->doSmth([&]() { this->_container.pop_back(); });
			return *this;
		}

		decltype(auto) erase(iterator pos) noexcept
		{
			this->doSmth([&]() { this->_container.erase(pos); });
			return *this;
		}

		decltype(auto) erase(const_iterator pos) noexcept
		{
			this->doSmth([&]() { this->_container.erase(pos); });
			return *this;
		}

		decltype(auto) erase(iterator from, iterator to) noexcept
		{
			this->doSmth([&]() { this->_container.erase(from, to); });
			return *this;
		}

		decltype(auto) erase(const_iterator from, const_iterator to) noexcept
		{
			this->doSmth([&]() { this->_container.erase(from, to); });
			return *this;
		}

		decltype(auto) clear() noexcept
		{
			this->doSmth([&]() { this->_container.clear(); });
			return *this;
		}

		decltype(auto) resize(size_type val) noexcept
		{
			this->doSmth([&]() { this->_container.resize(val); });
			return *this;
		}

	public:
		decltype(auto) maxSize() noexcept
		{
			this->getSmth([&]() { return this->_container.max_size(); });
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
}

#endif // !UTILITY_LIB_THREADING_ASYNC_CONTAINER_HPP

