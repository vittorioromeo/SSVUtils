// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_CMDLINE
#define SSVU_COMMANDLINE_CMDLINE

namespace ssvu
{
	namespace CommandLine
	{
		class Cmd;

		class CmdLine
		{
			private:
				VecUptr<Cmd> cmds;

			public:
				Cmd& findCmd(const std::string& mName) const;
				Cmd& create(const std::initializer_list<std::string>& mNames);
				void parseCmdLine(const std::vector<std::string>& mArgs);
				inline const decltype(cmds)& getCmds() const noexcept { return cmds; }
		};
	}
}

#endif
