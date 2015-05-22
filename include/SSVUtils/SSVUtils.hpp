// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVUTILS
#define SSVUTILS

// API configuration
#include "SSVUtils/Internal/API.hpp"

// "Core" module
#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Test/Test.hpp"
#include "SSVUtils/Benchmark/Benchmark.hpp"
#include "SSVUtils/Timeline/Timeline.hpp"
#include "SSVUtils/Delegate/Delegate.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"
#include "SSVUtils/TemplateSystem/TemplateSystem.hpp"
#include "SSVUtils/Encryption/Encryption.hpp"
#include "SSVUtils/Bimap/Bimap.hpp"
#include "SSVUtils/CmdLine/CmdLine.hpp"
#include "SSVUtils/FatEnum/FatEnum.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
#include "SSVUtils/HandleVector/HandleVector.hpp"
#include "SSVUtils/Easing/Easing.hpp"
#include "SSVUtils/Ticker/Ticker.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/Union/Union.hpp"
#include "SSVUtils/Container/Container.hpp"
#include "SSVUtils/Json/Json.hpp"

#ifndef SSVUT_DISABLE
	#include "SSVUtils/Tests/Tests.hpp"
#endif

#endif

// TODO:
// * Fix includes (old method was better)
// * investigate metaprogramming code generation
// * test framework - write expected and actual value and more...
// * ModuleName/Impl/...
// * zip iterator
// * experiment/consider reimplementing vector/map/flatmap with no exceptions and only asserts
// * remove "include/" folder
// * "String" -> "Str"

// TODO: Internal/ -> Impl/ ?

// * .inl and .cpp FILES and SSVU_INLINE macro
/*
	* Benchmark
	* Bimap
	CmdLine
	* Container
	Core
	* Delegate
	* Easing
	Encryption
	FatEnum
	GrowableArrayAS
	HandleVector
	Internal
	Json
	Memorymanager
	Range
	TemplateSystem
	Test
	Tests
	* Ticker
	* Timeline
	* Union
*/

// TODO: TArgs -> Ts (?)
// TODO: size_t literal operator, replace auto(0u), and SizeT() casts
// TODO: SSVCMAKE: release disable tests, maybe enable lfto optimizations, -ffast-math
// TODO: replace for loops with for(auto x : range())
// TODO: check for functionality in GGJ and ASG that can be brought in SSVU
// TODO: BTR::Ptr -> ssvu::Ptr (?)
// TODO: Flags<TImpl> class that can use a bitset or a char array as underlying impls
// TODO: mkMap, mkVector, mkUnorderedMap, and use them
