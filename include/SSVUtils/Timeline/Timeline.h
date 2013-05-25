// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE
#define SSVU_TIMELINE

#include <vector>
#include <functional>

namespace ssvu
{
	typedef std::function<void()> Action;

    struct Command;

	class Timeline
	{
		friend class Wait;
		friend class Do;
		friend class Go;

		private:
			std::vector<Command*> commandPtrs; // owned
			Command* currentCommandPtr{nullptr};
			bool ready{true}, finished{false};

			void append(Command* mCommandPtr);
			void insert(int mIndex, Command* mCommandPtr);
			void next();

		public:
			~Timeline();

			template<typename T, typename... TArgs> T* append(TArgs&&... mArgs)
			{
				T* result{new T(*this, std::forward<TArgs>(mArgs)...)};
				append(result); return result;
			}
			template<typename T, typename... TArgs> T* insert(int mIndex, TArgs&&... mArgs)
			{
				T* result{new T(*this, std::forward<TArgs>(mArgs)...)};
				insert(mIndex, result); return result;
			}

			void del(Command* mCommandPtr);

			void update(float mFrameTime);
			void jumpTo(int mIndex);

			void reset();
			void clear();
			void start();

			int getSize();
			int getCurrentIndex();
			bool isFinished();
	};
}

#endif
