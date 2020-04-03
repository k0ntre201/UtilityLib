#ifndef UTILITY_LIB_THREADING_CONTAINER_VECTOR_HPP
#define UTILITY_LIB_THREADING_CONTAINER_VECTOR_HPP

#include "container.hpp"

namespace UtilityLib::Threading::Container
{

	template <typename _Ty, template<typename, typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class Vector : public Container<_Ty, _Container, _Alloc>
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
		Vector() :Container<_Ty, _Container, _Alloc>() {}

		Vector(const _Container<_Ty>& c) :Container<_Ty, _Container, _Alloc>(c) {}

		Vector(const Vector& cnt) :Container<_Ty, _Container, _Alloc>(cnt) {}

		Vector(Vector&& cnt) :Container<_Ty, _Container, _Alloc>(cnt) {}

		template <typename _Iter, typename = typename std::enable_if<is_iterator_v<_Iter>>>
		Vector(_Iter first, _Iter last, const _Alloc& alloc = _Alloc()) : Container<_Ty, _Container, _Alloc>(first, last, alloc) {}

		Vector(std::initializer_list<_Ty> list, const _Alloc& alloc = _Alloc()) :Container<_Ty, _Container, _Alloc>(list, alloc) {}

		virtual ~Vector() = default;

	public:
		pointer data() noexcept
		{
			return this->getSmth([&]() {return this->_container.data(); });
		}

		const_pointer data() const noexcept
		{
			return this->getSmth([&]() {return this->_container.data(); });
		}

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

	public:
		decltype(auto) reserve(const size_type newCapacity)
		{
			this->doSmth([&]() { this->_container.reserve(newCapacity); });
			return *this;
		}

		size_type capacity() const noexcept
		{
			return this->getSmth([&]() {return std::move(this->_container.capacity()); });
		}

		decltype(auto) shrinkToFit()
		{
			this->doSmth([&]() { this->_container.shrink_to_fit(); });
			return *this;
		}
	};
}

namespace UtilityLib::Threading
{
	template <typename T, typename Alloc = std::allocator<T>>
	using vector = Container::Vector<T, std::vector, Alloc>;
}


#endif // !UTILITY_LIB_THREADING_CONTAINER_VECTOR_HPP
