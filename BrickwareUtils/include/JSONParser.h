#ifndef JSON_H
#define JSON_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//DLL Header
#include "BrickwareUtilsDLL.h"

//System level headers
#include <vector>

//Project Headers
#include "StringUtils.h"

using namespace std;

namespace Brickware
{
	namespace Utility
	{		
		class JSONKVP
		{
		public:
			inline JSONKVP(){}
			inline JSONKVP(char* key, void* value)
			{
				this->key = key;
				this->value = value;
			}
			inline char* getKey(){ return key; }
			inline void* getValue(){ return value; }
		private:
			char* key;
			void* value;
		};

		class JSONObject
		{
		public:
			inline JSONObject(){}

			template <typename T> T getValue(char* key)
			{
				for each(JSONKVP kvp in keyValuePairs)
				{
					char* kvpKey = kvp.getKey();
					if (strcmp(kvpKey, key) == 0)
					{
						return (T)kvp.getValue();
					}
				}
				return NULL;
			}

			inline unsigned int getSize()
			{
				return keyValuePairs.size();
			}

			inline void addKVP(JSONKVP kvp)
			{
				keyValuePairs.push_back(kvp);
			}

		private :
			std::vector< JSONKVP > keyValuePairs ;

		};

		class JSONParser
		{
		public:
			inline static JSONObject* DecodeJSONFromFile(const char* filename)
			{
				char* filecontents = StringUtils::textFileRead(filename);
				if (filecontents == NULL)
				{
					std::cout << "Could not read JSON file" << std::endl;
					return new JSONObject();
				}

				return DecodeJSONFromString(filecontents);
			}
			inline static JSONObject* DecodeJSONFromString(const char* rawString)
			{
				//We want to strip all whitespace from the raw string
				const char* jsonString = StringUtils::trimAllWhitespace(rawString);

				return parseObject(jsonString);
			}

			static void EncodeJSONToFile(const char* filename, JSONObject object);
			static const char* EncodeJSONToString(JSONObject object);
		private:
			inline static void* parseType(const char* string)
			{
				void* value;

				//Parsing Bools
				if (strcmp(string, "true") == 0)
				{
					value = (void*)(new bool(1));
				}
				else if (strcmp(string, "false") == 0)
				{
					value = (void*)(new bool(0));
				}
				//Parsing strings
				else if (string[0] == '\"')
				{
					int valStringLen = strlen(string) - 1;
					char* valueString = new char[valStringLen];
					memcpy(valueString, string + 1, valStringLen);
					valueString[valStringLen - 1] = '\0';

					value = (void*)(valueString);
				}
				//Parsing Objects
				else if (string[0] == '{')
				{
					value = (void*)parseObject(string);
				}
				//Parsing Arrays
				else if(string[0] == '[')
				{
					value = (void*)parseArray(string);
				}
				//Parsing numbers
				else if (strstr(string, "."))
				{
					float *floatVal = new float;
					*floatVal = (float)atof(string);
					value = (void*)floatVal;
				}
				else
				{
					int* intVal = new int;
					*intVal = atoi(string);
					value = (void*)intVal;
				}

				return value;
			}

			inline static JSONObject* parseObject(const char* string)
			{
				int index = 0;
				int stringLength = strlen(string);
				char c = string[index];

				JSONObject* object = new JSONObject;

				while (true)
				{
					//We've hit a member; determine how long until the next member and then parse it
					if (c == '\"')
					{
						int startIndex = index;
						int memberLength = 0;
						char m = c;
						char targetChar = ',';
						while (m != targetChar && m != '}')
						{
							m = string[startIndex + memberLength];

							//If we hit an object or array opening we now want to parse until we hit its close
							if (m == '{')
								targetChar = '}';
							
							else if (m == '[')
								targetChar = ']';
							
							memberLength++;
						}

						//If we've read past our string length we can just break
						if (index > stringLength)
							break;

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

						JSONKVP member = parseMember(memberString);

						//If the key is never set then there was a problem
						if (member.getKey() != "")
							object->addKVP(member);
						else
							std::cout << "Error parsing member in object" << std::endl;
						
						delete[] memberString;

						index += (memberLength - 1);
					}

					c = string[index++];
				}

				return object;
			}
			static std::vector<void*>* parseArray(const char* string)
			{
				std::vector<void*>* elements = new std::vector<void*>();

				//Trim the brackets from the array string
				char* trimmedString = new char[strlen(string) - 1];
				memcpy(trimmedString, string + 1, strlen(string));
				trimmedString[strlen(trimmedString) - 1] = '\0';

				//Split
				std::vector<std::string> elementStrings = StringUtils::stringSplit(trimmedString, ",");

				//Parse out the types and place them into elements
				for (unsigned int i = 0; i < elementStrings.size(); i++)
					elements->push_back(parseType(elementStrings[i].c_str()));
				
				return elements;
			}
			inline static JSONKVP parseMember(const char* string)
			{
				int stringLength = strlen(string) + 1;
				char* memberString = new char[stringLength];
				memcpy(memberString, string, stringLength);
				memberString[stringLength - 1] = '\0';

				char* key = "";
				void* value = NULL;

				//We can't parse the member just yet, we need to know its type
				std::vector<std::string> split = StringUtils::splitOnce(memberString, ":");
				if (split.size() == 2)
				{
					const char* memberKeyString = split[0].c_str();
					const char* memberValueString = split[1].c_str();

					int keyStringLen = strlen(memberKeyString);

					value = parseType(memberValueString);

					key = new char[keyStringLen];
					memcpy(key, memberKeyString, keyStringLen);
					key[keyStringLen - 1] = '\0';
				}
			
				delete[] memberString;
				return JSONKVP(key, value);
			}
		};
	}
}
#endif