// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_CMDLINE_INL
#define SSVU_COMMANDLINE_CMDLINE_INL

#include <deque>
#include <stdexcept>
#include <limits>
#include "SSVUtils/Utils/UtilsMath.h"
#include "SSVUtils/CommandLine/Cmd.h"
#include "SSVUtils/CommandLine/Elements/Flag.h"
#include "SSVUtils/CommandLine/CmdLine.h"
#include "SSVUtils/CommandLine/Elements/Bases/ArgBase.h"
#include "SSVUtils/CommandLine/Elements/Bases/ArgPackBase.h"

namespace ssvu
{
	namespace CommandLine
	{
		inline Cmd& CmdLine::findCmd(const std::string& mName) const
		{
			for(const auto& c : cmds) if(c->hasName(mName)) return *c;

			std::pair<unsigned int, std::string> closestMatch{std::numeric_limits<unsigned int>::max(), ""};

			for(const auto& c : cmds)
				for(const auto& n : c->getNames())
				{
					const auto& dist(getLevenshteinDistance(n, mName));
					if(dist < closestMatch.first) closestMatch = {dist, n};
				}

			throw std::runtime_error("No command with name <" + mName + ">\nDid you mean <" + closestMatch.second + ">?");
		}
		inline Cmd& CmdLine::create(const std::initializer_list<std::string>& mNames) { auto result(new Cmd{mNames}); cmds.emplace_back(result); return *result; }
		inline void CmdLine::parseCmdLine(const std::vector<std::string>& mArgs)
		{
			if(mArgs.empty()) return;

			std::deque<std::string> entered{begin(mArgs), end(mArgs)};

			// args.front() is expected to be the command name
			Cmd& cmd(findCmd(entered.front()));
			entered.pop_front();

			// Find all flags, put them in cFlags, remove them from mArgs
			std::vector<std::string> cFlags;
			for(const auto& s : entered)
			{
				if(beginsWith(s, flagPrefixShort) || beginsWith(s, flagPrefixLong))
				{
					cFlags.push_back(s);
					if(cFlags.size() > cmd.getFlagCount()) throw std::runtime_error("Incorrect number of flags for command " + cmd.getNamesStr() + " , correct number is '" + toStr(cmd.getFlagCount()) + "'");
				}
			}
			for(const auto& f : cFlags) eraseRemove(entered, f);

			// Find args, put them in cArgs
			std::vector<std::string> cArgs;
			for(auto i(cmd.getArgCount()); i > 0; --i)
			{
				if(entered.empty()) throw std::runtime_error("Incorrect number of args for command " + cmd.getNamesStr() + " , correct number is '" + toStr(cmd.getArgCount()) + "'");
				cArgs.push_back(entered.front());
				entered.pop_front();
			}

			// Remaining std::string in args must be optargs
			std::vector<std::string> cOptArgs;
			for(auto i(cmd.getOptArgCount()); i > 0; --i)
			{
				if(entered.empty()) break;
				cOptArgs.push_back(entered.front());
				if(cOptArgs.size() > cmd.getOptArgCount()) throw std::runtime_error("Incorrect number of optargs for command " + cmd.getNamesStr() + " , correct number is '" + toStr(cmd.getOptArgCount()) + "'");
				entered.pop_front();
			}

			// Check for argpacks
			for(auto i(0u); i < cmd.getArgPackCount(); ++i)
			{
				auto& argPack(*cmd.getArgPacks()[i]);
				std::vector<std::string> toPack;

				if(argPack.isInfinite())
				{
					if(i != cmd.getArgPackCount() -1) throw std::runtime_error("Infinite argpacks must be last");
					while(!entered.empty()) { toPack.push_back(entered.front()); entered.pop_front(); }
				}
				else
				{
					if(entered.size() < argPack.getMin()) throw std::runtime_error("Not enough args for finite argpack");

					auto clampedCount(getClamped<size_t>(entered.size(), 0, argPack.getMax()));
					for(auto iS(0u); iS < clampedCount; ++iS) { toPack.push_back(entered.front()); entered.pop_front(); }
				}

				argPack.set(toPack);
			}

			// If there still stuff left, there are too many arguments!
			if(!entered.empty()) throw std::runtime_error("Too many arguments!");

			for(auto i(0u); i < cArgs.size(); ++i) cmd.setArgValue(i, cArgs[i]);
			for(auto i(0u); i < cOptArgs.size(); ++i) cmd.setOptArgValue(i, cOptArgs[i]);
			for(const auto& f : cFlags) cmd.activateFlag(f);
			cmd();
		}
	}
}

#endif
