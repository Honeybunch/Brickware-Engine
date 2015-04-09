#define BRICKWARE_UTILITY_EXPORTS

#include "JSONParser.h"

using namespace Brickware;
using namespace Utility;

JSONObject* JSONParser::DecodeJSONFromFile(const char* filename)
{
	char* filecontents = StringUtils::textFileRead(filename);
	if (filecontents == NULL)
	{
		std::cout << "Could not read JSON file" << std::endl;
		return new JSONObject();
	}

	return DecodeJSONFromString(filecontents);
}

JSONObject* JSONParser::DecodeJSONFromString(const char* rawString)
{
	//We want to strip all whitespace from the raw string
	const char* jsonString = StringUtils::trimAllWhitespace(rawString);

	JSONObject* object = parseObject(jsonString);
	bool boolean = *object->getValue<bool*>("testBool");
	char* string = object->getValue<char*>("testStr");
	int integer = *object->getValue<int*>("testInt");
	float floatingPoint = *object->getValue<float*>("testFloat");
	JSONObject* nestedObject = object->getValue<JSONObject*>("testObject");
	int nestedInt = *nestedObject->getValue<int*>("testObjectInt");
	char* nestedString = nestedObject->getValue<char*>("testObjectStr");

	std::cout << boolean << std::endl;
	std::cout << string << std::endl;
	std::cout << integer << std::endl;
	std::cout << floatingPoint << std::endl;
	std::cout << '\t' << nestedInt << std::endl;
	std::cout << '\t' << nestedString << std::endl;

	return object;
}