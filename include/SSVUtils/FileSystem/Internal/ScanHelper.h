// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM_INTERNAL_SCANHELPER
#define SSVU_FILESYSTEM_INTERNAL_SCANHELPER

#include "SSVUtils/FileSystem/Enums.h"
#include "SSVUtils/FileSystem/Utils.h"

namespace ssvu
{
	namespace FileSystem
	{
		namespace Internal
		{
			template<Sort TS> struct SortHelper;
			template<> struct SortHelper<Sort::Alphabetic>
			{
				inline static void sort(std::vector<std::string>& mTarget) { std::sort(std::begin(mTarget), std::end(mTarget)); }
			};
			template<> struct SortHelper<Sort::Unsorted>
			{
				inline static void sort(std::vector<std::string>&) { }
			};

			template<Pick TP> struct PickHelper;
			template<> struct PickHelper<Pick::Any>
			{
				inline static void pick(std::vector<std::string>& mTarget, const std::string& mPath, const std::string&, const std::string&) { mTarget.push_back(mPath); }
			};
			template<> struct PickHelper<Pick::ByExt>
			{
				inline static void pick(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mName, const std::string& mDesiredExtension) { if(endsWith(mName, mDesiredExtension)) mTarget.push_back(mPath); }
			};
			template<> struct PickHelper<Pick::ByName>
			{
				inline static void pick(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mName, const std::string& mDesiredName) { if(mName == mDesiredName) mTarget.push_back(mPath); }
			};

			template<Type TT, Pick TP> struct TypeHelper;
			template<Pick TP> struct TypeHelper<Type::All, TP>
			{
				inline static void pickFolder(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mName, const std::string& mDesired) { PickHelper<TP>::pick(mTarget, mPath, mName, mDesired); }
				inline static void pickFile(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mName, const std::string& mDesired) { PickHelper<TP>::pick(mTarget, mPath, mName, mDesired); }
			};
			template<Pick TP> struct TypeHelper<Type::File, TP>
			{
				inline static void pickFolder(std::vector<std::string>&, const std::string&, const std::string&, const std::string&) { }
				inline static void pickFile(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mName, const std::string& mDesired) { PickHelper<TP>::pick(mTarget, mPath, mName, mDesired); }
			};
			template<Pick TP> struct TypeHelper<Type::Folder, TP>
			{
				inline static void pickFolder(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mName, const std::string& mDesired) { PickHelper<TP>::pick(mTarget, mPath, mName, mDesired); }
				inline static void pickFile(std::vector<std::string>&, const std::string&, const std::string&, const std::string&) { }
			};

			template<Mode TM, Type TT, Pick TP, Sort TS> struct ScanHelper;

			template<Mode TM, Type TT, Pick TP, Sort TS> struct ModeHelper;
			template<Type TT, Pick TP, Sort TS> struct ModeHelper<Mode::Recurse, TT, TP, TS>
			{
				inline static void recurse(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mDesired) { ScanHelper<Mode::Recurse, TT, TP, TS>::scan(mTarget, mPath, mDesired); }
			};
			template<Type TT, Pick TP, Sort TS> struct ModeHelper<Mode::Single, TT, TP, TS>
			{
				inline static void recurse(std::vector<std::string>&, const std::string&, const std::string&) { }
			};

			template<Mode TM, Type TT, Pick TP, Sort TS> struct ScanHelper
			{
				inline static void scan(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mDesired)
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
									TypeHelper<TT, TP>::pickFolder(mTarget, path, name, mDesired);
									ModeHelper<TM, TT, TP, TS>::recurse(mTarget, path, mDesired);
								}
								else TypeHelper<TT, TP>::pickFile(mTarget, path, name, mDesired);
							}

							entry = readdir(dir);
						}

						closedir(dir);
					}
					else log("Directory \"" + mPath + "\" not found", "FileSystem");

					SortHelper<TS>::sort(mTarget);
				}
			};
		}
	}
}

#endif
