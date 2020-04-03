#ifndef UTILITY_LIB_THREADING_CONTAINER_LIST_HPP
#define UTILITY_LIB_THREADING_CONTAINER_LIST_HPP

#include "container.hpp"

namespace UtilityLib::Threading::Container
{
	template <typename _Ty, template<typename, typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class List : public Container<_Ty, _Container, _Alloc>
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
		List() :Container<_Ty, _Container, _Alloc>() {}

		List(const _Container<_Ty>& c) :Container<_Ty, _Container, _Alloc>(c) {}

		List(const List& cnt) :Container<_Ty, _Container, _Alloc>(cnt) {}

		List(List&& cnt) :Container<_Ty, _Container, _Alloc>(cnt) {}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		List(_Iter first, _Iter last, const _Alloc& alloc = _Alloc()) : Container<_Ty, _Container, _Alloc>(first, last, alloc) {}

		List(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc()) :Container<_Ty, _Container, _Alloc>(list, alloc) {}

		virtual ~List() = default;

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
		decltype(auto) merge(List& other) noexcept
		{
			this->doSmth([&]() { this->_container.merge(other._container); });
			return *this;
		}

		decltype(auto) merge(List&& other) noexcept
		{
			this->doSmth([&]() { this->_container.merge(other._container); });
			return *this;
		}

		template <typename Comparator>
		decltype(auto) merge(List& other, Comparator&& comp) noexcept
		{
			this->doSmth([&]() { this->_container.merge(other._container, comp); });
			return *this;
		}

		template <typename Comparator>
		decltype(auto) merge(List&& other, Comparator&& comp) noexcept
		{
			this->doSmth([&]() { this->_container.merge(other._container, comp); });
			return *this;
		}

		decltype(auto) splice(const_iterator pos, List& other)
		{
			this->doSmth([&]() { this->_container.splice(pos, other._container); });
			return *this;
		}

		decltype(auto) splice(const_iterator pos, List&& other)
		{
			this->doSmth([&]() { this->_container.splice(pos, other._container); });
			return *this;
		}

		decltype(auto) splice(const_iterator pos, List& other, const_iterator it)
		{
			this->doSmth([&]() { this->_container.splice(pos, other._container, it); });
			return *this;
		}

		decltype(auto) splice(const_iterator pos, List&& other, const_iterator it)
		{
			this->doSmth([&]() { this->_container.splice(pos, other._container, it); });
			return *this;
		}

		decltype(auto) splice(const_iterator pos, List& other, const_iterator first, const_iterator last)
		{
			this->doSmth([&]() { this->_container.splice(pos, other._container, first, last); });
			return *this;
		}

		decltype(auto) splice(const_iterator pos, List&& other, const_iterator first, const_iterator last)
		{
			this->doSmth([&]() { this->_container.splice(pos, other._container, first, last); });
			return *this;
		}

		decltype(auto) reverse() noexcept
		{
			this->doSmth([&]() { this->_container.reverse(); });
			return *this;
		}

		decltype(auto) unique() noexcept
		{
			this->doSmth([&]() { this->_container.unique(); });
			return *this;
		}

		template <typename BinaryPredicate>
		decltype(auto) unique(BinaryPredicate p)
		{
			this->doSmth([&]() { this->_container.unique(p); });
			return *this;
		}

		decltype(auto) sort() noexcept
		{
			this->doSmth([&]() { this->_container.sort(); });
			return *this;
		}

		template <typename Comparator>
		decltype(auto) sort(Comparator p)
		{
			this->doSmth([&]() { this->_container.sort(p); });
			return *this;
		}
	};
}

namespace UtilityLib::Threading
{
	template <typename T, typename Alloc = std::allocator<T>>
	using list = Container::List<T, std::list, Alloc>;
}

#endif // !UTILITY_LIB_THREADING_CONTAINER_LIST_HPP
