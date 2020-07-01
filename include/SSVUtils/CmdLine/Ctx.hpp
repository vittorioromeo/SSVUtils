// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_CTX
#define SSVU_CMDLINE_CTX

#include "SSVUtils/Delegate/Delegate.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"
#include "SSVUtils/CmdLine/Cmd.hpp"

#include <string>
#include <vector>
#include <memory>

namespace ssvu
{
namespace CmdLine
{
class Ctx
{
private:
    std::vector<std::unique_ptr<Cmd>> cmds;
    Cmd cmdMain{Cmd::createCmdMain()};

    inline bool beginsAsFlag(const std::string& mStr) const noexcept
    {
        return beginsWith(mStr, Impl::flagPrefixShort) ||
               beginsWith(mStr, Impl::flagPrefixLong);
    }

    inline auto getForCmdPhrase(Cmd& mCmd) const noexcept
    {
        return mCmd.isMainCmd()
                   ? std::string{""}
                   : std::string{" for command "} + mCmd.getNamesStr();
    }

public:
    Cmd& findCmd(const std::string& mName) const;
    Cmd& create(const std::initializer_list<std::string>& mNames);

    void process(const std::vector<std::string>& mArgs);
    inline void process(int mArgCount, char* mArgValues[])
    {
        std::vector<std::string> args;
        for(int i{1}; i < mArgCount; ++i) args.emplace_back(mArgValues[i]);
        process(args);
    }

    inline const auto& getCmds() const noexcept
    {
        return cmds;
    }
    inline auto& getCmdMain() noexcept
    {
        return cmdMain;
    }
};
} // namespace CmdLine
} // namespace ssvu

#endif
