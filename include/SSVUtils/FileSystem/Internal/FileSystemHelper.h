// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_HELPER
#define SSVU_FILESYSTEM_HELPER

#include "SSVUtils/FileSystem/FileSystemUtils.h"

namespace ssvu
{
	namespace FileSystem
	{
		enum class Mode { RECURSIVE, NON_RECURSIVE };
		enum class Type { ALL, FILES, FOLDERS };
		enum class Pick { ANY, ENDING_WITH };

		template<Pick TP, typename... TArgs> struct PickHelper;
		template<typename... TArgs> struct PickHelper<Pick::ANY, TArgs...>
		{
			inline static void pick(std::vector<std::string>& mTarget, const std::string& mPath) { mTarget.push_back(mPath); }
		};
		template<typename... TArgs> struct PickHelper<Pick::ENDING_WITH, TArgs...>
		{
			inline static void pick(std::vector<std::string>& mTarget, const std::string& mPath) { // TODO }
		};

		template<Type TT, Pick TP, typename... TArgs> struct TypeHelper;
		template<Pick TP, typename... TArgs> struct TypeHelper<Type::ALL, TP, TArgs...>
		{
			inline static void pickFolder(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP, TArgs...>::pick(mTarget, mPath); }
			inline static void pickFile(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP, TArgs...>::pick(mTarget, mPath); }
		};
		template<Pick TP, typename... TArgs> struct TypeHelper<Type::FILES, TP, TArgs...>
		{
			inline static void pickFolder(std::vector<std::string>&, const std::string&) { }
			inline static void pickFile(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP, TArgs...>::pick(mTarget, mPath); }
		};
		template<Pick TP, typename... TArgs> struct TypeHelper<Type::FOLDERS, TP, TArgs...>
		{
			inline static void pickFolder(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP, TArgs...>::pick(mTarget, mPath); }
			inline static void pickFile(std::vector<std::string>&, const std::string&) { }
		};

		template<Mode TM, Type TT, Pick TP, typename... TArgs> struct FillHelper;

		template<Mode TM, Type TT, Pick TP> struct ModeHelper;
		template<Type TT, Pick TP> struct ModeHelper<Mode::RECURSIVE, TT, TP>
		{
			inline static void recurse(std::vector<std::string>& mTarget, const std::string& mPath) { FillHelper<Mode::RECURSIVE, TT, TP>::fill(mTarget, mPath); }
		};
		template<Type TT, Pick TP> struct ModeHelper<Mode::NON_RECURSIVE, TT, TP>
		{
			inline static void recurse(std::vector<std::string>&, const std::string&) { }
		};

		template<Mode TM, Type TT, Pick TP = Pick::ANY, typename... TArgs> struct FillHelper
		{
			inline static void fill(std::vector<std::string>& mTarget, const std::string& mPath, TArgs... mArgs)
			{
				if(isFolder(mPath))
				{
					DIR* dir{opendir(mPath.c_str())};
					dirent* entry{readdir(dir)};

					while(entry != NULL)
					{
						std::string name{entry->d_name}, path{getNormalizedPath(mPath) + "/" + name};

						if(!isRootOrParent(path))
						{
							if(isFolder(path))
							{
								TypeHelper<TT, TP, TArgs...>::pickFolder(mTarget, path);
								ModeHelper<TM, TT, TP>::recurse(mTarget, path);
							}
							else TypeHelper<TT, TP, TArgs...>::pickFile(mTarget, path);
						}

						entry = readdir(dir);
					}

					closedir(dir);
				}
				else ssvu::log("Directory \"" + mPath + "\" not found", "FileSystem");
			}
		};
	}
}

#endif
