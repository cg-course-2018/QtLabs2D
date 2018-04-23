#pragma once

#include <functional>
#include <stdexcept>

namespace platform
{

namespace detail
{
void DisplayException(const std::exception &ex);
}

template<class Callable, typename... Args>
decltype(auto) CatchAndDisplay(Callable &&fn, Args &&... args) noexcept
{
	using Result = decltype(fn(std::forward<Args>(args)...));
	try
	{
		return std::invoke(std::forward<Callable>(fn), std::forward<Args>(args)...);
	}
	catch (const std::exception &ex)
	{
		detail::DisplayException(ex);
	}
	return Result();
}

} // namespace platform
