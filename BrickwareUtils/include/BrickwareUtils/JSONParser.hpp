/*
 *
 * JSONParser.hpp
 *
 * A simple JSON Parser that is header only
 * This depends on the static StringUtils class
 *
 */

#ifndef JSON_H
#define JSON_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//System level headers
#include <vector>

//Project Headers
#include "BrickwareUtils/StringUtils.hpp"

using namespace std;

namespace Brickware
{
	namespace Utility
	{		
		class JSONObject;

		/* A union of types that represents a possible value in JSON
		 *
		 * This union solves the problem of 
		 *  "How do we store data in a template if we don't know its type"
		 *  without using a void* and the possiblity of the user causing a crash
		 *  when retrieving data.
		 */
		union JSONValue
		{
			char* string;
			int integer;
			float floatingPoint;
			bool boolean;
			JSONObject* object;
			std::vector<JSONValue>* array;

			operator char*() const { return string; }
			operator int() const { return integer; }
			operator float() const { return floatingPoint; }
			operator bool() const { return boolean; }
			operator JSONObject*() const { return object; }
			operator std::vector<JSONValue>*() const { return array; }

			JSONValue operator= (char* s)				{ string = s; return *this; }
			JSONValue operator= (int i)				{ integer = i; return *this; }
			JSONValue operator= (float f)				{ floatingPoint = f; return *this; }
			JSONValue operator= (bool b)				{ boolean = b; return *this; }
			JSONValue operator= (JSONObject* o)			{ object = o; return *this; }
			JSONValue operator= (std::vector<JSONValue>* a)		{ array = a; return *this; }
		};

		/* A class that represents a basic key value pair in JSON
		 *
		 * This is mostly a  container class to store data similar to std::pair.
		 * However in the JSON spec data will always be paired 
		 *  with a string as the key so there's no need for templating
		 */
		class JSONPair
		{
		public:
			//Blank Constructor
			inline JSONPair(){}

			/* Constructor if you know your data
			 * @key The string to be this pair's key
			 * @JSONValue The object to be mapped to the key
			 */
			inline JSONPair(char* key, JSONValue value)
			{
				this->key = key;
				this->value = value;
			}

			/* Returns the key of this pair.
			 * @returns The key
			 */
			inline char* getKey(){ return key; }
			/* Returns the value of this pair
			 * @returns The value
			 */
			inline JSONValue getValue(){ return value; }
		private:
			char* key;
			JSONValue value;
		};

		/* A class that represents an object in JSON
		 *
		 * Since there is no generic object in C or C++ one had to be made.
		 * It's essentially a collection of JSONPairs.
		 */
		class JSONObject
		{
		public:
			// Blank Constructor
			inline JSONObject(){}


			/* Get a value out of the object.
			 * @key The string that maps to the value we want to get.
			 *
			 * The given typename will be the type that the resulting data 
			 * will be attempted to be mapped to.
			 *
			 * @return The value at the given key if it exists,
			 *         0 if it doesn't exist.
			 */
			template <typename T> T getValue(char* key)
			{
				for(unsigned int i = 0; i < keyValuePairs.size(); i++)
				{
					JSONPair kvp = keyValuePairs[i];
					char* kvpKey = kvp.getKey();
					if (strcmp(kvpKey, key) == 0)
					{
						return (T)kvp.getValue();
					}
				}
				return 0;
			}

			/* Get the number of pairs in the object.
			 *
			 * @return The count of pairs in the object.
			 */
			inline unsigned int getSize()
			{
				return keyValuePairs.size();
			}

			/* Add a JSONPair to the object.
			 * @pair the JSONPair to be added to the object
			 */
			inline void addPair(JSONPair pair)
			{
				keyValuePairs.push_back(pair);
			}

		private :
			std::vector< JSONPair > keyValuePairs ;

		};

		// The static class that has the methods to decode and encode JSON
		class JSONParser
		{
		public:
			/* Decode JSON from a file
			 * @filename The name of the file that you want to parse Ex. "Data/myfile.json".
			 *
			 * @returns A pointer to the decoded JSONObject or nullptr if the file couldn't be read.
			 */
			inline static JSONObject* DecodeJSONFromFile(const char* filename)
			{
				char* filecontents = StringUtils::textFileRead(filename);
				if (filecontents == nullptr)
				{
					std::cout << "Could not read JSON file" << std::endl;
					return nullptr;
				}

				return DecodeJSONFromString(filecontents);
			}

			/* Decode JSON from a string
			 * @rawString A string in a JSON format that you wnat to decode into a JSONObject
			 *
			 * @returns A pointer to the decoded JSONObject.
			 */
			inline static JSONObject* DecodeJSONFromString(const char* rawString)
			{
				//We want to strip all whitespace from the raw string
				const char* jsonString = StringUtils::trimAllWhitespace(rawString);

				return parseObject(jsonString);
			}

			static void EncodeJSONToFile(const char* filename, JSONObject object);
			static const char* EncodeJSONToString(JSONObject object);
		private:
			inline static JSONValue parseValue(const char* string)
			{
				JSONValue value;

				//Parsing Bools
				if (strcmp(string, "true") == 0)
				{
					value = true;
				}
				else if (strcmp(string, "false") == 0)
				{
					value = false;
				}
				//Parsing strings
				else if (string[0] == '\"')
				{
					int valStringLen = strlen(string) - 1;
					char* valueString = new char[valStringLen];
					memcpy(valueString, string + 1, valStringLen);
					valueString[valStringLen - 1] = '\0';

					value = valueString;
				}
				//Parsing Objects
				else if (string[0] == '{')
				{
					value = parseObject(string);
				}
				//Parsing Arrays
				else if(string[0] == '[')
				{
					value = parseArray(string);
				}
				//Parsing null
				else if (strcmp(string, "null") == 0)
				{
					value = 0;
				}
				//Parsing numbers
				else if (strstr(string, "."))
				{
					float *floatVal = new float;
					*floatVal = (float)atof(string);
					value = *floatVal;
				}
				else
				{
					int* intVal = new int;
					*intVal = atoi(string);
					value = *intVal;
				}

				return value;
			}

			inline static JSONObject* parseObject(const char* string)
			{
				int index = 0;
				int stringLength = strlen(string);
				char c = string[index];

				JSONObject* object = new JSONObject;

				bool parsing = true;

				while (parsing)
				{
					//We've hit a member; determine how long until the next member and then parse it
					if (c == '\"')
					{
						int startIndex = index;
						int memberLength = 0;
						char m = c;
						char targetChar = ',';

						int nestedObjects = 0;
						int nestedArrays = 0;

						while (m != targetChar || nestedObjects > 0 || nestedArrays > 0)
						{
							//If we've read past our string length we can just break
							if (index + memberLength >= stringLength)
							{
								parsing = false;
								break;
							}

							m = string[startIndex + memberLength];							
							
							if (m == '}' && nestedObjects == 0)
								break;

							if (m == '{')
								nestedObjects++;
							else if (m == '}' && nestedObjects > 0)
								nestedObjects--;

							if (m == '[')
								nestedArrays++;
							else if (m == ']' && nestedArrays > 0)
								nestedArrays--;

							memberLength++;
						}

						//If we get a member that's less than 5 chars (by accident) just increment the index counter and keep going
						//5 because the least number of chars for a member is 5
						//Ex. "a":1
						if (memberLength < 5)
						{
							index++;
							continue;
						}
				
						char* memberString = new char[memberLength + 1];
						memcpy(memberString, string + startIndex, memberLength);

						//We MAY want to trim off the last character
						if (memberString[memberLength - 1] == ','
							|| (!strstr(memberString, "{") && memberString[memberLength - 1] == '}'))
							memberString[memberLength - 1] = '\0';
						//Regardless the string needs a null terminator
						else
							memberString[memberLength] = '\0';

						JSONPair member = parseMember(memberString);

						//If the key is never set then there was a problem
						if (member.getKey() != "")
							object->addPair(member);
						else
							std::cout << "Error parsing member in object" << std::endl;
						
						delete[] memberString;

						index += (memberLength - 1);
					}

					c = string[index++];

					//If we've read past our string length we can just break
					if (index >= stringLength)
						parsing = false;
				}

				return object;
			}
			static std::vector<JSONValue>* parseArray(const char* string)
			{
				std::vector<JSONValue>* elements = new std::vector<JSONValue>();

				//Trim the brackets from the array string
				char* trimmedString = new char[strlen(string) - 1];
				memcpy(trimmedString, string + 1, strlen(string));
				trimmedString[strlen(trimmedString) - 1] = '\0';

				//Split
				int stringLength = strlen(trimmedString);
				int index = 0;

				char m = trimmedString[0];
				char targetChar = ',';

				bool parsing = true;

				while (parsing)
				{
					int startIndex = index;
					int valueLength = 0;
					int nestedObjects = 0;
					int nestedArrays = 0;

					while (m != targetChar || nestedObjects > 0 || nestedArrays > 0)
					{
						//If we've read past our string length we can just break
						if (index + valueLength >= stringLength)
						{
							parsing = false;
							break;
						}

						m = trimmedString[index + valueLength];

						if (m == '}' && nestedObjects == 0)
							break;

						if (m == '{')
							nestedObjects++;
						else if (m == '}' && nestedObjects > 0)
							nestedObjects--;

						if (m == '[')
							nestedArrays++;
						else if (m == ']' && nestedArrays > 0)
							nestedArrays--;

						valueLength++;
					}

					if (valueLength == 0)
					{
						index++;
						continue;
					}

					char* valueString = new char[valueLength + 1];
					memcpy(valueString, trimmedString + index, valueLength);

					//We MAY want to trim off the last character
					if (valueString[valueLength - 1] == ','
						|| (!strstr(valueString, "{") && valueString[valueLength - 1] == '}'))
						valueString[valueLength - 1] = '\0';
					//Regardless the string needs a null terminator
					else
						valueString[valueLength] = '\0';

					/*Actually put the parsed value object into the array*/
					elements->push_back(parseValue(valueString));

					//If we've read past our string length we can just break
					if (valueLength >= stringLength)
						parsing = false;

					delete[] valueString;

					index += valueLength;

					m = trimmedString[index];
				}

				return elements;
			}
			inline static JSONPair parseMember(const char* string)
			{
				int stringLength = strlen(string) + 1;
				char* memberString = new char[stringLength];
				memcpy(memberString, string, stringLength);
				memberString[stringLength - 1] = '\0';

				char* key = "";
				JSONValue value;

				//We can't parse the member just yet, we need to know its type
				std::vector<std::string> split = StringUtils::splitOnce(memberString, ":");
				if (split.size() == 2)
				{
					const char* memberKeyString = split[0].c_str();
					const char* memberValueString = split[1].c_str();

					int keyStringLen = strlen(memberKeyString);

					value = parseValue(memberValueString);

					key = new char[keyStringLen];
					memcpy(key, memberKeyString, keyStringLen);
					key[keyStringLen - 1] = '\0';
				}
			
				delete[] memberString;
				return JSONPair(key, value);
			}
		};
	}
}
#endif
