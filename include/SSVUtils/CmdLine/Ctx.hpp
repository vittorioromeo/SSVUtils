// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_CTX
#define SSVU_CMDLINE_CTX

namespace ssvu
{
	namespace CmdLine
	{
		class Ctx
		{
			private:
				VecUPtr<Cmd> cmds;
				Cmd cmdMain{Cmd::createCmdMain()};

				inline bool beginsAsFlag(const std::string& mStr) const noexcept
				{
					return beginsWith(mStr, Internal::flagPrefixShort) || beginsWith(mStr, Internal::flagPrefixLong);
				}

				inline auto getForCmdPhrase(Cmd& mCmd) const noexcept
				{
					return mCmd.isMainCmd() ? ""s : " for command "s + mCmd.getNamesStr();
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

				inline const auto& getCmds() const noexcept { return cmds; }
				inline auto& getCmdMain() noexcept { return cmdMain; }
		};
	}
}

#endif
