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
	bool boolean = *object.getValue<bool*>("testBool");
	char* string = object.getValue<char*>("testStr");

	std::cout << boolean << std::endl;
	std::cout << string << std::endl;

	return object;
}