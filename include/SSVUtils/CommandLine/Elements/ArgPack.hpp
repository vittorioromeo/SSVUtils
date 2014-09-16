// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_ELEMENTS_ARGPACK
#define SSVU_COMMANDLINE_ELEMENTS_ARGPACK

namespace ssvu
{
	namespace CommandLine
	{
		template<typename T> class ArgPack final : public ArgPackBase
		{
			private:
				std::vector<T> values;

			public:
				inline ArgPack() noexcept = default;
				inline ArgPack(std::size_t mMin, std::size_t mMax) noexcept : ArgPackBase{mMin, mMax} { }

				inline void set(const std::vector<std::string>& mValues) override
				{
					for(const auto& v : mValues) values.emplace_back(Parser<T>::parse(v));
				}

				inline auto begin() noexcept		{ return values.begin(); }
				inline auto end() noexcept			{ return values.end(); }
				inline auto begin() const noexcept	{ return values.begin(); }
				inline auto end() const noexcept	{ return values.end(); }
				inline auto cbegin() const noexcept	{ return values.cbegin(); }
				inline auto cend() const noexcept	{ return values.cend(); }
		};
	}
}

#endif
