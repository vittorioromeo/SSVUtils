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
		// Val representations
		using Idx = std::size_t;
		using Key = std::string;
		using Str = std::string;
		using Bln = bool;

		// Num representations
		using IntS = long int;
		using IntU = unsigned long int;
		using Real = double;

		namespace Internal
		{
			template<typename T> using ObjImpl = Internal::VecMap<Key, T>;
			template<typename T> using ArrImpl = std::vector<T>;
		}

		struct Nll { };



		template<bool TFmt, bool TPretty> struct WriterSettings
		{
			enum { fmt = TFmt, pretty = TPretty };
		};

		using WSPretty = WriterSettings<false, true>;
		using WSMinified = WriterSettings<false, false>;
		using WSPrettyLog = WriterSettings<true, true>;



		template<bool TNoComments> struct ReaderSettings
		{
			enum { noComments = TNoComments };
		};

		using RSDefault = ReaderSettings<false>;
		using RSNoComments = ReaderSettings<true>;
	}
}

namespace ssvj = ssvu::Json;

#endif
