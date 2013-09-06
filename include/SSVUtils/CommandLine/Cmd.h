// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_CMD
#define SSVU_COMMANDLINE_CMD

#include <string>
#include <vector>
#include <functional>
#include "SSVUtils/Global/Typedefs.h"
#include "SSVUtils/Utils/UtilsContainers.h"
#include "SSVUtils/CommandLine/Elements/Flag.h"
#include "SSVUtils/CommandLine/Elements/Bases/ArgBase.h"
#include "SSVUtils/CommandLine/Elements/Bases/ArgPackBase.h"

namespace ssvu
{
	namespace CommandLine
	{
		class Flag;
		struct ArgBase;
		class ArgPackBase;
		template<typename T> class Arg;
		template<typename T> class OptArg;
		template<typename T> class ArgPack;

		namespace Internal
		{
			// These functions exist only to avoid some code repetition below
			template<typename T> std::string buildCmdStr(const T& mC, std::string mStart = "", const std::string& mEnd = "", const std::string& mSep = " ")
			{
				for(auto i(0u); i < mC.size(); ++i) { mStart += mC[i]->getUsageStr(); if(i < mC.size() - 1) mStart += mSep; }
				return mStart + mEnd;
			}
			template<template<typename...> class T> std::string buildCmdStr(const T<std::string>& mC, std::string mStart = "", const std::string& mEnd = "", const std::string& mSep = " ")
			{
				for(auto i(0u); i < mC.size(); ++i) { mStart += mC[i]; if(i < mC.size() - 1) mStart += mSep; }
				return mStart + mEnd;
			}
		}

		class Cmd
		{
			friend class CmdLine;

			private:
				std::vector<std::string> names;
				std::vector<Uptr<ArgBase>> args, optArgs;
				std::vector<Uptr<ArgPackBase>> argPacks;
				std::vector<Uptr<Flag>> flags;
				Action func;
				std::string desc;

				inline Flag& findFlag(const std::string& mName)
				{
					for(const auto& f : flags) if(f->hasName(mName)) return *f;
					throw std::runtime_error("No flag with name '" + mName + "' in command " + getNamesStr());
				}

				inline void setArgValue(unsigned int mIndex, const std::string& mValue)		{ args[mIndex]->set(mValue); }
				inline void setOptArgValue(unsigned int mIndex, const std::string& mValue)	{ optArgs[mIndex]->set(mValue); }

			public:
				Cmd(const std::initializer_list<std::string>& mNames) : names{mNames} { }

				inline Cmd& operator+=(Action mFunc) { func = mFunc; return *this; }
				inline Cmd& operator()() { func(); return *this; }

				template<typename T> inline Arg<T>& createArg()												{ auto result(new Arg<T>()); args.emplace_back(result); return *result; }
				template<typename T> inline OptArg<T>& createOptArg(const T& mDefaultValue)					{ auto result(new OptArg<T>(mDefaultValue)); optArgs.emplace_back(result); return *result; }
				template<typename T> inline ArgPack<T>& createArgPack(unsigned int mMin, unsigned int mMax)	{ auto result(new ArgPack<T>(mMin, mMax)); argPacks.emplace_back(result); return *result; }
				template<typename T> inline ArgPack<T>& createInfiniteArgPack()								{ auto result(new ArgPack<T>); argPacks.emplace_back(result); return *result; }
				inline Flag& createFlag(const std::string& mShortName, const std::string& mLongName)		{ auto result(new Flag{mShortName, mLongName}); flags.emplace_back(result); return *result; }

				inline bool isFlagActive(unsigned int mIndex) const	{ return *flags[mIndex]; }
				inline void activateFlag(const std::string& mName)	{ findFlag(mName) = true; }

				inline bool hasName(const std::string& mName) const		{ return contains(names, mName); }
				inline std::size_t getArgCount() const					{ return args.size(); }
				inline std::size_t getOptArgCount() const				{ return optArgs.size(); }
				inline std::size_t getArgPackCount() const				{ return argPacks.size(); }
				inline std::size_t getFlagCount() const					{ return flags.size(); }
				inline const decltype(names)& getNames() const			{ return names; }
				inline const decltype(args)& getArgs() const			{ return args; }
				inline const decltype(optArgs)& getOptArgs() const		{ return optArgs; }
				inline const decltype(argPacks)& getArgPacks() const	{ return argPacks; }
				inline const decltype(flags)& getFlags() const			{ return flags; }

				std::string getNamesStr() const		{ return Internal::buildCmdStr(names, "<", ">", " || "); }
				std::string getArgsStr() const		{ return Internal::buildCmdStr(args); }
				std::string getOptArgsStr() const	{ return Internal::buildCmdStr(optArgs); }
				std::string getArgPacksStr() const	{ return Internal::buildCmdStr(argPacks); }
				std::string getFlagsStr() const		{ return Internal::buildCmdStr(flags); }
				std::string getHelpStr() const
				{
					std::string result;

					if(!desc.empty()) result += ">>" + desc + "\n\n";

					if(!args.empty())		{ result += "\t" "Required arguments:" "\n"; for(const auto& a : args) result += a->getHelpStr(); result += "\n"; }
					if(!optArgs.empty())	{ result += "\t" "Optional arguments:" "\n"; for(const auto& a : optArgs) result += a->getHelpStr(); result += "\n"; }
					if(!argPacks.empty())	{ result += "\t" "Argument packs:" "\n"; for(const auto& p : argPacks) result += p->getHelpStr(); result += "\n"; }
					if(!flags.empty())		{ result += "\t" "Flags:" "\n"; for(const auto& f : flags) result += f->getHelpStr(); }

					return result;
				}

				inline void setDesc(const std::string& mDesc)	{ desc = mDesc; }
				inline const std::string& getDesc() const		{ return desc; }
		};
	}
}

#endif
