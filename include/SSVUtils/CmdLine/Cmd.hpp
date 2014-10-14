// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_CMD
#define SSVU_CMDLINE_CMD

namespace ssvu
{
	namespace CmdLine
	{
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

			class ManagerElements
			{
				private:
					static constexpr SizeT maxTypes{50};
					PolyFixedRecycler<BaseElement, maxTypes> recycler;
					std::vector<decltype(recycler)::PtrType> elements;
					std::array<std::vector<BaseElement*>, maxTypes> groupedElements;

					template<EType TET> inline auto& getGroupVec() noexcept				{ return groupedElements[static_cast<SizeT>(TET)]; }
					template<EType TET> inline const auto& getGroupVec() const noexcept	{ return groupedElements[static_cast<SizeT>(TET)]; }

				public:
					template<typename T, typename... TArgs> inline T& create(TArgs&&... mArgs)
					{
						SSVU_ASSERT_STATIC(isBaseOf<BaseElement, T>(), "`T` must derive from `BaseElement`");

						auto& result(recycler.getCreateEmplace<T>(elements, fwd<TArgs>(mArgs)...));
						getGroupVec<T::getEType()>().emplace_back(&result);
						return result;
					}

					template<EType TET> inline auto getCount() const noexcept	{ return getGroupVec<TET>().size(); }
					template<EType TET> inline auto isEmpty() const noexcept	{ return getCount<TET>() == 0; }

					template<EType TET> inline auto& getAll() noexcept				{ return getGroupVec<TET>(); }
					template<EType TET> inline const auto& getAll() const noexcept	{ return getGroupVec<TET>(); }

					template<EType TET> inline auto& getAt(SizeT mIdx) noexcept				{ return castUp<ETypeBase<TET>>(*getAll<TET>()[mIdx]); }
					template<EType TET> inline const auto& getAt(SizeT mIdx) const noexcept	{ return castUp<ETypeBase<TET>>(*getAll<TET>()[mIdx]); }
			};
		}

		class Cmd
		{
			friend class Ctx;

			private:
				std::vector<std::string> names;
				Internal::ManagerElements mgr;
				Delegate<void()> onAction;
				std::string desc;
				bool mainCmd{false}; // Is this the main cmd?

				inline Cmd() = default;
				inline static Cmd createCmdMain() { Cmd result; result.mainCmd = true; return result; }

				inline auto& findFlag(const std::string& mName)
				{
					for(auto& f : asRangeCastPtr<Flag&>(mgr.getAll<EType::Flag>())) if(f.hasName(mName)) return f;
					throw Exception::createFlagNotFound(mName, getNamesStr());
				}

				template<EType TET> inline void setElementValue(SizeT mIdx, const std::string& mValue) { mgr.getAt<TET>(mIdx).set(mValue); }

			public:
				inline Cmd(const std::initializer_list<std::string>& mNames) : names{mNames} { }

				inline Cmd& operator+=(const Action& mFunc) { onAction += mFunc; return *this; }
				inline Cmd& operator()() { onAction(); return *this; }

				template<typename T, typename... TArgs> inline T& create(TArgs&&... mArgs) { return mgr.create<T>(fwd<TArgs>(mArgs)...); }

				inline void activateFlag(const std::string& mName) { findFlag(mName) = true; }

				inline bool hasName(const std::string& mName) const	{ return contains(names, mName); }
				inline bool isMainCmd() const noexcept				{ return mainCmd; }

				inline const auto& getNames() const noexcept { return names; }

				template<EType TET> inline auto getCount() const noexcept		{ return mgr.getCount<TET>(); }
				template<EType TET> inline const auto& getAll() const noexcept	{ return mgr.getAll<TET>(); }

				template<EType TET> inline auto& getAt(SizeT mIdx) noexcept				{ return mgr.getAt<TET>(mIdx); }
				template<EType TET> inline const auto& getAt(SizeT mIdx) const noexcept	{ return mgr.getAt<TET>(mIdx); }

				std::string getNamesStr() const			{ return Internal::buildCmdStr(names, "<", ">", " || "); }
				template<EType TET> auto getStr() const	{ return Internal::buildCmdStr(getAll<TET>()); }

				auto getHelpStr() const
				{
					std::string result;

					if(!desc.empty()) result += ">>" + desc + "\n\n";

					if(!mgr.isEmpty<EType::Arg>())			{ result += "\tRequired arguments:\n";		for(auto p : getAll<EType::Arg>())			result += p->getHelpStr();		result += "\n"; }
					if(!mgr.isEmpty<EType::ArgOpt>())		{ result += "\tOptional arguments:\n";		for(auto p : getAll<EType::ArgOpt>())		result += p->getHelpStr();		result += "\n"; }
					if(!mgr.isEmpty<EType::ArgPack>())		{ result += "\tArgument packs:\n";			for(auto p : getAll<EType::ArgPack>())		result += p->getHelpStr();		result += "\n"; }
					if(!mgr.isEmpty<EType::Flag>())			{ result += "\tFlags:\n";					for(auto p : getAll<EType::Flag>())			result += p->getHelpStr();		result += "\n"; }
					if(!mgr.isEmpty<EType::FlagValue>())	{ result += "\tFlag values:\n";				for(auto p : getAll<EType::FlagValue>())	result += p->getHelpStr();		result += "\n"; }
					if(!mgr.isEmpty<EType::FlagValueOpt>())	{ result += "\tOptional flag values:\n";	for(auto p : getAll<EType::FlagValueOpt>())	result += p->getHelpStr();						}

					return result;
				}

				SSVU_DEFINE_SINK_SETTER_SIMPLE(setDesc, desc);
				inline const auto& getDesc() const noexcept { return desc; }
		};
	}
}

#endif
