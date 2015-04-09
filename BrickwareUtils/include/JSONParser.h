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
		class BRICKWARE_UTILITY_API JSONKVP
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

		template class BRICKWARE_UTILITY_API std::vector < JSONKVP >;

		class BRICKWARE_UTILITY_API JSONObject
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

		class BRICKWARE_UTILITY_API JSONParser
		{
		public:
			static JSONObject* DecodeJSONFromFile(const char* filename);
			static JSONObject* DecodeJSONFromString(const char* rawString);

			static void EncodeJSONToFile(const char* filename, JSONObject object);
			static const char* EncodeJSONToString(JSONObject object);
		private:
			inline static JSONObject* parseObject(const char* string)
			{
				int index = 0;
				int stringLength = strlen(string);
				char c = string[index];
				bool pointerIsNested = false;

				JSONObject* object = new JSONObject;

				while (c != '}' && !pointerIsNested)
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

							if (m == '{')
								targetChar = '}';
							else if (m == '[')
								targetChar = ']';

							memberLength++;
						}

						if (index > stringLength)
							break;

						if (memberLength < 5)
						{
							index++;
							continue;
						}
				
						char* memberString = new char[memberLength + 1];
						memcpy(memberString, string + startIndex, memberLength);

						if (memberString[memberLength - 1] == ','
							|| (!strstr(memberString, "{") && memberString[memberLength - 1] == '}'))
							memberString[memberLength - 1] = '\0';
						else
							memberString[memberLength] = '\0';

						JSONKVP member = parseMember(memberString);

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
			template<typename T> static std::vector<T> parseArray(char* string)
			{
			
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

					//Parsing Bools
					if (strcmp(memberValueString, "true") == 0)
					{
						value = (void*)(new bool(1));
					}
					else if (strcmp(memberValueString, "false") == 0)
					{
						value = (void*)(new bool(0));
					}
					//Parsing strings
					else if (memberValueString[0] == '\"')
					{
						int valStringLen = strlen(memberValueString) - 1;
						char* valueString = new char[valStringLen];
						memcpy(valueString, memberValueString + 1, valStringLen);
						valueString[valStringLen - 1] = '\0';

						value = (void*)(valueString);
					}
					//Parsing Objects
					else if (memberValueString[0] == '{')
					{
						value = (void*)parseObject(memberValueString);
					}
					//Parsing numbers
					else if (strstr(memberValueString, "."))
					{
						float *floatVal = new float;
						*floatVal = (float)atof(memberValueString);
						value = (void*)floatVal;
					}
					else
					{
						int* intVal = new int;
						*intVal = atoi(memberValueString);
						value = (void*)intVal;
					}

					if (value != NULL)
					{
						key = new char[keyStringLen];
						memcpy(key, memberKeyString, keyStringLen);
						key[keyStringLen - 1] = '\0';
					}
				}
			
				delete[] memberString;
				return JSONKVP(key, value);
			}
		};
	}
}
#endif
