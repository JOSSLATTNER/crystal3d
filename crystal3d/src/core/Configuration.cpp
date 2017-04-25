#include "Configuration.h"

namespace Core
{
	CrConfiguration::CrConfiguration(const IO::CrPath & a_Filename)
	{
		CrDebugSection("Configuration Parser");

		IO::CrPath configFolder( _CR_ENGINE_CONFIG );
		IO::CrPath iniFile = a_Filename;

		if (!a_Filename.is_absolute())
			iniFile = configFolder / a_Filename;

		std::ifstream ifs(iniFile);
		if (!ifs.good())
			throw CrException("Failed to load config file.");

		//Buffer entire file
		std::stringstream buffer;
		buffer << ifs.rdbuf();

		std::string currentGroup;
		for (std::string line; Util::getline_safe(buffer, line);)
		{
			if (line.empty()) //skip empty lines
				continue;
			if (line.front() == ';')
			{
				CrDebugOutput("Comment: %s", line.c_str() + 1);
			}
			else if (line.front() == '[')
			{
				//Strip '[' & ']'
				currentGroup = { line.begin() + 1, line.end() - 1 };
				CrDebugOutput("Group: %s", currentGroup.c_str());

				//Create new group
				m_Keys[currentGroup] = {};
			}
			else
			{
				//When need at least 1 group
				if (m_Keys.empty())
					throw CrException("Cant have key/value pair without group");

				//Split at '='
				auto del = line.find('=');
				if (del == std::string::npos)
					throw CrException("Failed to parse key/value pair");

				//Extract key/value
				std::string key = line.substr(0, del);
				std::string val = line.substr(del + 1);

				CrDebugOutput("%s:%s", key.c_str(), val.c_str());

				//Add key/value pair
				m_Keys[currentGroup].insert({ key,val });
			}

		}
	}

	CrConfiguration::~CrConfiguration()
	{
	}

	std::string CrConfiguration::GetStringValue(const std::string & a_Group, const std::string & a_Key)
	{
		auto container = m_Keys[a_Group];
		auto it = container.find(a_Key);
		if (it == container.end())
			throw CrException("Expected value");
		return it->second;
	}
}