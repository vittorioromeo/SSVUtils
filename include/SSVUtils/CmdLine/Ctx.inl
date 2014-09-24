// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_CTX_INL
#define SSVU_CMDLINE_CTX_INL

namespace ssvu
{
	namespace CmdLine
	{
		inline Cmd& Ctx::findCmd(const std::string& mName) const
		{
			for(const auto& c : cmds) if(c->hasName(mName)) return *c;

			auto closestMatch(std::make_pair(std::numeric_limits<std::size_t>::max(), std::string{}));

			for(const auto& c : cmds)
				for(const auto& n : c->getNames())
				{
					const auto& dist(getDistLevenshtein(n, mName));
					if(dist < closestMatch.first) closestMatch = {dist, n};
				}

			throw std::runtime_error("No command with name <" + mName + ">\nDid you mean <" + closestMatch.second + ">?");
		}
		inline Cmd& Ctx::create(const std::initializer_list<std::string>& mNames) { return getEmplaceUPtr<Cmd>(cmds, mNames); }
		inline void Ctx::process(const std::vector<std::string>& mArgs)
		{
			std::deque<std::string> entered{std::begin(mArgs), std::end(mArgs)};

			// args.front() is expected to be either the command name or a flag
			// If it is a flag, then we're gonna use the "main" command instead

			bool useCmdMain{false};

			if(entered.empty() || beginsAsFlag(entered.front())) useCmdMain = true;

			auto& cmd(useCmdMain ? cmdMain : findCmd(entered.front()));
			if(!useCmdMain) entered.pop_front();

			// Find all flags or flag values, put them in cFlags or cFlagValues, remove them from mArgs
			std::vector<std::string> cFlags, cFlagValues, cFlagValuesOpt;
			std::vector<std::string> cFlagValuesValues, cFlagValuesOptValues;
			for(const auto& s : entered)
			{
				if(beginsAsFlag(s))
				{
					auto equalPos(s.find('='));

					if(equalPos == std::string::npos)
					{
						cFlags.emplace_back(s);
						if(cFlags.size() > cmd.getFlagCount()) throw std::runtime_error("Incorrect number of flags" + getForCmdPhrase(cmd) + ", correct number is '" + toStr(cmd.getFlagCount()) + "'");
					}
					else
					{
						auto fName(s.substr(0, equalPos));
						auto fValue(s.substr(equalPos + 1, s.size() - equalPos));

						if(anyOf(cmd.getFlagValuesOpt(), [&fName](const auto& mFVO){ return mFVO->hasName(fName); }))
						{
							cFlagValuesOpt.emplace_back(s);
							if(cFlagValuesOpt.size() > cmd.getFlagValueOptCount()) throw std::runtime_error("Incorrect number of flagvaluesopt" + getForCmdPhrase(cmd) + ", correct number is '" + toStr(cmd.getFlagValueOptCount()) + "'");

							cFlagValuesOptValues.emplace_back(fValue);
						}
						else
						{
							cFlagValues.emplace_back(s);
							cFlagValuesValues.emplace_back(fValue);
						}

					}
				}
			}

			if(cFlagValues.size() != cmd.getFlagValueCount()) throw std::runtime_error("Incorrect number of flag values" + getForCmdPhrase(cmd) + ", correct number is '" + toStr(cmd.getFlagValueCount()) + "'");

			for(const auto& f : cFlags) eraseRemove(entered, f);
			for(const auto& f : cFlagValues) eraseRemove(entered, f);
			for(const auto& f : cFlagValuesOpt) eraseRemove(entered, f);

			// Find args, put them in cArgs
			std::vector<std::string> cArgs;
			for(auto i(cmd.getArgCount()); i > 0; --i)
			{
				if(entered.empty()) throw std::runtime_error("Incorrect number of args" + getForCmdPhrase(cmd) + ", correct number is '" + toStr(cmd.getArgCount()) + "'");
				cArgs.emplace_back(entered.front());
				entered.pop_front();
			}

			// Remaining std::string in args must be optargs
			std::vector<std::string> cArgsOpt;
			for(auto i(cmd.getArgOptCount()); i > 0; --i)
			{
				if(entered.empty()) break;
				cArgsOpt.emplace_back(entered.front());
				if(cArgsOpt.size() > cmd.getArgOptCount()) throw std::runtime_error("Incorrect number of argsopt" + getForCmdPhrase(cmd) + ", correct number is '" + toStr(cmd.getArgOptCount()) + "'");
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
					while(!entered.empty()) { toPack.emplace_back(entered.front()); entered.pop_front(); }
				}
				else
				{
					if(entered.size() < argPack.getMin()) throw std::runtime_error("Not enough args for finite argpack");

					auto clampedCount(getClamped(entered.size(), 0u, argPack.getMax()));
					for(auto iS(0u); iS < clampedCount; ++iS) { toPack.emplace_back(entered.front()); entered.pop_front(); }
				}

				argPack.set(toPack);
			}

			// If there still stuff left, there are too many arguments!
			if(!entered.empty()) throw std::runtime_error("Too many arguments!");

			for(auto i(0u); i < cArgs.size(); ++i) cmd.setArgValue(i, cArgs[i]);
			for(auto i(0u); i < cArgsOpt.size(); ++i) cmd.setArgOptValue(i, cArgsOpt[i]);
			for(const auto& f : cFlags) cmd.activateFlag(f);
			for(auto i(0u); i < cFlagValuesValues.size(); ++i) cmd.setFlagValueValue(i, cFlagValuesValues[i]);
			for(auto i(0u); i < cFlagValuesOptValues.size(); ++i) cmd.setFlagValueOptValue(i, cFlagValuesOptValues[i]);

			cmd();
		}
	}
}

#endif
