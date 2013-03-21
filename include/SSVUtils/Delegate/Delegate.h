// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Thanks to sim642 - http://www.facepunch.com/showthread.php?t=1239244&p=39204496&viewfull=1#post39204496

#ifndef SSVU_DELEGATE
#define SSVU_DELEGATE

#include <vector>
#include <functional>

namespace ssvu
{
	template<typename TReturn, typename... TArgs> class Delegate
	{
		private:
			using Func = std::function<TReturn(TArgs...)>;
			std::vector<Func> funcs;

		public:
			template<typename T> Delegate& operator+=(T mFunc) { funcs.push_back(Func(mFunc)); return *this; }
			std::vector<TReturn> operator()(TArgs... mArgs)
			{
				std::vector<TReturn> result;
				for(auto& f : funcs) result.push_back(f(mArgs...));
				return result;
			}
	};

	template<typename... TArgs> class Delegate<void, TArgs...>
	{
		private:
			using Func = std::function<void(TArgs...)>;
			std::vector<Func> funcs;

		public:
			template<typename T> Delegate& operator+=(T mFunc) { funcs.push_back(Func(mFunc)); return *this; }
			void operator()(TArgs... mArgs) { for(auto& f : funcs) f(mArgs...); }
	};
}

#endif 
