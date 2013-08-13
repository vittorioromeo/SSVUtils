// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/CommandLine/Elements/Bases/ArgBase.h"
#include "SSVUtils/CommandLine/Elements/Bases/ArgPackBase.h"
#include "SSVUtils/CommandLine/Elements/Arg.h"
#include "SSVUtils/CommandLine/Elements/Flag.h"
#include "SSVUtils/CommandLine/Cmd.h"

using namespace std;

namespace ssvu
{
	namespace CommandLine
	{
		Flag& Cmd::findFlag(const string& mName)
		{
			for(const auto& f : flags) if(f->hasName(mName)) return *f;
			throw runtime_error("No flag with name '" + mName + "' in command " + getNamesStr());
		}

		Flag& Cmd::createFlag(const string& mShortName, const string& mLongName) { auto result(new Flag{mShortName, mLongName}); flags.emplace_back(result); return *result; }

		void Cmd::setArgValue(unsigned int mIndex, const string& mValue)	{ args[mIndex]->set(mValue); }
		void Cmd::setOptArgValue(unsigned int mIndex, const string& mValue)	{ optArgs[mIndex]->set(mValue); }

		void Cmd::activateFlag(const string& mName) { findFlag(mName) = true; }
		bool Cmd::isFlagActive(unsigned int mIndex) const { return *flags[mIndex]; }

		string Cmd::getNamesStr() const
		{
			string result{"<"};
			for(auto i(0u); i < names.size(); ++i)
			{
				result.append(names[i]);
				if(i < names.size() - 1) result.append(" || ");
			}
			result.append(">");
			return result;
		}
		string Cmd::getArgsStr() const
		{
			string result;
			for(auto i(0u); i < args.size(); ++i)
			{
				result.append(args[i]->getUsageStr());
				if(i < args.size() - 1) result.append(" ");
			}
			return result;
		}
		string Cmd::getOptArgsStr() const
		{
			string result;
			for(auto i(0u); i < optArgs.size(); ++i)
			{
				result.append(optArgs[i]->getUsageStr());
				if(i < optArgs.size() - 1) result.append(" ");
			}
			return result;
		}
		string Cmd::getArgPacksStr() const
		{
			string result;
			for(auto i(0u); i < argPacks.size(); ++i)
			{
				result.append(argPacks[i]->getUsageStr());
				if(i < argPacks.size() - 1) result.append(" ");
			}
			return result;
		}
		string Cmd::getFlagsStr() const
		{
			string result;
			for(auto i(0u); i < flags.size(); ++i)
			{
				result.append(flags[i]->getUsageStr());
				if(i < flags.size() - 1) result.append(" ");
			}
			return result;
		}
		string Cmd::getHelpStr() const
		{
			string result;

			if(!desc.empty()) result += ">>" + desc + "\n\n";

			if(!args.empty()) result += "\t" "Required arguments:" "\n";
			for(const auto& a : args) result += a->getHelpStr();
			if(!args.empty()) result += "\n";

			if(!optArgs.empty()) result += "\t" "Optional arguments:" "\n";
			for(const auto& a : optArgs) result += a->getHelpStr();
			if(!optArgs.empty()) result += "\n";

			if(!argPacks.empty()) result += "\t" "Argument packs:" "\n";
			for(const auto& p : argPacks) result += p->getHelpStr();
			if(!argPacks.empty()) result += "\n";

			if(!flags.empty()) result += "\t" "Flags:" "\n";
			for(const auto& f : flags) result += f->getHelpStr();

			return result;
		}
	}
}
