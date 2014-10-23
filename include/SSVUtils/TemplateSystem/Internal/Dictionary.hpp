
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
				VecMap<std::string, std::string> replacements;
				VecMap<std::string, DictVec> sections;
				Dictionary* parentDict{nullptr};

				template<typename TKey> class Proxy
				{
					private:
						TKey key;
						Dictionary& dict;

					public:
						inline Proxy(TKey mKey, Dictionary& mDict) : key{mKey}, dict{mDict} { }

						template<typename T> inline auto& operator=(T&& mReplacement) { dict.replacements[key] = fwd<T>(mReplacement); return *this; }
						template<typename T> inline auto& operator+=(T&& mDict)	{ dict.sections[key].emplace_back(fwd<T>(mDict)); return *this; }
				};

				// Init single replacement
				template<typename T> inline void initImpl(T&& mKey, const std::string& mReplacement)	{ replacements[fwd<T>(mKey)] = mReplacement; }
				template<typename T> inline void initImpl(T&& mKey, std::string&& mReplacement)			{ replacements[fwd<T>(mKey)] = std::move(mReplacement); }

				// Init section replacement
				template<typename T> inline void initImpl(T&& mKey, const DictVec& mDicts)	{ sections[fwd<T>(mKey)] = mDicts; }
				template<typename T> inline void initImpl(T&& mKey, DictVec&& mDicts)		{ sections[fwd<T>(mKey)] = std::move(mDicts); }

				// Copy/move init
				inline void initImpl(const Dictionary& mDict)
				{
					parentDict = mDict.parentDict;
					replacements = mDict.replacements;
					sections = mDict.sections;
				}
				inline void initImpl(Dictionary&& mDict) noexcept
				{
					parentDict = mDict.parentDict; mDict.parentDict = nullptr;
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

				inline std::string getExpanded(std::string mSrc, Settings mSettings = Settings::EraseUnexisting)
				{
					refreshParents();

					std::string buf, bufKey;
					buf.reserve(static_cast<SizeT>(mSrc.size() * 1.5));
					bufKey.reserve(25);

					bool found{true};
					while(found)
					{
						found = expandImpl(mSrc, buf, bufKey, 0, mSrc.size(), 0, mSettings);

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
