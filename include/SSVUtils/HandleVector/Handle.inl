// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_HANDLE_INL
#define SSVU_HANDLEVECTOR_HANDLE_INL

namespace ssvu
{
	template<typename T> inline bool Handle<T>::isAlive() const noexcept
	{
		return hVec->marks[markIdx].ctr == ctr;
	}

	template<typename T> inline void Handle<T>::destroy() noexcept
	{
		hVec->destroy(markIdx);
	}
}

#endif
