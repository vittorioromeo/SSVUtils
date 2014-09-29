// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VALUE_INL
#define SSVU_JSON_VALUE_INL

namespace ssvu
{
	namespace Json
	{
		template<WriterMode TWS, bool TFmt> inline void Val::writeToStream(std::ostream& mStream) const	{ Internal::Writer<TWS, TFmt> w; w.write(*this, mStream); mStream.flush(); }
		template<WriterMode TWS, bool TFmt> inline void Val::writeToStr(std::string& mStr) const		{ std::ostringstream o; writeToStream<TWS, TFmt>(o); mStr = o.str(); }
		template<WriterMode TWS, bool TFmt> inline void Val::writeToFile(const ssvufs::Path& mPath) const	{ std::ofstream o{mPath}; writeToStream<TWS, TFmt>(o); o.close(); }
		template<WriterMode TWS, bool TFmt> inline auto Val::getWriteToStr() const						{ std::string result; writeToStr<TWS, TFmt>(result); return result; }

		inline void Val::readFromStr(std::string mStr)				{ Internal::Reader r{std::move(mStr)};							Internal::tryParse(*this, r); }
		inline void Val::readFromFile(const ssvufs::Path& mPath)	{ Internal::Reader r{std::move(mPath.getContentsAsString())};;	Internal::tryParse(*this, r); }
	}
}

#endif
