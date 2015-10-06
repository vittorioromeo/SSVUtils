// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_ASSERT_INL
#define SSVU_CORE_ASSERT_INL

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "SSVUtils/Core/Assert/Assert.hpp"
#include "SSVUtils/Core/Common/Common.hpp"
#include "SSVUtils/Core/Log/Log.hpp"

// This file implements `assertImpl`, which requires additional dependencies.

#if !(SSVU_IMPL_ASSERT_DISABLED)

namespace ssvu
{
    namespace Impl
    {
        inline void assertImpl(AssertData&& mAD, bool mExpression,
            const std::string& mMsg) noexcept
        {
            if(SSVU_LIKELY(mExpression)) return;

            lo() << hr() << "\n";
            lo("ASSERTION FAILED") << "\n" << mMsg << "\n\n"
                                   << "Line " << mAD.line << " in file "
                                   << mAD.file << "\n"
                                   << "Code: " << mAD.code << "\n\n";

            if(!mAD.rhs.empty() && !mAD.lhs.empty())
            {
                lo() << mAD.lhs << "\n" << mAD.rhs << "\n\n";
            }

            if(getAssertState().skip)
            {
                lo() << "Skipping assertion..." << std::endl;
                return;
            }

            lo() << "Choose what to do:\n\n"
                 << "(  0  ) -> Skip this assertion.\n"
                 << "(  1  ) -> Skip all assertions.\n"
                 << "( ... ) -> Terminate the program." << std::endl;

            std::string userInput;
            std::cin >> userInput;

            if(userInput == "0")
            {
                return;
            }
            if(userInput == "1")
            {
                getAssertState().skip = true;
                return;
            }

            std::terminate();
        }
    }
}

#endif

#endif
