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
				if(!mPath.existsAsFolder()) throw std::runtime_error{"Directory \"" + mPath + "\" not found"};

				DIR* dir{opendir(mPath.getCStr())};
				dirent* entry{readdir(dir)};

				while(entry != NULL)
				{
					std::string name{entry->d_name};
					Path path{mPath + name};
					if(!path.isRootOrParent())
					{
						if(path.existsAsFolder())
						{
							if(TT == Type::All || TT == Type::Folder) { mTarget.emplace_back(path); }
							if(TM == Mode::Recurse) { Internal::scan<Mode::Recurse, TT, TP, TS>(mTarget, path, mDesired); }
						}
						else if(TT == Type::All || TT == Type::File)
						{
							if(TP == Pick::Any)			{ mTarget.emplace_back(path); }
							else if(TP == Pick::ByExt)	{ if(endsWith(name, mDesired)) mTarget.emplace_back(path); }
							else if(TP == Pick::ByName)	{ if(name == mDesired) mTarget.emplace_back(path); }
						}
					}

					entry = readdir(dir);
				}

				closedir(dir);
				if(TS == Sort::Alphabetic) sort(mTarget);
			}
		}
	}
}

#endif
