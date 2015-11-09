/*
* XMLParser.hpp
*
* A simple header only XML parser
* This depends on the static StringUtils class
*/

#ifndef XML_PARSER_H
#define XML_PARSER_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//System level headers
#include <vector>

//Project Headers
#include "BrickwareUtils/StringUtils.hpp"

namespace Brickware
{
	namespace Utility
	{
		/*
		 * Represents an attribute of an XML Element
		*/
		class XMLAttribute
		{

		};

		/*
		 * Represents an element in an XML document
		 * Can have many attributes and can have one value
		*/
		class XMLElement
		{			
		public:
			inline XMLElement(){}

			template<typename T> T getValue(std::string key)
			{
			
			}

		private:
			std::vector<XMLAttribute> attributes;
		};

		/*
		 * Represents an XML Document
		 * Can have many elements
		*/
		class XMLDoc
		{
			friend class XMLParser;

		public:

		private:
			std::vector<XMLElement> elements;
		};

		//Contains static methods for parsing XML from files and strings
		class XMLParser
		{
		public:
			inline static XMLDoc* DecodeXMLFromFile(const char* filename)
			{
				char* filecontents = StringUtils::textFileRead(filename);
				if (filecontents == nullptr)
				{
					std::cout << "Could not read JSON file" << std::endl;
					return nullptr;
				}

				return DecodeXMLFromString(filecontents);
			}

			inline static XMLDoc* DecodeXMLFromString(const char* xmlString)
			{
				const char* rawXML = StringUtils::trimAllWhitespace(xmlString);
				return parseDocument(rawXML);
			}		

		private:
			inline static XMLDoc* parseDocument(const char* documentString)
			{
				int index = 0;
				int length = strlen(documentString);
				
				//If the first char is not a '<' then it's malformed
				if (documentString[index] != '<')
					return nullptr;

				//Create the XMLDocument
				XMLDoc* doc = new XMLDoc();

				bool searching = false;
				int elementStartIndex;
				int nestedElement = 0;
				while (index < length)
				{
					char c = documentString[index];

					if (c == '<')
					{
						//When we hit an '<', parse until we find the closing '>'
						if (!searching)
						{
							searching = true;
							elementStartIndex = index;
							index++;
							continue;
						}

						//If we hit an '<' and we already started searching, take note
						else
						{
							nestedElement++;
							index++;
							continue;
						}
					}

					else if (c == '>')
					{
						//If we hit a '>' and we weren't searching for one, this is malformed
						if (!searching)
						{
							return nullptr;
						}
						else
						{
							//If we were looking for an '>' but we hit an extra '<' beforehand, decrement the nestedElement counter
							if (nestedElement > 0)
							{
								nestedElement--;
								index++;
								continue;
							}
							//If we hit an '>' and we weren't in a nestedElement, parse the element and add it to the collection
							else
							{
								index++;
								int elementStringLength = index - elementStartIndex;
								char* elementString = new char[elementStringLength + 1];

								//Copy element data from the documentString into the elementString
								memcpy(elementString, documentString + elementStartIndex, elementStringLength);
								elementString[elementStringLength] = '\0';

								int i = 0;

								//XMLElement element = parseElement(elementString);
								//doc->elements.push_back(element);

								//Done searching!
								searching = false;
							}
						}
					}
					else
					{
						//Don't care about this char, increment the index
						index++;
					}
				}

				return doc;
			}

			inline static XMLElement parseElement(const char* elementString)
			{
				return XMLElement();
			}
		};


	}
}

#endif
