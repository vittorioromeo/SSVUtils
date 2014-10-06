// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_IO
#define SSVU_JSON_IO

namespace ssvu
{
	namespace Json
	{
		class ReadException : public std::runtime_error
		{
			private:
				std::string title, src;

			public:
				inline ReadException(std::string mTitle, std::string mWhat, std::string mSrc) : std::runtime_error{std::move(mWhat)}, title{std::move(mTitle)}, src{std::move(mSrc)} { }

				inline const auto& getTitle() const noexcept	{ return title; }
				inline const auto& getSrc() const noexcept		{ return src; }
		};
	}
}

#include "SSVUtils/Json/Io/Reader.hpp"
#include "SSVUtils/Json/Io/Writer.hpp"

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename TRS> inline bool tryParse(Val& mVal, Reader<TRS>& mReader)
			{
				try
				{
					mVal = mReader.parseVal();
				}
				catch(const ReadException& mEx)
				{
					lo("JSON") << "Error occured during read\n";
					lo(mEx.getTitle()) << mEx.what() << " - at:\n" + mEx.getSrc() << std::endl;
					return false;
				}

				return true;
			}
		}
	}
}

#endif
