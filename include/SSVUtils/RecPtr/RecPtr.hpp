// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RECPTR
#define SSVU_RECPTR

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<typename T> class Recycler
		{
			private:
				std::vector<T*> ptrs;
				std::allocator<T> alloc;

				inline T* pop() noexcept
				{
					SSVU_ASSERT(!ptrs.empty());

					auto result(ptrs.back());
					ptrs.pop_back();
					return reinterpret_cast<T*>(result);
				}

			public:
				inline Recycler() = default;
				inline ~Recycler() noexcept { for(auto p : ptrs) alloc.deallocate(reinterpret_cast<T*>(p), 1); }

				template<typename TT> inline void recycle(TT* mPtr) noexcept(noexcept(alloc.destroy(mPtr)) && noexcept(ptrs.emplace_back(mPtr)))
				{
					SSVU_ASSERT(mPtr != nullptr);
					SSVU_ASSERT_STATIC(isBaseOf<TT, T>() || isSame<TT, T>(), "TT must be the same as T or its base type");

					alloc.destroy(reinterpret_cast<T*>(mPtr));
					ptrs.emplace_back(reinterpret_cast<T*>(mPtr));
				}

				template<typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					auto result(ptrs.empty() ? alloc.allocate(1) : pop());
					alloc.construct(result, std::forward<TArgs>(mArgs)...);
					return result;
				}
		};

		template<typename T> inline Recycler<T>& getRecycler() noexcept
		{
			static Recycler<T> result; return result;
		}
	}

	template<typename T, typename TBase> using UptrRecPoly = Uptr<T, void(*)(TBase*)>;
	template<typename T> using UptrRec = UptrRecPoly<T, T>;
	template<typename TBase> using VecUptrRec = std::vector<UptrRec<TBase>>;

	template<typename T, typename TBase, typename... TArgs> inline UptrRecPoly<T, TBase> makeUptrRecPoly(TArgs&&... mArgs)
	{
		return {Internal::getRecycler<T>().create(std::forward<TArgs>(mArgs)...), [](TBase* mPtr){ Internal::getRecycler<T>().recycle(mPtr); }};
	}
	template<typename T, typename... TArgs> inline UptrRec<T> makeUptrRec(TArgs&&... mArgs) { return makeUptrRecPoly<T, T>(std::forward<TArgs>(mArgs)...); }

	namespace Internal
	{
		template<typename T, typename TBase> struct MakerUptrRecPoly
		{
			template<typename... TArgs> static inline UptrRecPoly<T, TBase> make(TArgs&&... mArgs)
			{
				return makeUptrRecPoly<T, TBase, TArgs...>(std::forward<TArgs>(mArgs)...);
			}
		};
	}

	template<typename T, typename TBase, typename... TArgs, typename TC> inline T& getEmplaceUptrRecPoly(TC& mContainer, TArgs&&... mArgs)
	{
		return Internal::getEmplaceUptrImpl<T, TC, Internal::MakerUptrRecPoly<T, TBase>>(mContainer, std::forward<TArgs>(mArgs)...);
	}
	template<typename T, typename... TArgs, typename TC> inline T& getEmplaceUptrRec(TC& mContainer, TArgs&&... mArgs)
	{
		return getEmplaceUptrRecPoly<T, T>(mContainer, std::forward<TArgs>(mArgs)...);
	}
}

namespace ssvu
{
//	struct MemoryManageable { bool ssvu_mmAlive{true}; };


	template<typename TBase> class PolyManager
	{
		template<typename T1, typename T2> friend void ssvu::eraseRemoveIf(T1&, const T2&);

		private:
			class PolyManagerStorage
			{
				private:
					class Chunk
					{
						private:
							std::vector<TBase*> ptrs;
							std::size_t allocSize;

						public:
							inline Chunk(std::size_t mAllocSize) noexcept : allocSize{mAllocSize} { }
							inline ~Chunk() { for(auto p : ptrs) delete p; }

							inline void push(TBase* mPtr) noexcept(noexcept(ptrs.emplace_back(mPtr))) { ptrs.emplace_back(mPtr); }
							inline TBase* pop() noexcept(noexcept(ptrs.pop_back()))
							{
								auto result(ptrs.back());
								ptrs.pop_back();
								return result;
							}

							inline std::size_t getAllocSize() const noexcept { return allocSize; }
							inline bool isEmpty() const noexcept { return ptrs.empty(); }
					};

					std::vector<Chunk> chunks;

					inline Chunk& getChunkBySize(std::size_t mSize)
					{
						auto i(0u);
						for(; i < chunks.size(); ++i)
						{
							if(chunks[i].getAllocSize() > mSize) break;
							if(chunks[i].getAllocSize() == mSize) return chunks[i];
						}

						chunks.emplace(std::begin(chunks) + i, mSize);
						return chunks[i];
					}

				public:
					template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
					{
						std::allocator<T> alloc;
						auto& chunk(getChunkBySize(sizeof(T)));

						auto result(chunk.isEmpty() ? alloc.allocate(1) : chunk.pop());
						alloc.construct(result, std::forward<TArgs>(mArgs)...);

						return reinterpret_cast<T*>(result);
					}

					inline void recycle(TBase* mPtr, std::size_t mS)
					{
						auto& chunk(getChunkBySize(mS));
						mPtr->TBase::~TBase();
						chunk.push(mPtr);
					}
			};

			PolyManagerStorage pms;

			struct RType
			{
				PolyManager* pm;
				std::size_t s;
				inline RType(PolyManager* mPm, std::size_t mS) noexcept : pm(mPm), s(mS) { }
				inline void operator()(TBase* mPtr) const { pm->pms.recycle(mPtr, s); }
			};

			using PtrType = Uptr<TBase, RType>;
			using Container = std::vector<PtrType>;
			Container items, toAdd;

		public:
			inline void clear()	noexcept { items.clear(); toAdd.clear(); }
			inline void del(TBase& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

			inline void refresh()
			{
				for(auto& i : this->toAdd) this->items.emplace_back(std::move(i)); this->toAdd.clear();
				eraseRemoveIf(this->items, this->template isDead<PtrType>);
			}
			template<typename T = TBase, typename... TArgs> inline T& create(TArgs&&... mArgs)
			{
				auto result(pms.template create<T>(std::forward<TArgs>(mArgs)...));
				toAdd.emplace_back(result, RType{this, sizeof(T)});
				return *reinterpret_cast<T*>(result);
			}

			template<typename T> inline static bool isAlive(const T& mItem) noexcept	{ return mItem->ssvu_mmAlive; }
			template<typename T> inline static bool isDead(const T& mItem) noexcept		{ return !isAlive(mItem); }

			inline auto size() const noexcept -> decltype(items.size()) { return items.size(); }

			// Standard iterator support
			inline auto begin()		noexcept		-> decltype(items.begin())		{ return items.begin(); }
			inline auto end()		noexcept		-> decltype(items.end())		{ return items.end(); }
			inline auto begin()		const noexcept	-> decltype(items.begin())		{ return items.begin(); }
			inline auto end()		const noexcept	-> decltype(items.end())		{ return items.end(); }
			inline auto cbegin()	const noexcept	-> decltype(items.cbegin())		{ return items.cbegin(); }
			inline auto cend()		const noexcept	-> decltype(items.cend())		{ return items.cend(); }
			inline auto rbegin()	noexcept		-> decltype(items.rbegin())		{ return items.rbegin(); }
			inline auto rend()		noexcept		-> decltype(items.rend())		{ return items.rend(); }
			inline auto crbegin()	const noexcept	-> decltype(items.crbegin())	{ return items.crbegin(); }
			inline auto crend()		const noexcept	-> decltype(items.crend())		{ return items.crend(); }
	 };
}

#endif

// TODO: docs, test, sptr
