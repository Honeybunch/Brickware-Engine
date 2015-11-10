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
			friend class XMLElement;
		};

		/*
		 * Represents an element in an XML document
		 * Can have many attributes and can have one value
		*/
		class XMLElement
		{			
			friend class XMLDoc;

		public:
			inline XMLElement(){}

			template<typename T> T getValue(std::string key)
			{
			
			}

		private:
			std::vector<XMLAttribute> attributes;

			inline static XMLElement Parse(const char* elementString)
			{
				return XMLElement();
			}
		};

		/*
		 * Represents an XML Document
		 * Can have many elements
		*/
		class XMLDoc
		{
			friend class XMLParser;

		private:
			std::vector<XMLElement> elements;

			inline static XMLDoc* Parse(const char* documentString)
			{
				int index = 0;
				int length = strlen(documentString);

				//If the first char is not a '<' then it's malformed
				if (documentString[index] != '<')
					return nullptr;

				//Create the XMLDocument
				XMLDoc* doc = new XMLDoc();

				bool searchingForTag = false;
				bool searchingToClose = false;

				char c;
				char nextChar;

				char* searchTag = nullptr;

				int elementStartIndex = -1;
				int tagStartIndex = 0;
				int nestedElement = 0;
				while (index < length)
				{
					c = documentString[index];
					nextChar = documentString[index + 1];

					//If we hit an opening tag, parse the whole tag
					if (c == '<' && !searchingToClose)
					{
						if (elementStartIndex == -1)
							elementStartIndex = index;
						tagStartIndex = index;

						//Check for '>' and ' ' because we want the tag NAME, not the attributes
						while (c != '>')
						{
							c = documentString[index];
							index++;
						}

						int tagLength = index - tagStartIndex - 2;
						char* tag = new char[tagLength];

						memcpy(tag, documentString + tagStartIndex + 1, tagLength);
						tag[tagLength] = '\0';

						if (!searchingForTag)
						{
							//If the tag's last char is a '/', it's a self closing element; parse it
							if (tag[tagLength - 1] == '/')
							{
								searchingToClose = true;
								searchingForTag = false;
							}
							//If not, we need to search to find the matching tag
							else
							{
								searchingForTag = true;
								//We want to search for just the tag name
								int indexEndOfName = 0;
								char marker = tag[indexEndOfName];
								while (marker != '>' && marker != ' ')
								{
									marker = tag[indexEndOfName];
									indexEndOfName++;
								}

								//Create the tag name we want to search for
								int searchTagLength = indexEndOfName - 1;
								searchTag = new char[searchTagLength];
								memcpy(searchTag, tag, searchTagLength);
								searchTag[searchTagLength] = '\0';
							}
						}
						//If we're searching for tag, we're looking for a tag with the same name as the searchTag but starting with an '/'
						else
						{
							//This current tag needs to start with an '/' to even begin to match
							if (tag[0] == '/')
							{
								int testLength = tagLength - 1;

								//Copy string without the '/' to see if it matches
								char* testTag = new char[testLength];
								memcpy(testTag, tag + 1, testLength);
								testTag[testLength] = '\0';

								index--;

								//If it matches, we can search to close
								if (strcmp(testTag, searchTag) == 0)
								{
									searchingToClose = true;
									searchingForTag = false;

									//delete[] searchTag;
								}

								//delete[] testTag;
							}
						}

						//delete[] tag;
					}

					//If we're searching to close, we're parsing to the end of the tag; the next '>'
					if (searchingToClose)
					{
						while (c != '>')
						{
							c = documentString[index];
							index++;
						}

						//Get the string
						int elementLength = index - elementStartIndex;
						char* elementString = new char[elementLength];

						memcpy(elementString, documentString + elementStartIndex, elementLength);
						elementString[elementLength] = '\0';

						//Parse and push back to array
						doc->elements.push_back(XMLElement::Parse(elementString));

						//No long searching to close
						searchingToClose = false;
						elementStartIndex = -1;
					}
					else
					{
						index++;
					}
				}

				return doc;
			}

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
					std::cout << "Could not read XML file" << std::endl;
					return nullptr;
				}

				return DecodeXMLFromString(filecontents);
			}

			inline static XMLDoc* DecodeXMLFromString(const char* xmlString)
			{
				const char* rawXML = StringUtils::trimWhitespaceForXML(xmlString);
				return XMLDoc::Parse(rawXML);
			}		
		};
	}
}

#endif
