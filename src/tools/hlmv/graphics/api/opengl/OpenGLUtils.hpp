#pragma once

#include <memory>

namespace graphics::api::opengl::detail
{
template<typename DST, typename SRC>
DST& AssertSubtype(SRC& src)
{
	auto dst = dynamic_cast<DST*>(&src);

#ifndef NDEBUG
	if (!dst)
	{
		throw std::invalid_argument("Type is not expected type");
	}
#endif

	return *dst;
}

template<typename DST, typename SRC>
std::shared_ptr<DST> AssertSubtype(const std::shared_ptr<SRC>& src)
{
	auto dst = std::dynamic_pointer_cast<DST>(src);

#ifndef NDEBUG
	if (!dst)
	{
		throw std::invalid_argument("Type is not expected type");
	}
#endif

	return dst;
}

void CheckGLError();
}
