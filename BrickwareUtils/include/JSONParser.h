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

			template <typename T = char*> void* getValue(char* key)
			{
				for each(JSONKVP kvp in keyValuePairs)
				{
					if (strcmp(kvp.getKey(), key))
						return (T*)kvp.getValue();
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
						while (m != ',')
						{
							m = string[startIndex + memberLength];
							memberLength++;
						}

						char* memberString = new char[memberLength];
						memcpy(memberString, string + startIndex, memberLength);

						JSONKVP member = parseMember(memberString);

						if (member.getKey() != "")
						{
							object.addKVP(member);
						}
						else
						{
							std::cout << "Error parsing member in object" << std::endl;
						}

						index += (memberLength);

						delete memberString;
					}

					c = string[++index];
				}

				return object;
			}
			template<typename T> static std::vector<T> parseArray(char* string)
			{
			
			}
			inline static JSONKVP parseMember(char* string)
			{
				//We can't parse the member just yet, we need to know its type
				std::vector<char*> split = StringUtils::stringSplit(string, ":");
				if (split.size() == 2)
				{
					char* memberKeyString = split[0];
					char* memberValueString = split[1];

					if (strcmp(memberValueString, "true"))
						return JSONKVP(memberKeyString, (void*)(new bool(1)));
					else if (strcmp(memberValueString, "false"))
						return JSONKVP(memberKeyString, (void*)(new bool(0)));
					else
						return JSONKVP("", NULL);
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
