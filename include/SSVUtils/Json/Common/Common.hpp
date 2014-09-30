// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_COMMON
#define SSVU_JSON_COMMON

#define SSVU_JSON_ENABLE_IF_IS(mT, mType)		EnableIf<isSame<RemoveAll<mT>, mType>()>* = nullptr
#define SSVU_JSON_ENABLE_IF_IS_NOT(mT, mType)	EnableIf<!isSame<RemoveAll<mT>, mType>()>* = nullptr

#define SSVU_JSON_UNREACHABLE()					SSVU_ASSERT(false); std::terminate()

#include "SSVUtils/Json/Common/VecMap.hpp"

namespace ssvu
{
	namespace Json
	{
		using Idx = std::size_t;
		using Key = std::string;
		using Str = std::string;
		using Bln = bool;

		namespace Internal
		{
			template<typename T> using ObjImpl = Internal::VecMap<Key, T>;
			template<typename T> using ArrImpl = std::vector<T>;
		}

		struct Nll { };

		enum class WriterMode{Pretty, Minified};
	}
}

namespace ssvj = ssvu::Json;

#endif
