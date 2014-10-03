// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_COMMON
#define SSVU_JSON_COMMON

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



		// TODO: can be better?
		enum class WMode{Pretty, Minified, PrettyConsole};

		template<WMode TWM> struct WriterSettings;
		template<> struct WriterSettings<WMode::Pretty>			{ enum { fmt = false,	pretty = true }; };
		template<> struct WriterSettings<WMode::Minified>		{ enum { fmt = false,	pretty = false }; };
		template<> struct WriterSettings<WMode::PrettyConsole>	{ enum { fmt = true,	pretty = true }; };



		enum class RMode{Default, NoComments};

		template<RMode TRM> struct ReaderSettings;
		template<> struct ReaderSettings<RMode::Default>		{ enum { noComments = false }; };
		template<> struct ReaderSettings<RMode::NoComments>		{ enum { noComments = true }; };
	}
}

namespace ssvj = ssvu::Json;

#endif
