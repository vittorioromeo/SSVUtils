// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TIMELINE
#define SSVU_IMPL_TIMELINE

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"
#include "SSVUtils/Timeline/Inc/Command.hpp"

namespace ssvu
{
	namespace Impl
	{
		template<bool> class WaitLoop;
		template<typename> class GoImpl;
	}

	class Timeline
	{
		friend class Do;
		friend class Wait;
		template<bool> friend class Impl::WaitLoop;
		template<typename> friend class Impl::GoImpl;

		public:
			using Idx = int;
			static constexpr Idx nullIdx{-1};

		private:
			PolyRecVector<Command> commands;
			Idx cIdx{nullIdx};
			FT remainder{0.f};
			bool ready{true}, finished{false};

			bool isCIdxNull() const noexcept;

			template<typename T, typename... TArgs> inline Idx insertImpl(Idx mIdx, TArgs&&... mArgs)
			{
				commands.createAt<T>(mIdx, *this, FWD(mArgs)...);

				// If the current command is null, set it to the newly added one
				if(isCIdxNull()) cIdx = mIdx;

				return mIdx;
			}

			void next();

		public:
			Timeline(bool mStart = true) noexcept;

			template<typename T, typename... TArgs> inline auto append(TArgs&&... mArgs)			{ return insertImpl<T>(commands.size(), FWD(mArgs)...); }
			template<typename T, typename... TArgs> inline auto insert(Idx mIdx, TArgs&&... mArgs)	{ return insertImpl<T>(mIdx, FWD(mArgs)...); }

			void jumpTo(Idx mIdx) noexcept;
			void start() noexcept;
			void clear() noexcept;
			void stop() noexcept;

			void update(FT mFT);
			void reset();

			auto getSize() const noexcept;
			bool isFinished() const noexcept;
			int getCurrentIndex() const;
	};
}

#endif
