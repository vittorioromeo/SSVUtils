// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <deque>
#include <stdexcept>
#include <limits>
#include "SSVUtils/Utils/UtilsMath.h"
#include "SSVUtils/CommandLine/Cmd.h"
#include "SSVUtils/CommandLine/Elements/Flag.h"
#include "SSVUtils/CommandLine/CmdLine.h"
#include "SSVUtils/CommandLine/Elements/Bases/ArgBase.h"
#include "SSVUtils/CommandLine/Elements/Bases/ArgPackBase.h"

using namespace std;

namespace ssvu
{
	namespace CommandLine
	{
		Cmd& CmdLine::findCmd(const string& mName) const
		{
			for(const auto& c : cmds) if(c->hasName(mName)) return *c;

			pair<unsigned int, string> closestMatch{numeric_limits<unsigned int>::max(), ""};

			for(const auto& c : cmds)
				for(const auto& n : c->getNames())
				{
					const auto& dist(getLevenshteinDistance(n, mName));
					if(dist < closestMatch.first) closestMatch = {dist, n};
				}

			throw runtime_error("No command with name <" + mName + ">\nDid you mean <" + closestMatch.second + ">?");
		}
		Cmd& CmdLine::create(const initializer_list<string>& mNames) { auto result(new Cmd{mNames}); cmds.emplace_back(result); return *result; }
		void CmdLine::parseCmdLine(const vector<string>& mArgs)
		{
			deque<string> entered{begin(mArgs), end(mArgs)};

			// args.front() is expected to be the command name
			Cmd& cmd(findCmd(entered.front()));
			entered.pop_front();

			// Find all flags, put them in cFlags, remove them from mArgs
			vector<string> cFlags;
			for(const auto& s : entered)
			{
				if(beginsWith(s, flagPrefixShort) || beginsWith(s, flagPrefixLong))
				{
					cFlags.push_back(s);
					if(cFlags.size() > cmd.getFlagCount()) throw runtime_error("Incorrect number of flags for command " + cmd.getNamesStr() + " , correct number is '" + toStr(cmd.getFlagCount()) + "'");
				}
			}
			for(const auto& f : cFlags) eraseRemove(entered, f);

			// Find args, put them in cArgs
			vector<string> cArgs;
			for(auto i(cmd.getArgCount()); i > 0; --i)
			{
				if(entered.empty()) throw runtime_error("Incorrect number of args for command " + cmd.getNamesStr() + " , correct number is '" + toStr(cmd.getArgCount()) + "'");
				cArgs.push_back(entered.front());
				entered.pop_front();
			}

			// Remaining string in args must be optargs
			vector<string> cOptArgs;
			for(auto i(cmd.getOptArgCount()); i > 0; --i)
			{
				if(entered.empty()) break;
				cOptArgs.push_back(entered.front());
				if(cOptArgs.size() > cmd.getOptArgCount()) throw runtime_error("Incorrect number of optargs for command " + cmd.getNamesStr() + " , correct number is '" + toStr(cmd.getOptArgCount()) + "'");
				entered.pop_front();
			}

			// Check for argpacks
			for(auto i(0u); i < cmd.getArgPackCount(); ++i)
			{
				auto& argPack(*cmd.getArgPacks()[i]);
				vector<string> toPack;

				if(argPack.isInfinite())
				{
					if(i != cmd.getArgPackCount() -1) throw runtime_error("Infinite argpacks must be last");
					while(!entered.empty()) { toPack.push_back(entered.front()); entered.pop_front(); }
				}
				else
				{
					if(entered.size() < argPack.getMin()) throw runtime_error("Not enough args for finite argpack");

					unsigned int clampedCount{getClamped<unsigned int>(entered.size(), 0, argPack.getMax())};
					for(auto iS(0u); iS < clampedCount; ++iS) { toPack.push_back(entered.front()); entered.pop_front(); }
				}

				argPack.set(toPack);
			}

			// If there still stuff left, there are too many arguments!
			if(!entered.empty()) throw runtime_error("Too many arguments!");

			for(auto i(0u); i < cArgs.size(); ++i) cmd.setArgValue(i, cArgs[i]);
			for(auto i(0u); i < cOptArgs.size(); ++i) cmd.setOptArgValue(i, cOptArgs[i]);
			for(const auto& f : cFlags) cmd.activateFlag(f);
			cmd();
		}
	}
}
