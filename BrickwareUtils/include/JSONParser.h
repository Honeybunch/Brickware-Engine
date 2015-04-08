#ifndef JSON_H
#define JSON_H

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
			static JSONObject DecodeJSONFromFile(char* filename);
			static JSONObject DecodeJSONFromString(char* rawString);

			static void EncodeJSONToFile(char* filename, JSONObject object);
			static char* EncodeJSONToString(JSONObject object);
		private:
			inline static JSONObject parseObject(char* string)
			{
				int index = 0;
				char c = string[index];
				bool pointerIsNested = false;

				JSONObject object;

				while (c != '}' && !pointerIsNested)
				{
					//We've hit a member; determine how long until the next member and then parse it
					if (c == '\"')
					{
						int startIndex = index;
						int memberLength = 0;
						char m = c;
						while (m != ',' && m != '}')
						{
							m = string[startIndex + memberLength];
							memberLength++;
						}

						if (memberLength > 2)
							index += (memberLength - 2);
						else
							break;

						char* memberString = new char[memberLength];
						memcpy(memberString, string + startIndex, memberLength);

						bool lastMember = false;

						memberString[memberLength - 1] = '\0';

						JSONKVP member = parseMember(memberString);

						if (member.getKey() != "")
						{
							object.addKVP(member);
						}
						else
						{
							std::cout << "Error parsing member in object" << std::endl;
						}

						delete memberString;
					}

					c = string[index++];
				}

				return object;
			}
			template<typename T> static std::vector<T> parseArray(char* string)
			{
			
			}
			inline static JSONKVP parseMember(char* string)
			{
				//We can't parse the member just yet, we need to know its type
				std::vector<std::string > split = StringUtils::stringSplit(string, ":");
				if (split.size() == 2)
				{
					const char* memberKeyString = split[0].c_str();
					const char* memberValueString = split[1].c_str();

					char* key = "";
					void* value = NULL;

					int keyStringLen = strlen(memberKeyString);

					if (strcmp(memberValueString, "true") == 0)
					{
						key = new char[keyStringLen];
						memcpy(key, memberKeyString, keyStringLen);
						key[keyStringLen - 1] = '\0';

						value = (void*)(new bool(1));
					}
					else if (strcmp(memberValueString, "false") == 0)
					{
						key = new char[keyStringLen];
						memcpy(key, memberKeyString, keyStringLen);
						key[keyStringLen - 1] = '\0';

						value = (void*)(new bool(0));
					}
					else if (memberValueString[0] == '\"')
					{
						key = new char[keyStringLen];
						memcpy(key, memberKeyString, keyStringLen);
						key[keyStringLen - 1] = '\0';

						int valStringLen = strlen(memberValueString) - 1;
						char* valueString = new char[valStringLen];
						memcpy(valueString, memberValueString + 1, valStringLen);
						valueString[valStringLen - 1] = '\0';

						value = (void*)(valueString);
					}

					return JSONKVP(key, value);
				}
				else
				{
					return JSONKVP("", NULL);
				}
			}
		};
	}
}
#endif
