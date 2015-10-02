// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_IO
#define SSVU_JSON_IO

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Json/Io/ReadException.hpp"
#include "SSVUtils/Json/Io/Reader.hpp"
#include "SSVUtils/Json/Io/Writer.hpp"

namespace ssvu
{
namespace Json
{
    namespace Impl
    {
        template <typename TRS>
        inline bool tryParse(Val& mVal, Reader<TRS>& mReader)
        {
            try
            {
                mVal = mReader.parseVal();
            }
            catch(const ReadException& mEx)
            {
                lo("JSON") << "Error occured during read\n";
                lo(mEx.getTitle()) << mEx.what() << " - at:\n" + mEx.getSrc()
                                   << std::endl;
                return false;
            }

            return true;
        }
    }
}
}

#endif
