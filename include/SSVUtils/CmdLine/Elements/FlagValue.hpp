// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_FLAGVALUE
#define SSVU_CMDLINE_ELEMENTS_FLAGVALUE

namespace ssvu
{
	namespace CmdLine
	{
		namespace Internal
		{
			template<typename T> class FlagValueImpl : public BaseFlagValue
			{
				private:
					T value;

				public:
					inline FlagValueImpl(const std::string& mNameShort, const std::string& mNameLong) noexcept : BaseFlagValue{mNameShort, mNameLong} { }

					inline void set(const std::string& mValue) override	{ value = Parser<T>::parse(mValue); }
					inline auto get() const noexcept					{ return value; }

					inline std::string getUsageStr() const override { return "[" + getNameShort() + "=(...) || " + getNameLong() + "=(...)]"; }
			};
		}

		template<typename T> class FlagValue final : public Internal::FlagValueImpl<T>, public Internal::ETypeInfo<EType::FlagValue>
		{
			public:
				inline FlagValue(const std::string& mNameShort, const std::string& mNameLong) noexcept
					: Internal::FlagValueImpl<T>{mNameShort, mNameLong} { }
		};
	}
}

#endif
