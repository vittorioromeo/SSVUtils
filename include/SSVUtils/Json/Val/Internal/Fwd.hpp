// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_FWD
#define SSVU_JSON_VAL_INTERNAL_FWD

// Forward declarations required for `Val`

namespace ssvu
{
	namespace Json
	{
		// `Val` forward declaration.
		class Val;

		namespace Internal
		{
			/// @brief Helper class to convert C++ objects to/from `Val`.
			template<typename> struct Cnv;

			/// @brief Helper class to check if `Val` contains a specific type.
			template<typename> struct Chk;

			/// @brief Helper class to check if `Val` contains a specific type, excluding numeric types.
			/// @details This is required because multiple numeric types can be stored with the same representation.
			template<typename> struct ChkNoNum;

			/// @brief Helper class to generate C++ object instances from `Val`.
			template<typename> struct AsHelper;

			/// @brief Helper class for `arch`/`extr` conversion functions.
			struct CnvFuncHelper;

			/// @brief Helper class for conversion to/from `std::tuple`.
			struct TplCnvHelper;

			/// @brief Helper class for checking types to/from `std::tuple`.
			struct TplIsHelper;
		}
	}
}

#endif
