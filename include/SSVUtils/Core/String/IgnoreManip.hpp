// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_IGNOREMANIP
#define SSVU_CORE_IGNOREMANIP

namespace ssvu
{
	/// @brief `std::string` wrapper that ignores stream manipulators when sent to a stream.
	class IgnoreManip
	{
		private:
			std::string str;

		public:
			inline IgnoreManip() = default;
			inline IgnoreManip(const std::string& mStr) : str{mStr} { }
			inline IgnoreManip(std::string&& mStr) noexcept : str{std::move(mStr)} { }

			template<typename T> inline auto& operator=(T&& mStr) { str = fwd<T>(mStr); return *this; }

			inline operator const std::string&() const noexcept { return str; }

			inline auto begin() const noexcept	{ return std::begin(str); }
			inline auto end() const noexcept	{ return std::end(str); }
	};

	/// @brief Interaction between an `std::ostream` object and a `IgnoreManip` object.
	inline auto& operator<<(std::ostream& mStream, const IgnoreManip& mIBM)
	{
		for(auto c : mIBM) mStream.put(c);
		return mStream;
	}
}

#endif

