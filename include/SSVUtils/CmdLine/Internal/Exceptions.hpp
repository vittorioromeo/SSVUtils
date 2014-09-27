// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_INTERNAL_EXCEPTIONS
#define SSVU_CMDLINE_INTERNAL_EXCEPTIONS

namespace ssvu
{
	namespace CmdLine
	{
		namespace Exception
		{
			class Base : public std::runtime_error
			{
				private:
					std::string title;

				public:
					inline Base(const std::string& mTitle, const std::string& mStr) : std::runtime_error{mStr}, title{mTitle} { }
					inline const auto& getTitle() const noexcept { return title; }
			};

			inline Base createFlagNotFound(const std::string& mFlagName, const std::string& mCmdName)
			{
				return {"Flag not found", "Flag `" +  mFlagName + "` not found in command " + mCmdName};
			}

			inline Base createCmdNotFound(const std::string& mCmdName, const std::string& mSuggestion)
			{
				return {"Command not found", "Command with name <" +  mCmdName + "> not found - did you mean <" + mSuggestion + ">?"};
			}

			inline Base createSignatureMismatch(const std::string& mForCmdPhrase, const std::string& mElement, const::std::string& mCorrect)
			{
				return {"Signature mismatch", "Incorrect number of " + mElement + " " + mForCmdPhrase + " - expected: " + mCorrect};
			}

			inline Base createArgPackInfinitePositionError()
			{
				return {"Infinite ArgPack position error", "Infinite ArgPacks must be specified last in the command"};
			}

			inline Base createArgPackSignatureMismatch(Internal::BaseArgPack& mArgPack)
			{
				return {"ArgPack signature mismatch", "Incorrect number of parameters for ArgPack `" + mArgPack.getName() + "` - expected (" + toStr(mArgPack.getMin()) + " .. " + toStr(mArgPack.getMax()) + ")"};
			}

			inline Base createParsingError()
			{
				return {"Parsing error", "Too many arguments used in command line"};
			}

		}
	}
}

#endif
