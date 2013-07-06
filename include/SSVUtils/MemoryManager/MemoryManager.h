// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/MemoryManager/Traits/Traits.h"

namespace ssvu
{
	/*!
	 *
	 * @brief Heap memory manager with cleanup features.
	 *
	 * This class allocates/deallocates objects on the heap. It uses a container to store objects, and a container to store objects that are "dead".
	 * Calling create(...) returns a reference to a new object. Calling del(...) sets an object as "dead", but doesn't actually deallocate it.
	 * Calling createDelayed(...) allocates a new object, but doesn't put it in the main container until a `cleanUp()` call.
	 * Calling cleanUp() deallocates all the "dead" objects and removes them from the main container, and also adds all the "to add" objects to the main container.
	 * Can be useful to manage game entities, timelines, etc...
	 *
	 * @tparam TItem Type of the objects.
	 * @tparam TContainer Type of the container that will store the objects.
	 * @tparam TModContainer Type of the containe that will store "to add" and "dead" objects. Consider using a set to avoid accidentally deleting objects twice.
	 *
	 */
	template<typename TItem, typename TContainer = std::vector<TItem*>, typename TModContainer = std::vector<TItem*>> class MemoryManager
	{
		private:
			typedef Traits::Container<TContainer, TItem*> ContainerTraits; /*!< Typedef for the main container traits. */
			typedef Traits::Container<TModContainer, TItem*> ModContainerTraits; /*!< Typedef for the "to add" / "to delete" container traits. */

			TContainer items; /*!< Container that stores the objects. */
			TModContainer itemsToAdd; /*!< Container that stores the items that will be added on `cleanUp()`. */
			TModContainer itemsToDel; /*!< Container that stores the "dead" objects. */

		public:
			/*!
			 *
			 * @brief Default constructor.
			 *
			 * Initializes both containers calling the traits's init method.
			 *
			*/
			MemoryManager() { ContainerTraits::init(items); ModContainerTraits::init(itemsToAdd); ModContainerTraits::init(itemsToDel); }

			/*!
			 *
			 * @brief Default destructor.
			 *
			 * Deallocates every object.
			 *
			*/
			~MemoryManager() { for(const auto& i : items) delete i; }

			/*!
			 *
			 * @brief Starts managing a pre-existing pre-heap-allocated object.
			 *
			 * Do not pass non-heap-allocated objects to this function.
			 * Only objects already allocated with `new` are accepted.
			 *
			 * @param mItem Item to "adopt". It has to be already allocated with `new`.
			 *
			*/
			void adopt(TItem& mItem) { ContainerTraits::add(items, &mItem); }

			/*!
			 *
			 * @brief Starts managing a pre-existing pre-heap-allocated object. (user-decided type)
			 *
			 * Do not pass non-heap-allocated objects to this function.
			 * Only objects already allocated with `new` are accepted.
			 *
			 * @tparam TItemType Type of the object. Must be possible to statically cast it to TItem*.
			 * @param mItem Item to "adopt". It has to be already allocated with `new`.
			 *
			*/
			template<typename TItemType> void adopt(TItemType& mItem) { ContainerTraits::add(items, static_cast<TItem*>(&mItem)); }

			/*!
			 *
			 * @brief Allocates a new object and returns a reference to it. It will be added immediately. (user-decided type)
			 *
			 * @tparam TItemType Type of the object. Must be possible to statically cast it to TItem*.
			 * @tparam TArgs Variadic types for the arguments of the object's constructor.
			 * @param mArgs Arguments for the object's constructor.
			 *
			 * @return Returns a reference to a heap-allocated object.
			 *
			 */
			template<typename TItemType, typename... TArgs> TItemType& create(TArgs&&... mArgs)
			{
				TItemType* result{new TItemType(std::forward<TArgs>(mArgs)...)};
				adopt<TItemType>(*result); return *result;
			}

			/*!
			 *
			 * @brief Allocates a new object and returns a reference to it. It will be added on next `cleanUp()`. (user-decided type)
			 *
			 * @tparam TItemType Type of the object. Must be possible to statically cast it to TItem*.
			 * @tparam TArgs Variadic types for the arguments of the object's constructor.
			 * @param mArgs Arguments for the object's constructor.
			 *
			 * @return Returns a reference to a heap-allocated object.
			 *
			 */
			template<typename TItemType, typename... TArgs> TItemType& createDelayed(TArgs&&... mArgs)
			{
				TItemType* result{new TItemType(std::forward<TArgs>(mArgs)...)};
				ModContainerTraits::add(itemsToAdd, static_cast<TItem*>(result)); return *result;
			}

			/*!
			 *
			 * @brief Allocates a new object and returns a reference to it. It will be added on next immediately.
			 *
			 * @tparam TArgs Variadic types for the arguments of the object's constructor.
			 * @param mArgs Arguments for the object's constructor.
			 *
			 * @return Returns a reference to a heap-allocated object.
			 *
			 */
			template<typename... TArgs> TItem& create(TArgs&&... mArgs)	{ return create<TItem, TArgs...>(std::forward<TArgs>(mArgs)...); }

			/*!
			 *
			 * @brief Allocates a new object and returns a reference to it. It will be added on next `cleanUp()`.
			 *
			 * @tparam TArgs Variadic types for the arguments of the object's constructor.
			 * @param mArgs Arguments for the object's constructor.
			 *
			 * @return Returns a reference to a heap-allocated object.
			 *
			 */
			template<typename... TArgs> TItem& createDelayed(TArgs&&... mArgs)	{ return createDelayed<TItem, TArgs...>(std::forward<TArgs>(mArgs)...); }

			/*!
			 *
			 * @brief Marks an object as "dead".
			 *
			 * The object is not actually deallocated. To deallocate it, call cleanUp().
			 *
			 * @param mItem Pointer to the object that will be marked as "dead".
			 *
			 */
			void del(TItem* mItem) { ModContainerTraits::add(itemsToDel, mItem); }

			/*!
			 *
			 * @brief Deallocates all "dead" objects, adds all the "to add" objects.
			 *
			 * Removes all "dead" objects from the main container, deallocates them (calls delete), and clears the "dead" container.
			 * After that, it 'adopts' all the objects previously created with `createDelayed(...)`, effectively adding them in the main container.
			 *
			 */
			void cleanUp()
			{
				for(const auto& i : itemsToDel) { ContainerTraits::del(items, i); delete i; }
				ModContainerTraits::clear(itemsToDel);

				for(const auto& i : itemsToAdd) adopt(*i);
				ModContainerTraits::clear(itemsToAdd);
			}

			/*!
			 *
			 * @brief Clears the MemoryManager.
			 *
			 * All objects are deallocated, all containers are cleared.
			 *
			 */
			void clear()
			{
				for(const auto& i : items) delete i;
				ContainerTraits::clear(items);
				ModContainerTraits::clear(itemsToAdd);
				ModContainerTraits::clear(itemsToDel);
			}

			/*!
			 *
			 * @brief Gets a reference to the main container.
			 *
			 * @return Returns a reference to the main container.
			 *
			 */
			TContainer& getItems() { return items; }


			// Foreach loop/algorithms iterator support
			typedef typename std::vector<TItem*>::iterator iterator;
			typedef typename std::vector<TItem*>::const_iterator const_iterator;

			iterator begin()				{ return items.begin(); }
			const_iterator begin() const	{ return items.begin(); }
			const iterator cbegin() const	{ return items.cbegin(); }
			iterator end()					{ return items.end(); }
			const_iterator end() const		{ return items.end(); }
			const iterator cend() const		{ return items.cend(); }
	};
}

#endif
