// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_COMMANDLINE_CMD
#define SSVU_COMMANDLINE_CMD

#include "SSVUtils/Delegate/Delegate.hpp"

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
				VecUPtr<ArgBase> args, optArgs;
				VecUPtr<ArgPackBase> argPacks;
				VecUPtr<Flag> flags;
				Delegate<void()> onAction;
				std::string desc;

				inline Flag& findFlag(const std::string& mName)
				{
					for(const auto& f : flags) if(f->hasName(mName)) return *f;
					throw std::runtime_error("No flag with name '" + mName + "' in command " + getNamesStr());
				}

				inline void setArgValue(unsigned int mIdx, std::string mValue)		{ args[mIdx]->set(std::move(mValue)); }
				inline void setOptArgValue(unsigned int mIdx, std::string mValue)	{ optArgs[mIdx]->set(std::move(mValue)); }

			public:
				Cmd(const std::initializer_list<std::string>& mNames) : names{mNames} { }

				inline Cmd& operator+=(const Action& mFunc) { onAction += mFunc; return *this; }
				inline Cmd& operator()() { onAction(); return *this; }

				template<typename T> inline auto& createArg()											{ return ssvu::getEmplaceUPtr<Arg<T>>(args); }
				template<typename T> inline auto& createOptArg(const T& mDefaultValue)					{ return ssvu::getEmplaceUPtr<OptArg<T>>(optArgs, mDefaultValue); }
				template<typename T> inline auto& createArgPack(unsigned int mMin, unsigned int mMax)	{ return ssvu::getEmplaceUPtr<ArgPack<T>>(argPacks, mMin, mMax); }
				template<typename T> inline auto& createInfiniteArgPack()								{ return ssvu::getEmplaceUPtr<ArgPack<T>>(argPacks); }
				inline Flag& createFlag(std::string mShortName, std::string mLongName)					{ return ssvu::getEmplaceUPtr<Flag>(flags, std::move(mShortName), std::move(mLongName)); }

				inline bool isFlagActive(unsigned int mIdx) const	{ return *flags[mIdx]; }
				inline void activateFlag(const std::string& mName)	{ findFlag(mName) = true; }

				inline bool hasName(const std::string& mName) const	{ return contains(names, mName); }
				inline auto getArgCount() const noexcept			{ return args.size(); }
				inline auto getOptArgCount() const noexcept			{ return optArgs.size(); }
				inline auto getArgPackCount() const noexcept		{ return argPacks.size(); }
				inline auto getFlagCount() const noexcept			{ return flags.size(); }
				inline const auto& getNames() const noexcept		{ return names; }
				inline const auto& getArgs() const noexcept			{ return args; }
				inline const auto& getOptArgs() const noexcept		{ return optArgs; }
				inline const auto& getArgPacks() const noexcept		{ return argPacks; }
				inline const auto& getFlags() const noexcept		{ return flags; }

				std::string getNamesStr() const	{ return Internal::buildCmdStr(names, "<", ">", " || "); }
				auto getArgsStr() const		{ return Internal::buildCmdStr(args); }
				auto getOptArgsStr() const	{ return Internal::buildCmdStr(optArgs); }
				auto getArgPacksStr() const	{ return Internal::buildCmdStr(argPacks); }
				auto getFlagsStr() const	{ return Internal::buildCmdStr(flags); }
				auto getHelpStr() const
				{
					std::string result;

					if(!desc.empty()) result += ">>" + desc + "\n\n";

					if(!args.empty())		{ result += "\t" "Required arguments:" "\n"; for(const auto& a : args) result += a->getHelpStr(); result += "\n"; }
					if(!optArgs.empty())	{ result += "\t" "Optional arguments:" "\n"; for(const auto& a : optArgs) result += a->getHelpStr(); result += "\n"; }
					if(!argPacks.empty())	{ result += "\t" "Argument packs:" "\n"; for(const auto& p : argPacks) result += p->getHelpStr(); result += "\n"; }
					if(!flags.empty())		{ result += "\t" "Flags:" "\n"; for(const auto& f : flags) result += f->getHelpStr(); }

					return result;
				}

				inline void setDesc(std::string mDesc)		{ desc = std::move(mDesc); }
				inline const auto& getDesc() const noexcept	{ return desc; }
		};
	}
}

#endif
