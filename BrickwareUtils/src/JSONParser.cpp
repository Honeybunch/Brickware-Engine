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
	//We want to strip all whitespace from the raw string
	char* jsonString = StringUtils::trimAllWhitespace(rawString);

	JSONObject object = parseObject(jsonString);
	bool val = object.getValue<bool>("test");

	std::cout << val << std::endl;

	return object;
}