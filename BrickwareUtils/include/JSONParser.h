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
		template <typename T = char*> class BRICKWARE_UTILITY_API JSONKVP
		{
		public:
			JSONKVP(){}
			JSONKVP(char* key, T value)
			{
				this->key = key;
				this->value = value;
			}
			char* getKey(){ return key; }
			T getValue(){ return value; }
		private:
			char* key;
			T value;
		};

		template class BRICKWARE_UTILITY_API std::vector < JSONKVP<> >;

		class BRICKWARE_UTILITY_API JSONObject
		{
		public:
			inline JSONObject(){}

			inline void* getValue(char* key)
			{
				for each(JSONKVP<> kvp in keyValuePairs)
				{
					if (kvp.getKey() == key)
						return kvp.getValue();
				}

				return NULL;
			}

			inline unsigned int getSize()
			{
				return keyValuePairs.size();
			}

			template <typename T = char*> void addKVP(char* key, T value)
			{
				keyValuePairs.push_back(JSONKVP<T>(key, value));
			}

		private :
			std::vector < JSONKVP<> > keyValuePairs ;
		};

		class BRICKWARE_UTILITY_API JSONParser
		{
		public:
			static JSONObject DecodeJSONFromFile(char* filename);
			static JSONObject DecodeJSONFromString(char* rawString);

			static void EncodeJSONToFile(char* filename, JSONObject object);
			static char* EncodeJSONToString(JSONObject object);
		private:
			
		};
	}
}
#endif
