// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	template<typename> class PolyManager;
	template<typename> class MonoManager;
	template<typename, typename, typename> class BaseManager;

	using Byte = char;

	namespace Internal
	{
		namespace Item
		{
			template<typename T> inline Byte* getAllocated() noexcept
			{
				return reinterpret_cast<Byte*>(std::malloc(sizeof(bool) + sizeof(T)));
			}

			inline void deallocate(Byte* mPtr) noexcept
			{
				SSVU_ASSERT(mBase != nullptr);
				std::free(mPtr);
			}

			template<typename T, typename... TArgs> inline void construct(Byte* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
			{
				SSVU_ASSERT(mBase != nullptr);

				new (mPtr + 0) bool{true};
				new (mPtr + sizeof(bool)) T(std::forward<TArgs>(mArgs)...);
			}

			template<typename TBase> inline void destroy(TBase* mBase) noexcept(noexcept(mBase->TBase::~TBase()))
			{
				SSVU_ASSERT(mBase != nullptr);
				mBase->TBase::~TBase();
			}

			template<typename TBase> inline void setBool(TBase* mBase, bool mBool) noexcept
			{
				*reinterpret_cast<bool*>(reinterpret_cast<Byte*>(mBase) - sizeof(bool)) = mBool;
			}
			template<typename TBase> inline constexpr bool getBool(const TBase* mBase) noexcept
			{
				return *reinterpret_cast<const bool*>(reinterpret_cast<const Byte*>(mBase) - sizeof(bool));
			}

			template<typename TBase> inline constexpr Byte* getByte(TBase* mBase) noexcept { return reinterpret_cast<Byte*>(mBase) - sizeof(bool); }
			template<typename T> inline constexpr T* getBase(Byte* mPtr) noexcept { return reinterpret_cast<T*>(mPtr + sizeof(bool)); }
		};

		class Chunk
		{
			private:
				std::vector<Byte*> ptrs;

			public:
				inline ~Chunk() { for(auto p : ptrs) Item::deallocate(p); }

				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					Byte* result;

					if(ptrs.empty()) result = Item::getAllocated<T>();
					else
					{
						result = ptrs.back();
						ptrs.pop_back();
					}

					Item::construct<T>(result, std::forward<TArgs>(mArgs)...);

					return Item::getBase<T>(result);
				}

				template<typename TBase> inline void recycle(TBase* mBase)
				{
					Item::destroy(mBase);
					ptrs.emplace_back(Item::getByte(mBase));
				}
		};

		template<typename TBase> class ChunkDeleter
		{
			private:
				Chunk* chunk;

			public:
				inline ChunkDeleter(Chunk& mChunk) noexcept : chunk{&mChunk} { }
				inline void operator()(TBase* mPtr) const { chunk->recycle<TBase>(mPtr); }
		};

		class PolyStorage
		{
			private:
				std::map<std::size_t, Chunk> chunks;

			public:
				template<typename T> inline Chunk& getChunk() { return chunks[sizeof(T)]; }
				template<typename T, typename... TArgs> inline T* create(Chunk& mChunk, TArgs&&... mArgs)
				{
					return mChunk.create<T>(std::forward<TArgs>(mArgs)...);
				}
		};

		struct MonoStorage { Chunk chunk; };

		template<typename TBase> using PolyManagerBase = BaseManager<TBase, PolyStorage, PolyManager<TBase>>;
		template<typename TBase> using MonoManagerBase = BaseManager<TBase, MonoStorage, MonoManager<TBase>>;
	}

	template<typename TBase, typename TStorage, typename TDerived> class BaseManager
	{
		template<typename T1, typename T2> friend void ssvu::eraseRemoveIf(T1&, const T2&);

		protected:
			using ChunkType = Internal::Chunk;
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;
			using PtrType = Uptr<TBase, ChunkDeleterType>;
			using Container = std::vector<PtrType>;
			TStorage storage;
			Container items, toAdd;

		public:
			template<typename T = TBase, typename... TArgs> inline T& create(TArgs&&... mArgs)
			{
				return reinterpret_cast<TDerived*>(this)->template createImpl<T>(std::forward<TArgs>(mArgs)...);
			}

			inline void clear()	noexcept { items.clear(); toAdd.clear(); }
			inline void del(TBase& mBase) noexcept { Internal::Item::setBool(&mBase, false); }

			inline void refresh()
			{
				for(auto& i : this->toAdd) this->items.emplace_back(std::move(i)); this->toAdd.clear();
				eraseRemoveIf(this->items, [](const PtrType& mP){ return TDerived::isDead(mP.get()); });
			}

			inline static bool isAlive(const TBase* mBase) noexcept	{ return Internal::Item::getBool(mBase); }
			inline static bool isDead(const TBase* mBase) noexcept	{ return !isAlive(mBase); }

			//inline static bool isAlive(const TBase& mItem) noexcept	{ return isAlive(&mItem); }
			//inline static bool isDead(const TBase& mItem) noexcept	{ return isDead(&mItem); }

			inline auto size()		const noexcept	-> decltype(items.size())		{ return items.size(); }
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

	template<typename TBase> class PolyManager : public Internal::PolyManagerBase<TBase>
	{
		friend Internal::PolyManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				auto& chunk(this->storage.template getChunk<T>());
				auto result(chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				this->toAdd.emplace_back(result, ChunkDeleterType{chunk});
				return *result;
			}
	 };

	template<typename TBase> class MonoManager : public Internal::MonoManagerBase<TBase>
	{
//		SSVU_ASSERT_STATIC(!isPolymorphic<T>(), "T must not be polymorphic");
		friend Internal::MonoManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				auto result(this->storage.chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				this->toAdd.emplace_back(result, ChunkDeleterType{this->storage.chunk});
				return *result;
			}
	 };
}

SSVUT_TEST(PtrTests)
{
	struct KTest
	{
		double d;
		int i;
		float f;

		KTest(double mD, int mI, float mF) : d{mD}, i{mI}, f{mF} { }
	};

	ssvu::Byte* ptr;
	ptr = reinterpret_cast<ssvu::Byte*>(std::malloc(sizeof(bool) + sizeof(KTest)));

	new (ptr + 0) bool{true};
	new (ptr + sizeof(bool)) KTest{22.5, 5, 11.f};

	SSVUT_EXPECT(*reinterpret_cast<bool*>(ptr) == true);
	SSVUT_EXPECT(reinterpret_cast<KTest*>(ptr + sizeof(bool))->d == 22.5);
	SSVUT_EXPECT(reinterpret_cast<KTest*>(ptr + sizeof(bool))->i == 5);
	SSVUT_EXPECT(reinterpret_cast<KTest*>(ptr + sizeof(bool))->f == 11.f);

	*reinterpret_cast<bool*>(ptr) = false;
	SSVUT_EXPECT(*reinterpret_cast<bool*>(ptr) == false);

	*reinterpret_cast<bool*>(ptr) = true;
	SSVUT_EXPECT(*reinterpret_cast<bool*>(ptr) == true);

	auto kkc = [](const KTest* mP){ return *reinterpret_cast<const bool*>(reinterpret_cast<const ssvu::Byte*>(mP) - sizeof(bool)); };
	auto kk = [](KTest* mP, bool mB){ *reinterpret_cast<bool*>(reinterpret_cast<ssvu::Byte*>(mP) - sizeof(bool)) = mB; };

	KTest* kptr = reinterpret_cast<KTest*>(ptr + sizeof(bool));

	SSVUT_EXPECT(kkc(kptr) == true);
	kk(kptr, false);
	SSVUT_EXPECT(kkc(kptr) == false);
	kk(kptr, true);
	SSVUT_EXPECT(kkc(kptr) == true);
}


#endif
