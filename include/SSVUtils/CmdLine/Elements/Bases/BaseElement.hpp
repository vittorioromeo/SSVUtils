// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_BASES_BASEELEMENT
#define SSVU_CMDLINE_ELEMENTS_BASES_BASEELEMENT

namespace ssvu
{
	namespace CmdLine
	{
		namespace Internal
		{
			class BaseElement
			{
				private:
					std::string name, briefDesc, desc;

				public:
					inline virtual ~BaseElement() { }

					SSVU_DEFINE_SINK_SETTER_SIMPLE(setName, name)
					SSVU_DEFINE_SINK_SETTER_SIMPLE(setBriefDesc, briefDesc)
					SSVU_DEFINE_SINK_SETTER_SIMPLE(setDesc, desc)

					inline const auto& getName() const noexcept			{ return name; }
					inline const auto& getBriefDesc() const noexcept	{ return briefDesc; }
					inline const auto& getDesc() const noexcept			{ return desc; }

					virtual std::string getUsageStr() const = 0;

					inline auto getHelpStr()
					{
						std::string result, usageStr{this->getUsageStr()};

						if(!usageStr.empty())	appendTo(result, "* ", usageStr);
						if(!name.empty())		appendTo(result, "\n  --", name);
						if(!briefDesc.empty())	appendTo(result, "\n  --", briefDesc);
						if(!desc.empty())		appendTo(result, "\n  --", desc);

						return result + "\n\n";
					}
			};
		}
	}
}

#endif
