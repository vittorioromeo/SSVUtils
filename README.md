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

###Log

###MemoryManager

###PreAlloc

###String

###TemplateSystem

###Timeline

###Utils
