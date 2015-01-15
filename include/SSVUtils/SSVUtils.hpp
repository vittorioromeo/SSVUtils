// Copyright (c) 2013-2014 Vittorio Romeo
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
#include "SSVUtils/Timeline/TimelineManager.hpp"
#include "SSVUtils/Encryption/Encryption.hpp"
#include "SSVUtils/Bimap/Bimap.hpp"
#include "SSVUtils/CmdLine/CmdLine.hpp"
#include "SSVUtils/FatEnum/FatEnum.hpp"
#include "SSVUtils/GrowableArray/GrowableArray.hpp"
#include "SSVUtils/HandleVector/HandleVector.hpp"
#include "SSVUtils/Easing/Easing.hpp"
#include "SSVUtils/RecPtr/RecPtr.hpp"
#include "SSVUtils/Ticker/Ticker.hpp"
#include "SSVUtils/Range/Range.hpp"
#include "SSVUtils/UnionVariant/UnionVariant.hpp"
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
// * Internal:: -> Impl:: ?
// * remove "include/" folder

// * .inl and .cpp FILES and SSVU_INLINE macro
/*
	* Benchmark
	* Bimap
	CmdLine
	Container
	Core
	Delegate
	Easing
	Encryption
	FatEnum
	GrowableArray
	HandleVector
	Internal
	Json
	Memorymanager
	Range
	RecPtr
	TemplateSystem
	Test
	Tests
	* Ticker
	Timeline
	UnionVariant
*/
