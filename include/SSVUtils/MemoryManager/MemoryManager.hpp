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

	namespace Internal
	{
		namespace Item
		{
			template<typename T> struct Layout
			{
				AlignedStorageBasic<bool> storageBool;
				AlignedStorageBasic<T> storageItem;
			};

			template<typename T> inline char* allocate()
			{
				return reinterpret_cast<char*>(std::allocator<Layout<T>>{}.allocate(1));
			}
			inline void deallocate(char* mPtr) noexcept { SSVU_ASSERT(mPtr != nullptr); delete mPtr; }

			template<typename T> inline constexpr char* getBoolAddress(char* mPtr) noexcept { return reinterpret_cast<char*>(&reinterpret_cast<Layout<T>*>(mPtr)->storageBool); }
			template<typename T> inline constexpr char* getItemAddress(char* mPtr) noexcept { return reinterpret_cast<char*>(&reinterpret_cast<Layout<T>*>(mPtr)->storageItem); }

			template<typename T, typename... TArgs> inline void construct(char* mPtr, TArgs&&... mArgs) noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
			{
				SSVU_ASSERT(mPtr != nullptr);

				new (getBoolAddress<T>(mPtr)) bool{true};
				new (getItemAddress<T>(mPtr)) T(std::forward<TArgs>(mArgs)...);
			}

			template<typename TBase> inline void destroy(TBase* mBase) noexcept(noexcept(mBase->TBase::~TBase()))
			{
				SSVU_ASSERT(mBase != nullptr);
				mBase->TBase::~TBase();
			}


			template<typename TBase> inline constexpr Layout<TBase>* getLayout(TBase* mBase) noexcept				{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR(Layout<TBase>, mBase, storageItem); }
			template<typename TBase> inline constexpr const Layout<TBase>* getLayout(const TBase* mBase) noexcept	{ return SSVU_GET_BASEPTR_FROM_MEMBERPTR_CONST(Layout<TBase>, mBase, storageItem); }

			template<typename TBase> inline constexpr char* getByte(TBase* mBase) noexcept	{ return reinterpret_cast<char*>(getLayout<TBase>(mBase)); }
			template<typename T> inline constexpr T* getBase(char* mPtr) noexcept			{ return reinterpret_cast<T*>(&reinterpret_cast<Layout<T>*>(mPtr)->storageItem); }

			template<typename TBase> inline void setBool(TBase* mBase, bool mBool) noexcept		{ *reinterpret_cast<bool*>(&getLayout(mBase)->storageBool) = mBool; }
			template<typename TBase> inline constexpr bool getBool(const TBase* mBase) noexcept	{ return *reinterpret_cast<const bool*>(&getLayout(mBase)->storageBool); }
		}

		struct Chain
		{
			Chain* next;
		};

		class Chunk
		{
			private:
				Chain* chain{nullptr};

			public:
				inline ~Chunk()
				{
					Chain* temp;
					while(chain != nullptr)
					{
						temp = chain;
						chain = chain->next;
						Item::deallocate(reinterpret_cast<char*>(temp));
					}
				}

				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					char* result;

					if(chain == nullptr) result = Item::allocate<T>();
					else
					{
						result = reinterpret_cast<char*>(chain);
						chain = chain->next;
					}

					Item::construct<T>(result, std::forward<TArgs>(mArgs)...);

					return Item::getBase<T>(result);
				}

				template<typename TBase> inline void recycle(TBase* mBase)
				{
					Item::destroy(mBase);

					auto newHead(reinterpret_cast<Chain*>(Item::getByte(mBase)));
					newHead->next = chain;
					chain = newHead;
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

		struct MonoStorage
		{
			Chunk chunk;
		};

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
				SSVU_ASSERT_STATIC(sizeof(TBase) >= sizeof(char*), "sizeof(TBase) must be >= sizeof(char*)");

				return reinterpret_cast<TDerived*>(this)->template createImpl<T>(std::forward<TArgs>(mArgs)...);
			}

			inline void clear()	noexcept { items.clear(); toAdd.clear(); }
			inline void del(TBase& mBase) noexcept { Internal::Item::setBool(&mBase, false); }

			inline void reserve(std::size_t mCapacity)
			{
				SSVU_ASSERT(items.capacity() < mCapacity);
				items.reserve(mCapacity);
				toAdd.reserve(mCapacity);
			}

			inline void refresh()
			{
				std::size_t i{0u}, k{0u};

				// While there currently are items in the main container...
				for(; i < items.size(); ++i)
				{
					// Skip alive items
					if(TDerived::isAlive(items[i].get())) continue;

					// Found a dead item - if there are items to add, replace it with one of them...
					if(k < toAdd.size())
					{
						items[i] = std::move(toAdd[k++]);
						continue;
					}

					// ...otherwise, erase-remove-if all dead items from this point forward
					items.erase(std::remove_if(std::begin(items) + i, std::end(items), [](const PtrType& mP){ return TDerived::isDead(mP.get()); }), std::end(items));
					break;
				}

				// Emplace_back remaining items in the main container
				while(k < toAdd.size()) items.emplace_back(std::move(toAdd[k++]));

				// Clear now-unused pointers
				toAdd.clear();
			}

			inline static bool isAlive(const TBase* mBase) noexcept	{ return Internal::Item::getBool(mBase); }
			inline static bool isDead(const TBase* mBase) noexcept	{ return !isAlive(mBase); }

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

	template<typename TBase> class PolyManager final : public Internal::PolyManagerBase<TBase>
	{
		friend Internal::PolyManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSame<TBase, T>() || isBaseOf<TBase, T>(), "PolyManager can only store types that belong to the same hierarchy");

				auto& chunk(this->storage.template getChunk<T>());
				auto result(chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				this->toAdd.emplace_back(result, ChunkDeleterType{chunk});
				return *result;
			}
	 };

	template<typename TBase> class MonoManager final : public Internal::MonoManagerBase<TBase>
	{
		friend Internal::MonoManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSame<TBase, T>(), "MonoManager can only store objects of the same type");

				auto result(this->storage.chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				this->toAdd.emplace_back(result, ChunkDeleterType{this->storage.chunk});
				return *result;
			}
	 };
}

/*
SSVUT_TEST(PtrTests)
{
	struct KTest
	{
		double d;
		int i;
		float f;

		KTest(double mD, int mI, float mF) : d{mD}, i{mI}, f{mF} { }
	};

	char* ptr;
	ptr = reinterpret_cast<char*>(std::malloc(sizeof(bool) + sizeof(KTest)));

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

	auto kkc = [](const KTest* mP){ return *reinterpret_cast<const bool*>(reinterpret_cast<const char*>(mP) - sizeof(bool)); };
	auto kk = [](KTest* mP, bool mB){ *reinterpret_cast<bool*>(reinterpret_cast<char*>(mP) - sizeof(bool)) = mB; };

	KTest* kptr = reinterpret_cast<KTest*>(ptr + sizeof(bool));

	SSVUT_EXPECT(kkc(kptr) == true);
	kk(kptr, false);
	SSVUT_EXPECT(kkc(kptr) == false);
	kk(kptr, true);
	SSVUT_EXPECT(kkc(kptr) == true);
}
*/

#endif
