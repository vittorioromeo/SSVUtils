# SSVUtils

C++14 multi-purpose utility library that only depends on the STL.

*The code examples below are currently outdated.*

---

###CommandLine
```cpp
```

###Delegates

```cpp
int result{0};
ssvu::Delegate<void(int)> onSomething;
onSomething += [&](int mValue){ result += mValue; };
onSomething += [&](int mValue){ result += mValue * 2; };
onSomething(2);
assert(result == 6);
```

###Encryption
```cpp
string source{"encrypt me!"};
auto encrypted = ssvu::Encryption::encrypt<Encryption::Type::Base64>(source);
auto decrypted = ssvu::Encryption::decrypt<Encryption::Type::Base64>(encrypted);
assert(source == decrypted);
```

###FileSystem
```cpp
string myPath{"/home/folder"};
ssvu::FileSystem::normalizePath(myPath);
assert(myPath == "/home/folder/");

if(!ssvu::FileSystem::exists(myPath)) ssvu::FileSystem::createFolder(myPath);

ssvu::FileSystem::removeFile(myPath + "garbage_file");

string homePath{ssvu::FileSystem::getParentPath(myPath)};
assert(homePath == "/home/");

for(const auto& filePath : ssvu::FileSystem::getScan<Mode::Recurse, Type::File>(myPath))
	// Iterate on all file paths (recursively)

for(const auto& subFolderPath : ssvu::FileSystem::getScan<Mode::Recurse, Type::Folder>(myPath))
	// Iterate on all sub folder paths (recursively)

// and more...
```

###Global
```cpp
// std::unique_ptr is verbose
using namespace ssvu;

Uptr<int> test{new int};
Uptr<int, SomeCustomDeleter>{new int};
```

###Log
```cpp
using namespace ssvu;

lo << lt("Testing log") << "Hello!" << endl;
// Prints to console: "[Testing log]	Hello!"

saveLogToFile("log.txt");
// Saves everything that was logged to "log.txt"
```

###MemoryManager
```cpp
```

###PreAlloc
```cpp
```

###String
```cpp
string test{"abcdef"};

assert(ssvu::beginsWith(test, "abc"));
assert(ssvu::endsWith(test, "def"));

string test2{"ABCDEF"};
assert(ssvu::toLower(test2) == test);

ssvu::replaceAll(test, "abc", "ABC");
assert(ssvu::beginsWith(test, "ABC"));

test += ssvu::toStr(100);
assert(test == "ABCdef100");

// and more...
```

###TemplateSystem
```cpp
// Example 1
using namespace ssvu::TemplateSystem;
Dictionary dict{{"key1", "value1"}, {"key2", "value2"}};
string toExpand{".{{key1}}.{{key2}}."};
assert(dict.getExpanded(toExpand) == ".value1.value2.");

// Example 2
using namespace ssvu::TemplateSystem;
Dictionary dict;
Dictionary subdict1{{"key", "value1"}};
Dictionary subdict2{{"key", "value2"}};
dict += {"section", subdict1};
dict += {"section", subdict2};
string toExpand{"{{#section}}.{{key}}.{{/section}}"};
assert(dict.getExpanded(toExpand) == ".value1..value2.");
```

###Timeline
```cpp
```

###Utils
```cpp
auto i = ssvu::getRnd(0, 5); // get random int [0, 5)
auto f = ssvu::getRndF(0.f, 5.f); // get random float [0.f, 5.f)

auto x = ssvu::getRnd(0, 100);
auto clamped = ssvu::getClamped(x, 40, 60); // gets x clamped between 40 and 60

auto rr = ssvu::toRadians(720.f);
auto dd = ssvu::toDegrees(rr);
assert(ssvu::wrapDegrees(dd) == 360.f);

std::vector<int> container{0, 5, 10, 15};
ssvu::eraseRemove(container, 10);
// container is now = {0, 5, 15}

ssvu::eraseRemoveIf(container, [&](const int& mValue){ return mValue > 0; });
// container is now = {0}

assert(ssvu::contains(container, 0));
assert(!ssvu::contains(container, 5));
assert(!ssvu::containsAny(container, {10, 15}));

// and more...
```
