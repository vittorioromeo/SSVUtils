// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_CLASSES
#define SSVU_UTILS_CLASSES

namespace ssvu
{
	/// @brief Class that inhibits copying.
	/// @details Use it as a base class for your types.
	/// Internally uses "= delete" on constructor and assignment.
	struct NoCopy
	{
		inline constexpr NoCopy() noexcept = default;
		NoCopy(const NoCopy&) = delete;
		NoCopy& operator=(const NoCopy&) = delete;
	};

	/// @brief Class that inhibits moving.
	/// @details Use it as a base class for your types.
	/// Internally uses "= delete" on constructor and assignment.
	struct NoMove
	{
		inline constexpr NoMove() noexcept = default;
		NoMove(NoMove&&) = delete;
		NoMove& operator=(NoMove&&) = delete;
	};
}

#endif
