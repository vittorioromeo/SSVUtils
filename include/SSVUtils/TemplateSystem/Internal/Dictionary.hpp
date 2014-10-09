
// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM_DICTIONARY
#define SSVU_TEMPLATESYSTEM_DICTIONARY

namespace ssvu
{
	namespace TemplateSystem
	{
		class Dictionary
		{
			friend class Expander;

			public:
				using DictVec = std::vector<Dictionary>;

			private:
				std::map<std::string, std::string> replacements;
				std::map<std::string, DictVec> sections;
				Dictionary* parentDict{nullptr};

				template<typename TKey> class Proxy
				{
					private:
						TKey key;
						Dictionary& dict;

					public:
						inline Proxy(TKey mKey, Dictionary& mDict) : key{mKey}, dict{mDict} { }

						template<typename T> inline auto& operator=(T&& mReplacement) { dict.replacements[key] = fwd<T>(mReplacement); return *this; }

						inline auto& operator+=(const Dictionary& mDict)	{ dict.sections[key].emplace_back(mDict); return *this; }
						inline auto& operator+=(Dictionary&& mDict)			{ dict.sections[key].emplace_back(std::move(mDict)); return *this; }
				};

				// Init single replacement
				inline void initImpl(const std::string& mKey,	const std::string& mReplacement)	{ replacements[mKey] 			= mReplacement; }
				inline void initImpl(std::string&& mKey,		const std::string& mReplacement)	{ replacements[std::move(mKey)]	= mReplacement; }
				inline void initImpl(const std::string& mKey,	std::string&& mReplacement)			{ replacements[mKey]			= std::move(mReplacement); }
				inline void initImpl(std::string&& mKey,		std::string&& mReplacement)			{ replacements[std::move(mKey)]	= std::move(mReplacement); }

				// Init section replacement
				inline void initImpl(const std::string& mKey,	const DictVec& mDicts)	{ sections[mKey]			= mDicts; }
				inline void initImpl(std::string&& mKey,		const DictVec& mDicts)	{ sections[std::move(mKey)]	= mDicts; }
				inline void initImpl(const std::string& mKey,	DictVec&& mDicts)		{ sections[mKey]			= std::move(mDicts); }
				inline void initImpl(std::string&& mKey,		DictVec&& mDicts)		{ sections[std::move(mKey)]	= std::move(mDicts); }

				// Copy/move init
				inline void initImpl(const Dictionary& mDict)
				{
					parentDict = mDict.parentDict;
					replacements = mDict.replacements;
					sections = mDict.sections;
				}
				inline void initImpl(Dictionary&& mDict) noexcept
				{
					parentDict = mDict.parentDict;
					replacements = std::move(mDict.replacements);
					sections = std::move(mDict.sections);
				}

				inline void init() noexcept { }
				template<typename T1, typename... TArgs> inline void init(T1&& mA1, TArgs&&... mArgs)
				{
					initImpl(fwd<T1>(mA1)); init(fwd<TArgs>(mArgs)...);
				}
				template<typename T1, typename T2, typename... TArgs> inline void init(T1&& mA1, T2&& mA2, TArgs&&... mArgs)
				{
					initImpl(fwd<T1>(mA1), fwd<T2>(mA2)); init(fwd<TArgs>(mArgs)...);
				}

				inline void refreshParents()
				{
					for(auto& v : sections)
						for(auto& d : v.second)
						{
							d.parentDict = this;
							d.refreshParents();
						}
				}

				template<typename... TArgs> bool expandImpl(TArgs&&... mArgs) const
				{
					Expander e(*this, fwd<TArgs>(mArgs)...);
					return e.expand();
				}

			public:
				template<typename... TArgs> inline Dictionary(TArgs&&... mArgs) { init(fwd<TArgs>(mArgs)...); }

				inline std::string getExpanded(std::string mSrc, bool mMaintainNotFound = false)
				{
					refreshParents();

										// corrupted linked list? BUG ?
					std::string buf; // result.reserve(mSrc.size());
					std::string bufKey; bufKey.reserve(25);

					bool found{true};
					while(found)
					{
						found = expandImpl(mSrc, buf, bufKey, 0, mSrc.size(), 0, mMaintainNotFound);

						mSrc = buf;
						buf.clear();
					}

					// De-escape '{'
					if(mSrc.size() > 0)
					{
						for(auto i(0u); i < mSrc.size() - 1; ++i)
						{
							if(mSrc[i] == '\\' && mSrc[i + 1] == '{') ++i;
							buf += mSrc[i];
						}

						buf += mSrc[mSrc.size() - 1];
					}

					return buf;
				}

				template<typename T> inline auto operator[](T&& mKey) noexcept { return Proxy<T>{fwd<T>(mKey), *this}; }
		};
	}
}

#endif
