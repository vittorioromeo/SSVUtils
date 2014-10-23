// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_COMMON
#define SSVU_JSON_COMMON

namespace ssvu
{
	namespace Json
	{
		/// @typedef Type of Arr indices.
		using Idx = SizeT;

		/// @typedef Type of Obj keys.
		using Key = std::string;

		/// @typedef Type of string values.
		using Str = std::string;

		/// @typedef Type of boolean values.
		using Bln = bool;

		/// @typedef Representation/storage for a signed integral numeric value.
		using IntS = long int;

		/// @typedef Representation/storage for an unsigned integral numeric value.
		using IntU = unsigned long int;

		/// @typedef Representation/storage for a real value.
		using Real = double;

		namespace Internal
		{
			/// @typedef Template for `Obj` type. Intended to be instantiated with `Val`.
			template<typename T> using ObjImpl = VecMap<Key, T>;

			/// @typedef Template for `Arr` type. Intended to be instantiated with `Val`.
			template<typename T> using ArrImpl = std::vector<T>;
		}

		/// @brief Empty struct representing a null type.
		struct Nll { };


		/// @brief Struct holding settings for `Writer`.
		/// @tparam TFmt If true, applies console color/style formatting.
		/// @tparam TPretty If true, applies indentation and newlines.
		template<bool TFmt, bool TPretty> struct WriterSettings
		{
			enum { fmt = TFmt, pretty = TPretty };
		};

		/// @typedef `Writer` settings intended for human-readable text files.
		using WSPretty = WriterSettings<false, true>;

		/// @typedef `Writer` settings intended for non human-readable text files.
		using WSMinified = WriterSettings<false, false>;

		/// @typedef `Writer` settings intended for console logging.
		using WSPrettyLog = WriterSettings<true, true>;


		/// @brief Struct holding settings for `Reader`.
		struct ReaderSettings { };

		/// @typedef `Reader` settings intended for any JSON file.
		using RSDefault = ReaderSettings;
	}
}

/// @namespace Shortcut for the `ssvu::Json` namespace.
namespace ssvj = ssvu::Json;

#endif
