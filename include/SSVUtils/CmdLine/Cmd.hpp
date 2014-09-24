// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_CMD
#define SSVU_CMDLINE_CMD

#include "SSVUtils/Delegate/Delegate.hpp"

namespace ssvu
{
	namespace CmdLine
	{
		class Flag;
		struct ArgBase;
		class ArgPackBase;
		template<typename T> class Arg;
		template<typename T> class ArgOpt;
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
			friend class Ctx;

			private:
				std::vector<std::string> names;
				VecUPtr<ArgBase> args, argsOpt;
				VecUPtr<ArgPackBase> argPacks;
				VecUPtr<Flag> flags;
				VecUPtr<FlagValueBase> flagValues, flagValuesOpt;
				Delegate<void()> onAction;
				std::string desc;
				bool main{false}; // Is this the main cmd?

				inline Cmd() = default;
				inline static Cmd createCmdMain() { Cmd result; result.main = true; return result; }

				inline Flag& findFlag(const std::string& mName)
				{
					for(const auto& f : flags) if(f->hasName(mName)) return *f;
					throw std::runtime_error("No flag with name '" + mName + "' in command " + getNamesStr());
				}

				inline void setArgValue(std::size_t mIdx, const std::string& mValue)			{ args[mIdx]->set(mValue); }
				inline void setArgOptValue(std::size_t mIdx, const std::string& mValue)			{ argsOpt[mIdx]->set(mValue); }
				inline void setFlagValueValue(std::size_t mIdx, const std::string& mValue)		{ flagValues[mIdx]->set(mValue); }
				inline void setFlagValueOptValue(std::size_t mIdx, const std::string& mValue)	{ flagValuesOpt[mIdx]->set(mValue); }

			public:
				inline Cmd(const std::initializer_list<std::string>& mNames) : names{mNames} { }

				inline Cmd& operator+=(const Action& mFunc) { onAction += mFunc; return *this; }
				inline Cmd& operator()() { onAction(); return *this; }

				template<typename T> inline auto& createArg()																				{ return ssvu::getEmplaceUPtr<Arg<T>>(args); }
				template<typename T> inline auto& createArgOpt(const T& mDefaultValue)														{ return ssvu::getEmplaceUPtr<ArgOpt<T>>(argsOpt, mDefaultValue); }
				template<typename T> inline auto& createArgPack(std::size_t mMin, std::size_t mMax)											{ return ssvu::getEmplaceUPtr<ArgPack<T>>(argPacks, mMin, mMax); }
				template<typename T> inline auto& createInfiniteArgPack()																	{ return ssvu::getEmplaceUPtr<ArgPack<T>>(argPacks); }
				inline Flag& createFlag(std::string mShortName, std::string mLongName)														{ return ssvu::getEmplaceUPtr<Flag>(flags, std::move(mShortName), std::move(mLongName)); }
				template<typename T> inline auto& createFlagValue(std::string mShortName, std::string mLongName)							{ return ssvu::getEmplaceUPtr<FlagValue<T>>(flagValues, std::move(mShortName), std::move(mLongName)); }
				template<typename T> inline auto& createFlagValueOpt(std::string mShortName, std::string mLongName, const T& mDefaultValue)	{ return ssvu::getEmplaceUPtr<FlagValueOpt<T>>(flagValuesOpt, std::move(mShortName), std::move(mLongName), mDefaultValue); }

				inline bool isFlagActive(std::size_t mIdx) const	{ return *flags[mIdx]; }
				inline void activateFlag(const std::string& mName)	{ findFlag(mName) = true; }

				inline bool hasName(const std::string& mName) const		{ return contains(names, mName); }
				inline bool isMain() const noexcept						{ return main; }
				inline auto getArgCount() const noexcept				{ return args.size(); }
				inline auto getArgOptCount() const noexcept				{ return argsOpt.size(); }
				inline auto getArgPackCount() const noexcept			{ return argPacks.size(); }
				inline auto getFlagCount() const noexcept				{ return flags.size(); }
				inline auto getFlagValueCount() const noexcept			{ return flagValues.size(); }
				inline auto getFlagValueOptCount() const noexcept		{ return flagValuesOpt.size(); }
				inline const auto& getNames() const noexcept			{ return names; }
				inline const auto& getArgs() const noexcept				{ return args; }
				inline const auto& getArgsOpt() const noexcept			{ return argsOpt; }
				inline const auto& getArgPacks() const noexcept			{ return argPacks; }
				inline const auto& getFlags() const noexcept			{ return flags; }
				inline const auto& getFlagValues() const noexcept		{ return flagValues; }
				inline const auto& getFlagValuesOpt() const noexcept	{ return flagValuesOpt; }

				std::string getNamesStr() const		{ return Internal::buildCmdStr(names, "<", ">", " || "); }
				auto getArgsStr() const				{ return Internal::buildCmdStr(args); }
				auto getArgsOptStr() const			{ return Internal::buildCmdStr(argsOpt); }
				auto getArgPacksStr() const			{ return Internal::buildCmdStr(argPacks); }
				auto getFlagsStr() const			{ return Internal::buildCmdStr(flags); }
				auto getFlagValuesStr() const		{ return Internal::buildCmdStr(flagValues); }
				auto getFlagValuesOptStr() const	{ return Internal::buildCmdStr(flagValuesOpt); }
				auto getHelpStr() const
				{
					std::string result;

					if(!desc.empty()) result += ">>" + desc + "\n\n";

					if(!args.empty())			{ result += "\t" "Required arguments:" "\n";	for(const auto& a : args) result += a->getHelpStr(); result += "\n"; }
					if(!argsOpt.empty())		{ result += "\t" "Optional arguments:" "\n";	for(const auto& a : argsOpt) result += a->getHelpStr(); result += "\n"; }
					if(!argPacks.empty())		{ result += "\t" "Argument packs:" "\n";		for(const auto& p : argPacks) result += p->getHelpStr(); result += "\n"; }
					if(!flags.empty())			{ result += "\t" "Flags:" "\n";					for(const auto& f : flags) result += f->getHelpStr(); }
					if(!flagValues.empty())		{ result += "\t" "Flag values:" "\n";			for(const auto& f : flagValues) result += f->getHelpStr(); }
					if(!flagValuesOpt.empty())	{ result += "\t" "Optional flag values:" "\n";	for(const auto& f : flagValuesOpt) result += f->getHelpStr(); }

					return result;
				}

				inline void setDesc(std::string mDesc)		{ desc = std::move(mDesc); }
				inline const auto& getDesc() const noexcept	{ return desc; }
		};
	}
}

#endif
