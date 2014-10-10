// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_FILESYSTEM_INTERNAL_SCANHELPER
#define SSVU_CORE_FILESYSTEM_INTERNAL_SCANHELPER

namespace ssvu
{
	namespace FileSystem
	{
		namespace Internal
		{
			template<Mode TM, Type TT, Pick TP, Sort TS> inline void scan(std::vector<Path>& mTarget, const Path& mPath, const std::string& mDesired)
			{
				if(!mPath.exists<Type::Folder>()) throw std::runtime_error{"Directory \"" + mPath + "\" not found"};

				DIR* dir{opendir(mPath.getCStr())};
				std::string bufName;
				Path bufPath;

				for(dirent* entry{readdir(dir)}; entry != nullptr; entry = readdir(dir))
				{
					bufName = entry->d_name;
					bufPath = mPath + bufName;

					if(!bufPath.isRootOrParent())
					{
						if(bufPath.exists<Type::Folder>())
						{
							if(TT == Type::All || TT == Type::Folder) { mTarget.emplace_back(bufPath); }
							if(TM == Mode::Recurse) { Internal::scan<Mode::Recurse, TT, TP, TS>(mTarget, bufPath, mDesired); }
						}
						else if(TT == Type::All || TT == Type::File)
						{
							if(TP == Pick::Any)			{ mTarget.emplace_back(bufPath); }
							else if(TP == Pick::ByExt)	{ if(endsWith(bufName, mDesired)) mTarget.emplace_back(bufPath); }
							else if(TP == Pick::ByName)	{ if(bufName == mDesired) mTarget.emplace_back(bufPath); }
						}
					}
				}

				closedir(dir);
				if(TS == Sort::Alphabetic) sort(mTarget);
			}
		}
	}
}

#endif
