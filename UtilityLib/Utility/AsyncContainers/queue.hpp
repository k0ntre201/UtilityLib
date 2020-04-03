#ifndef UTILITY_LIB_THREADING_CONTAINER_QUEUE_HPP
#define UTILITY_LIB_THREADING_CONTAINER_QUEUE_HPP

#include "container.hpp"

namespace UtilityLib::Threading::Container
{
	template <typename _Ty, template<typename...> typename _Container, typename _Alloc = std::allocator<_Ty>>
	class Queue : public _AbstractInstructions<_Ty, _Container, _Alloc>
	{
	public:
		using value_type = _Ty;
		using reference = _Ty&;
		using const_reference = const _Ty&;
		using size_type = typename _Container<value_type>::size_type;

	public:
		Queue() = default;

		Queue(const _Container<_Ty>& c)
		{
			this->_container = std::move(c);
		}

		Queue(const Queue& q)
		{
			this->_container = q._container;
		}

		Queue(Queue&& q)
		{
			this->_container = q._container;
		}

		virtual ~Queue() = default;

	public:
		decltype(auto) push(const_reference val) noexcept
		{
			this->doSmth([&]() {this->_container.push(val); });
			return *this;
		}

		decltype(auto) pop() noexcept
		{
			this->doSmth([&]() {this->_container.pop(); });
			return *this;
		}
	};
}

namespace UtilityLib::Threading
{
	template <typename T>
	using queue = Container::Queue<T, std::queue>;
}


#endif // !UTILITY_LIB_THREADING_CONTAINER_QUEUE_HPP

