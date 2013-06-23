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
	 * Calling cleanUp() deallocates all the "dead" objects and removes them from the object list.
	 * Can be useful to manage game entities, timelines, etc...
	 *
	 * @tparam TItem Type of the objects.
	 * @tparam TContainer Type of the container that will store the objects.
	 * @tparam TDelContainer Type of the containe that will store "dead" objects. Consider using a set to avoid accidentally deleting objects twice.
	 *
	 */
	template<typename TItem, typename TContainer = std::vector<TItem*>, typename TDelContainer = std::vector<TItem*>> class MemoryManager
	{
		private:
			typedef Traits::Container<TContainer, TItem*> ContainerTraits; /*!< Typedef for the main container traits. */
			typedef Traits::Container<TDelContainer, TItem*> DelContainerTraits; /*!< Typedef for the "dead" container traits. */

			TContainer items; /*!< Container that stores the objects. */
			TDelContainer itemsToDel; /*!< Container that stores the "dead" objects. */

		public:
			/*!
			 *
			 * @brief Default constructor.
			 *
			 * Initializes both containers calling the traits's init method.
			 *
			*/
			MemoryManager() { ContainerTraits::init(items); DelContainerTraits::init(itemsToDel); }

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
			 * @brief Allocates a new object and returns a reference to it. (user-decided type)
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
				ContainerTraits::add(items, static_cast<TItem*>(result)); return *result;
			}

			/*!
			 *
			 * @brief Allocates a new object and returns a reference to it.
			 *
			 * @tparam TArgs Variadic types for the arguments of the object's constructor.
			 * @param mArgs Arguments for the object's constructor.
			 *
			 * @return Returns a reference to a heap-allocated object.
			 *
			 */
			template<typename... TArgs> TItem& create(TArgs&&... mArgs)
			{
				TItem* result{new TItem(std::forward<TArgs>(mArgs)...)};
				ContainerTraits::add(items, result); return *result;
			}

			/*!
			 *
			 * @brief Marks an object as "dead".
			 *
			 * The object is not actually deallocated. To deallocate it, call cleanUp().
			 *
			 * @param mItem Pointer to the object that will be marked as "dead".
			 *
			 */
			void del(TItem* mItem) { DelContainerTraits::add(itemsToDel, mItem); }

			/*!
			 *
			 * @brief Deallocates all "dead" objects.
			 *
			 * Removes all "dead" objects from the main container, deallocates them (calls delete), and clears the "dead" container.
			 *
			 */
			void cleanUp()
			{
				for(const auto& i : itemsToDel) { ContainerTraits::del(items, i); delete i; }
				DelContainerTraits::clear(itemsToDel);
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
				DelContainerTraits::clear(itemsToDel);
			}

			/*!
			 *
			 * @brief Gets a reference to the main container.
			 *
			 * @return Returns a reference to the main container.
			 *
			 */
			TContainer& getItems() { return items; }

			typedef typename std::vector<TItem*>::iterator iterator;
			typedef typename std::vector<TItem*>::const_iterator const_iterator;

			iterator begin() { return items.begin(); }
			const_iterator begin() const { return items.begin(); }
			const iterator cbegin() const { return items.cbegin(); }
			iterator end() { return items.end(); }
			const_iterator end() const { return items.end(); }
			const iterator cend() const { return items.cend(); }
	};
}

#endif
