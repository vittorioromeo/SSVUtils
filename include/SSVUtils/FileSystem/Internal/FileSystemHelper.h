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

		template<Pick TP> struct PickHelper;
		template<> struct PickHelper<Pick::ANY>
		{
			inline static void pick(std::vector<std::string>& mTarget, const std::string& mPath) { mTarget.push_back(mPath); }
		};
		template<> struct PickHelper<Pick::ENDING_WITH>
		{
			inline static void pick(std::vector<std::string>& mTarget, const std::string& mPath) { /* todo */ }
		};

		template<Type TT, Pick TP> struct TypeHelper;
		template<Pick TP> struct TypeHelper<Type::ALL, TP>
		{
			inline static void pickFolder(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP>::pick(mTarget, mPath); }
			inline static void pickFile(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP>::pick(mTarget, mPath); }
		};
		template<Pick TP> struct TypeHelper<Type::FILES, TP>
		{
			inline static void pickFolder(std::vector<std::string>&, const std::string&) { }
			inline static void pickFile(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP>::pick(mTarget, mPath); }
		};
		template<Pick TP> struct TypeHelper<Type::FOLDERS, TP>
		{
			inline static void pickFolder(std::vector<std::string>& mTarget, const std::string& mPath) { PickHelper<TP>::pick(mTarget, mPath); }
			inline static void pickFile(std::vector<std::string>&, const std::string&) { }
		};

		template<Mode TM, Type TT, Pick TP> struct FillHelper;

		template<Mode TM, Type TT, Pick TP> struct ModeHelper;
		template<Type TT, Pick TP> struct ModeHelper<Mode::RECURSIVE, TT, TP>
		{
			inline static void recurse(std::vector<std::string>& mTarget, const std::string& mPath) { FillHelper<Mode::RECURSIVE, TT, TP>::fill(mTarget, mPath); }
		};
		template<Type TT, Pick TP> struct ModeHelper<Mode::NON_RECURSIVE, TT, TP>
		{
			inline static void recurse(std::vector<std::string>&, const std::string&) { }
		};

		template<Mode TM, Type TT, Pick TP = Pick::ANY> struct FillHelper
		{
			inline static void fill(std::vector<std::string>& mTarget, const std::string& mPath)
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
								TypeHelper<TT, TP>::pickFolder(mTarget, path);
								ModeHelper<TM, TT, TP>::recurse(mTarget, path);
							}
							else TypeHelper<TT, TP>::pickFile(mTarget, path);
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
