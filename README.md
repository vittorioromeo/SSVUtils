# SSVUtils

C++11 multi-purpose utility library that only depends on the STL.

---

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
```

###Timeline
```cpp
```

###Utils
```cpp
```