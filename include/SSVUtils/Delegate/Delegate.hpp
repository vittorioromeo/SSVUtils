// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// Thanks to sim642 - http://www.facepunch.com/showthread.php?t=1239244&p=39204496&viewfull=1#post39204496

#ifndef SSVU_DELEGATE
#define SSVU_DELEGATE

#include <SSVUtils/Core/Core.hpp>

namespace ssvu
{
	template<typename> class Delegate;
	namespace Internal { template<typename, typename...> struct DelegateHelper; }

	/// @brief C#-like delegate class.
	/// @details A collection of functions with the same signature that can be dynamically changed.
	/// @tparam TReturn Return type.
	/// @tparam TArgs Function signature variadic arguments.
	template<typename TReturn, typename... TArgs> class Delegate<TReturn(TArgs...)>
	{
		template<typename, typename...> friend struct Internal::DelegateHelper;

		private:
			using FuncType = Func<TReturn(TArgs...)>;
			std::vector<FuncType> funcs; /*!< Internal collection of functions. */

		public:
			/// @brief Add a function to the delegate
			/// @details Operator+= adds a function the delegate's internal function list which will be called using operator().
			/// @code
			/// ssvu::Delegate<int(int)> delegate;
			/// delegate += [](int mParameter){ return mParameter * 2; };
			/// @endcode
			/// @param mFunc Function to add - can be a std::function or a lambda
			template<typename T> inline auto& operator+=(const T& mFunc) { funcs.emplace_back(mFunc); return *this; }

			/// @brief Call all the functions in the delegate.
			/// @code
			/// ssvu::Delegate<int()> delegate;
			/// delegate += []{ return 0; };
			/// delegate += []{ return 10; };
			/// delegate += []{ return 20; };
			/// auto result{delegate()};
			///
			/// SSVU_ASSERT(result[0] == 0);
			/// SSVU_ASSERT(result[1] == 10);
			/// SSVU_ASSERT(result[2] == 20);
			/// @endcode
			/// @param mArgs Arguments passed to every function
			/// @return void or std::vector containing return values from the functions
			inline auto operator()(TArgs... mArgs)
			{
				return Internal::DelegateHelper<TReturn, TArgs...>::exec(*this, mArgs...);
			}

			/// @brief Clears all the functions from the delegate.
			inline void clear() noexcept { funcs.clear(); }

			/// @brief Returns whether the delegate has any callback or not.
			inline bool isEmpty() const noexcept { return funcs.empty(); }
	};

	namespace Internal
	{
		template<typename TReturn, typename... TArgs> struct DelegateHelper
		{
			inline static auto exec(Delegate<TReturn(TArgs...)>& mDelegate, TArgs... mArgs)
			{
				std::vector<TReturn> result; result.reserve(mDelegate.funcs.size());
				for(const auto& f : mDelegate.funcs) result.emplace_back(f(mArgs...));
				return result;
			}
		};
		template<typename... TArgs> struct DelegateHelper<void, TArgs...>
		{
			inline static void exec(Delegate<void(TArgs...)>& mDelegate, TArgs... mArgs)
			{
				for(const auto& f : mDelegate.funcs) f(mArgs...);
			}
		};
	}
}

#endif

// TODO: new delegate class with deletion
