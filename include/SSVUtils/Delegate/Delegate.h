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
	/*!
	 *
	 * @brief C#-like delegate class (returns a std::vector)
	 *
	 * A collection of functions with the same signature that can be dynamically changed.
	 *
	 * @tparam TReturn Return type.
	 * @tparam TArgs Function signature variadic arguments.
	 *
	 */
	template<typename TReturn, typename... TArgs> class Delegate
	{
		private:
			using Func = std::function<TReturn(TArgs...)>;
			std::vector<Func> funcs; /*!< Internal collection of functions. */

		public:
			/*!
			 *
			 * @brief Add a function to the delegate
			 *
			 * Operator+= adds a function the delegate's internal function list which will be called using operator().
			 *
			 * @code
			 * ssvu::Delegate<int, int> delegate;
			 * delegate += [](int mParameter){ return mParameter * 2; };
			 * @endcode
			 *
			 * @param mFunc Function to add - can be a std::function or a lambda
			 *
			 */
			template<typename T> Delegate& operator+=(T mFunc) { funcs.push_back(Func(mFunc)); return *this; }

			/*!
			 *
			 * @brief Call all the functions in the delegate.
			 *
			 * @code
			 * ssvu::Delegate<int> delegate;
			 * delegate += []{ return 0; };
			 * delegate += []{ return 10; };
			 * delegate += []{ return 20; };
			 * auto result{delegate()};
			 *
			 * assert(result[0] == 0);
			 * assert(result[1] == 10);
			 * assert(result[2] == 20);
			 * @endcode
			 *
			 * @param mArgs Arguments passed to every function
			 *
			 * @return std::vector containing return values from the functions
			 *
			 */
			std::vector<TReturn> operator()(TArgs... mArgs)
			{
				std::vector<TReturn> result;
				for(const auto& f : funcs) result.push_back(f(mArgs...));
				return result;
			}
	};

	/*!
	 *
	 * @brief C#-like delegate class (returns void)
	 *
	 * A collection of functions with the same signature that can be dynamically changed.
	 *
	 * @tparam TArgs Function signature variadic arguments.
	 *
	 */
	template<typename... TArgs> class Delegate<void, TArgs...>
	{
		private:
			using Func = std::function<void(TArgs...)>;
			std::vector<Func> funcs; /*!< Internal collection of functions. */

		public:
			/*!
			 *
			 * @brief Add a function to the delegate.
			 *
			 * Operator+= adds a function the delegate's internal function list which will be called using operator().
			 *
			 * @code
			 * ssvu::Delegate<void, int> delegate;
			 * delegate += [](int mParameter){ std::cout << mParameter; };
			 * @endcode
			 *
			 * @param mFunc Function to add - can be a std::function or a lambda
			 *
			 */
			template<typename T> Delegate& operator+=(T mFunc) { funcs.push_back(Func(mFunc)); return *this; }

			/*!
			 *
			 * @brief Call all the functions in the delegate.
			 *
			 * @code
			 * int result{0};
			 * ssvu::Delegate<void, int> delegate;
			 * delegate += [&](int mParameter){ ++result; };
			 * delegate += [&](int mParameter){ ++result; };
			 * delegate += [&](int mParameter){ ++result; };
			 * delegate();
			 *
			 * assert(result == 3);
			 * @endcode
			 *
			 * @param mArgs Arguments passed to every function
			 *
			 */
			void operator()(TArgs... mArgs) { for(const auto& f : funcs) f(mArgs...); }
	};
}

#endif
