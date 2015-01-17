// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE
#define SSVU_CORE

// "Core" is the main module of the library: its parts all depend on each other,
// and cannot be used separately.

// STL Dependencies
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cassert>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <random>
#include <map>
#include <set>
#include <cstddef>
#include <functional>
#include <climits>
#include <stack>
#include <memory>
#include <iterator>
#include <cstring>
#include <bitset>
#include <future>
#include <mutex>
#include <list>

// The "Core/Detection/Detection" module detects OS, compiler and debug/release information
#include "SSVUtils/Core/Detection/Detection.hpp"

// The "Core/Preprocessor/Preprocessor" module is used in various parts of the library.
// It does not introduce any additional dependency and is safe to include first.
#include "SSVUtils/Core/Preprocessor/Preprocessor.hpp"

// "Core/Assert/Assert" comes next, as many other modules make heavy use of assertions.
#include "SSVUtils/Core/Assert/Assert.hpp"

// The "Core/Common/Common" module defines various typedefs and shortcut functions.
// It also includes some commonly used STL header files and checks the current OS.
#include "SSVUtils/Core/Common/Common.hpp"

// "Core/Utils/Utils" module has many utility functions that are used heavily throughout
// the whole library.
#include "SSVUtils/Core/Utils/Utils.hpp"

// "Core/Tuple/Tuple" module has many tuple utility functions that are used throughout
// the whole library.
#include "SSVUtils/Core/Tuple/Tuple.hpp"

// "Core/String/String" and its utilities are required for logging and stringification.
// It needs to be included before the logging module.
#include "SSVUtils/Core/String/String.hpp"

// "Core/ConsoleFmt/ConsoleFmt" deals with console output formatting. It is requried for
// stringification methods.
#include "SSVUtils/Core/ConsoleFmt/ConsoleFmt.hpp"

// "Core/Stringifier/Stringifier" deals with pretty-print and value to string conversions.
#include "SSVUtils/Core/Stringifier/Stringifier.hpp"

// "Core/Log/Log" is also used in most of the library.
// "Core/Assert/Assert"'s implementation requires it as well.
#include "SSVUtils/Core/Log/Log.hpp"
#include "SSVUtils/Core/Assert/Assert.inl"

// "Core/FileSystem/FileSystem" deals with paths and file management.
// "Core/Log/Log"'s implementation requires it as well.
#include "SSVUtils/Core/FileSystem/FileSystem.hpp"
#include "SSVUtils/Core/Log/Log.inl"

#endif
