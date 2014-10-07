
// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEMPLATESYSTEM_EXPANDER
#define SSVU_TEMPLATESYSTEM_EXPANDER

namespace ssvu
{
	namespace TemplateSystem
	{
		class Dictionary;

		class Expander
		{
			private:
				const Dictionary& dict;
				const std::string& src;
				std::string& bufResult;
				std::string& bufKey;
				const std::size_t idxBegin, idxEnd;
				std::size_t idx;
				bool separate;

				inline auto getC() const noexcept 					{ SSVU_ASSERT(idx >= 0 && idx < src.size()); return src[idx]; }
				inline auto getC(std::size_t mIdx) const noexcept 	{ SSVU_ASSERT(mIdx >= 0 && mIdx < src.size()); return src[mIdx]; }

			public:
				inline Expander(const Dictionary& mDict, const std::string& mSrc, std::string& mBufResult, std::string& mBufKey, std::size_t mIdxBegin, std::size_t mIdxEnd, bool mSeparate)
					: dict{mDict}, src{mSrc}, bufResult{mBufResult}, bufKey{mBufKey}, idxBegin{mIdxBegin}, idxEnd{mIdxEnd}, idx{idxBegin}, separate{mSeparate} { }

				void expand();
		};
	}
}

#endif
