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

		class Cmd
		{
			private:
				std::vector<std::string> names;
				std::vector<Uptr<ArgBase>> args;
				std::vector<Uptr<ArgBase>> optArgs;
				std::vector<Uptr<ArgPackBase>> argPacks;
				std::vector<Uptr<Flag>> flags;
				std::function<void()> func;
				std::string desc;

				Flag& findFlag(const std::string& mName);

			public:
				Cmd(const std::initializer_list<std::string>& mNames) : names{mNames} { }

				inline Cmd& operator+=(std::function<void()> mFunc) { func = mFunc; return *this; }
				inline Cmd& operator()() { func(); return *this; }

				template<typename T> inline Arg<T>& createArg()												{ auto result(new Arg<T>()); args.emplace_back(result); return *result; }
				template<typename T> inline OptArg<T>& createOptArg(const T& mDefaultValue)					{ auto result(new OptArg<T>(mDefaultValue)); optArgs.emplace_back(result); return *result; }
				template<typename T> inline ArgPack<T>& createArgPack(unsigned int mMin, unsigned int mMax)	{ auto result(new ArgPack<T>(mMin, mMax)); argPacks.emplace_back(result); return *result; }
				template<typename T> inline ArgPack<T>& createInfiniteArgPack()								{ auto result(new ArgPack<T>); argPacks.emplace_back(result); return *result; }
				Flag& createFlag(const std::string& mShortName, const std::string& mLongName);

				void setArgValue(unsigned int mIndex, const std::string& mValue);
				void setOptArgValue(unsigned int mIndex, const std::string& mValue);
				void setArgPackValues(unsigned int mIndex, const std::vector<std::string>& mValues);

				bool isFlagActive(unsigned int mIndex) const;
				void activateFlag(const std::string& mName);

				inline bool hasName(const std::string& mName) const		{ return contains(names, mName); }
				inline unsigned int getArgCount() const					{ return args.size(); }
				inline unsigned int getOptArgCount() const				{ return optArgs.size(); }
				inline unsigned int getArgPackCount() const				{ return argPacks.size(); }
				inline unsigned int getFlagCount() const				{ return flags.size(); }
				inline const decltype(names)& getNames() const			{ return names; }
				inline const decltype(args)& getArgs() const			{ return args; }
				inline const decltype(optArgs)& getOptArgs() const		{ return optArgs; }
				inline const decltype(argPacks)& getArgPacks() const	{ return argPacks; }
				inline const decltype(flags)& getFlags() const			{ return flags; }

				std::string getNamesStr() const;
				std::string getArgsStr() const;
				std::string getOptArgsStr() const;
				std::string getArgPacksStr() const;
				std::string getFlagsStr() const;
				std::string getHelpStr() const;

				inline void setDesc(const std::string& mDesc)	{ desc = mDesc; }
				inline const std::string& getDesc() const		{ return desc; }
		};
	}
}

#endif
