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

			auto closestMatch(std::make_pair(std::numeric_limits<SizeT>::max(), std::string{}));

			for(const auto& c : cmds)
				for(const auto& n : c->getNames())
				{
					const auto& dist(getDistLevenshtein(n, mName));
					if(dist < closestMatch.first) closestMatch = {dist, n};
				}

			throw Exception::createCmdNotFound(mName, closestMatch.second);
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
						if(cFlags.size() > cmd.getCount<EType::Flag>()) throw Exception::createSignatureMismatch(getForCmdPhrase(cmd), "flags", toStr(cmd.getCount<EType::Flag>()));
					}
					else
					{
						auto fName(s.substr(0, equalPos));
						auto fValue(s.substr(equalPos + 1, s.size() - equalPos));

						if(anyOf(cmd.getAll<EType::FlagValueOpt>(), [&fName](auto mFVO){ return castUp<Internal::BaseFlag>(mFVO)->hasName(fName); }))
						{
							cFlagValuesOpt.emplace_back(s);
							if(cFlagValuesOpt.size() > cmd.getCount<EType::FlagValueOpt>()) throw Exception::createSignatureMismatch(getForCmdPhrase(cmd), "optional flag values", toStr(cmd.getCount<EType::FlagValueOpt>()));

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

			if(cFlagValues.size() != cmd.getCount<EType::FlagValue>()) throw Exception::createSignatureMismatch(getForCmdPhrase(cmd), "required flag values", toStr(cmd.getCount<EType::FlagValue>()));

			for(const auto& f : cFlags) eraseRemove(entered, f);
			for(const auto& f : cFlagValues) eraseRemove(entered, f);
			for(const auto& f : cFlagValuesOpt) eraseRemove(entered, f);

			// Find args, put them in cArgs
			std::vector<std::string> cArgs;
			for(auto i(cmd.getCount<EType::Arg>()); i > 0; --i)
			{
				if(entered.empty()) throw Exception::createSignatureMismatch(getForCmdPhrase(cmd), "args", toStr(cmd.getCount<EType::Arg>()));

				cArgs.emplace_back(entered.front());
				entered.pop_front();
			}

			// Remaining std::string in args must be optargs
			std::vector<std::string> cArgsOpt;
			for(auto i(cmd.getCount<EType::ArgOpt>()); i > 0; --i)
			{
				if(entered.empty()) break;
				cArgsOpt.emplace_back(entered.front());
				if(cArgsOpt.size() > cmd.getCount<EType::ArgOpt>()) throw Exception::createSignatureMismatch(getForCmdPhrase(cmd), "optional args", toStr(cmd.getCount<EType::ArgOpt>()));
				entered.pop_front();
			}

			// Check for argpacks
			for(auto i(0u); i < cmd.getCount<EType::ArgPack>(); ++i)
			{
				auto& argPack(cmd.getAt<EType::ArgPack>(i));
				std::vector<std::string> toPack;

				if(argPack.isInfinite())
				{
					if(i != cmd.getCount<EType::ArgPack>() -1) throw Exception::createArgPackInfinitePositionError();
					for(; !entered.empty(); entered.pop_front()) toPack.emplace_back(entered.front());
				}
				else
				{
					if(entered.size() < argPack.getMin()) throw Exception::createArgPackSignatureMismatch(argPack);

					auto clampedCount(getClamped(entered.size(), 0u, argPack.getMax()));
					for(auto iS(0u); iS < clampedCount; ++iS) { toPack.emplace_back(entered.front()); entered.pop_front(); }
				}

				argPack.set(toPack);
			}

			// If there still stuff left, there are too many arguments!
			if(!entered.empty()) throw Exception::createParsingError();

			for(auto i(0u); i < cArgs.size(); ++i) cmd.setElementValue<EType::Arg>(i, cArgs[i]);
			for(auto i(0u); i < cArgsOpt.size(); ++i) cmd.setElementValue<EType::ArgOpt>(i, cArgsOpt[i]);
			for(const auto& f : cFlags) cmd.activateFlag(f);
			for(auto i(0u); i < cFlagValuesValues.size(); ++i) cmd.setElementValue<EType::FlagValue>(i, cFlagValuesValues[i]);
			for(auto i(0u); i < cFlagValuesOptValues.size(); ++i) cmd.setElementValue<EType::FlagValueOpt>(i, cFlagValuesOptValues[i]);

			cmd();
		}
	}
}

#endif
