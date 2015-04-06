#define BRICKWARE_UTILITY_EXPORTS

#include "JSONParser.h"

using namespace Brickware;
using namespace Utility;

JSONObject JSONParser::DecodeJSONFromFile(char* filename)
{
	char* filecontents = StringUtils::textFileRead(filename);
	if (filecontents == NULL)
	{
		std::cout << "Could not read JSON file" << std::endl;
		return JSONObject();
	}

	return DecodeJSONFromString(filecontents);
}

JSONObject JSONParser::DecodeJSONFromString(char* rawString)
{
	std::cout << rawString << std::endl;
	return JSONObject();
}