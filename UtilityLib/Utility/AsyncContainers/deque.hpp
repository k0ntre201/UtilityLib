#ifndef UTILITY_LIB_THREADING_CONTAINER_DEQUE_HPP
#define UTILITY_LIB_THREADING_CONTAINER_DEQUE_HPP

#include "container.hpp"

namespace UtilityLib::Threading::Container
{
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
		Deque() :Container<_Ty, _Container, _Alloc>() {}

		Deque(const _Container<_Ty>& c) :Container<_Ty, _Container, _Alloc>(c) {}

		Deque(const Deque& cnt) :Container<_Ty, _Container, _Alloc>(cnt) {}

		Deque(Deque&& cnt) :Container<_Ty, _Container, _Alloc>(cnt) {}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		Deque(_Iter first, _Iter last, const _Alloc& alloc = _Alloc()) : Container<_Ty, _Container, _Alloc>(first, last, alloc) {}

		Deque(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc()) :Container<_Ty, _Container, _Alloc>(list, alloc) {}

		virtual ~Deque() = default;

	public:
		template <typename... Vals>
		decltype(auto) emplaceFront(Vals&&... vals) noexcept
		{
			return this->getSmth([&]() { this->_container.emplace_front(vals...); });
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
		decltype(auto) at(const size_type pos) noexcept
		{
			return this->getSmth([&]() {return std::move(this->_container.at(pos)); });
		}

		decltype(auto) operator[](const size_type pos) noexcept
		{
			return this->getSmth([&]() {return std::move(this->_container[pos]); });
		}

		const _Ty& operator[](const size_type pos) const noexcept
		{
			return this->getSmth([&]() {return std::move(this->_container[pos]); });
		}
	};
}

namespace UtilityLib::Threading
{
	template <typename T, typename Alloc = std::allocator<T>>
	using deque = Container::Deque<T, std::deque, Alloc>;
}

#endif // !UTILITY_LIB_THREADING_CONTAINER_QUEUE_HPP

