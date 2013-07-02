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
			template<Mode TM, Type TT, Pick TP, Sort TS> static void scan(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mDesired)
			{
				if(isFolder(mPath))
				{
					DIR* dir{opendir(mPath.c_str())};
					dirent* entry{readdir(dir)};

					while(entry != NULL)
					{
						std::string name{entry->d_name}, path{getNormalizedPath(getNormalizedPath(mPath) + name)};
						if(!isRootOrParent(path))
						{
							if(isFolder(path))
							{
								if(TT == Type::All || TT == Type::Folder) { mTarget.push_back(path); }
								if(TM == Mode::Recurse) { scan<Mode::Recurse, TT, TP, TS>(mTarget, path, mDesired); }
							}
							else if(TT == Type::All || TT == Type::File)
							{
								if(TP == Pick::Any)			{ mTarget.push_back(path); }
								else if(TP == Pick::ByExt)	{ if(endsWith(name, mDesired)) mTarget.push_back(path); }
								else if(TP == Pick::ByName)	{ if(name == mDesired) mTarget.push_back(path); }
							}
						}

						entry = readdir(dir);
					}

					closedir(dir);
				}
				else log("Directory \"" + mPath + "\" not found", "ssvu::FileSystem::ScanHelper");

				if(TS == Sort::Alphabetic) std::sort(std::begin(mTarget), std::end(mTarget));
			}
		}
	}
}

#endif
