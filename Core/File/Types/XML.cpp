#include "XML.h"
#include "../FileNode.h"
#include <tinyxml/tinyxml.h>

namespace Monocle
{
	namespace FileType
	{
		XML::XML()
		{

		}

		void XML::WriteToFile(const std::string &filename, FileNode *node)
		{
			TiXmlDocument doc = TiXmlDocument();
			TiXmlElement *element = new TiXmlElement(node->GetName());

			for(FileNode::ChildList::iterator it = node->children.begin(); it != node->children.end(); it++)
			{
				TiXmlElement *child = new TiXmlElement(it->first);
				WriteNode(child, &it->second);
				doc.LinkEndChild(child);
			}

			doc.SaveFile(filename);
		}

		void XML::WriteNode(TiXmlElement *element, FileNode *node)
		{
			for(FileNode::AttributeList::iterator it = node->attributes.begin(); it != node->attributes.end(); it++)
			{
				element->SetAttribute(it->first, it->second);
			}

			for(FileNode::ChildList::iterator it = node->children.begin(); it != node->children.end(); it++)
			{
				TiXmlElement *child = new TiXmlElement(it->first);
				WriteNode(child, &it->second);
				element->LinkEndChild(child);
			}
		}

		void XML::ReadFromFile(const std::string &filename, FileNode *root)
		{
			TiXmlDocument doc(filename);
			if( doc.LoadFile() )
			{
				TiXmlElement *child = doc.FirstChildElement();
				while(child)
				{
					FileNode *childNode = root->InsertEndChildNode(child->Value());
					ReadNode(child, childNode);
					child = child->NextSiblingElement();
				}
			}
		}

		void XML::ReadNode(TiXmlElement *element, FileNode *node)
		{
			TiXmlAttribute *attrib = element->FirstAttribute();
			while(attrib)
			{
				node->Write(attrib->Name(), attrib->Value());
				attrib = attrib->Next();
			}

			TiXmlElement *child = element->FirstChildElement();
			while(child)
			{
				FileNode *childNode = node->InsertEndChildNode(child->Value());
				ReadNode(child, childNode);
				child = child->NextSiblingElement();
			}
		}
	}
	//XMLFileNode::XMLFileNode()
	//	: element(NULL), iterator(NULL)
	//{

	//}

	//XMLFileNode::XMLFileNode(TiXmlElement *element)
	//	: element(element), iterator(NULL)
	//{

	//}

	//XMLFileNode::~XMLFileNode()
	//{
	//	if (iterator)
	//	{
	//		delete iterator;
	//		iterator = NULL;
	//	}
	//}
	//
	//FileNode* XMLFileNode::FirstChildNode(const std::string &name)
	//{
	//	if (!iterator)
	//		iterator = new XMLFileNode(element->FirstChildElement(name));
	//	else
	//		iterator->element = element->FirstChildElement(name);

	//	if (iterator != NULL && iterator->element == NULL)
	//	{
	//		delete iterator;
	//		iterator = NULL;
	//	}

	//	return iterator;
	//}
	//
	//FileNode* XMLFileNode::NextChildNode(const std::string &name)
	//{
	//	// if we have an iterator
	//	if (iterator)
	//	{
	//		// if we have an element
	//		if (iterator->element)
	//		{
	//			// get the next element
	//			iterator->element = iterator->element->NextSiblingElement(name);
	//		}
	//		
	//		// if it's element is NULL
	//		if (iterator->element == NULL)
	//		{
	//			// delete it
	//			delete iterator;
	//			iterator = NULL;
	//		}
	//	}
	//		
	//	return iterator;
	//}

	////FileNode* NewNode(const std::string &name)
	////{
	////	XMLFileNode *xmlFileNode = new XMLFileNode();
	////	xmlFileNode->managed = new TiXmlElement(name);
	////}

	////FileNode* XMLFileNode::InsertEndChildNode(FileNode *fileNode)
	////{
	////	XMLFileNode *xmlFileNode = (XMLFileNode*)fileNode;
	////	xmlFileNode->element->InsertEndChild(*xmlFileNode->element);
	////}

	///// WRITE
	//void XMLFileNode::Write(const std::string &name, const int &value)
	//{
	//	element->SetAttribute(name, value);
	//}

	//void XMLFileNode::Write(const std::string &name, const float &value)
	//{
	//	std::ostringstream os;
	//	os << value;
	//	element->SetAttribute(name, os.str());
	//	//element->SetAttribute(name, value);
	//}

	//void XMLFileNode::Write(const std::string &name, const std::string &value)
	//{
	//	element->SetAttribute(name, value);
	//}

	//void XMLFileNode::Write(const std::string &name, const Vector2 &value)
	//{
	//	std::ostringstream os;
	//	os << value.x << " " << value.y;
	//	element->SetAttribute(name, os.str());
	//}

	//void XMLFileNode::Write(const std::string &name, const Color &value)
	//{
	//	std::ostringstream os;
	//	os << value.r << " " << value.g << " " << value.b << " " << value.a;
	//	element->SetAttribute(name, os.str());
	//}

	//void XMLFileNode::Write(const std::string &name, const bool &value)
	//{
	//	element->SetAttribute(name, (int)value);
	//}


	///// READ
	//void XMLFileNode::Read(const std::string &name, int &value)
	//{
	//	if (element->Attribute(name))
	//	{
	//		std::istringstream read(*element->Attribute(name));
	//		read >> value;
	//	}
	//}

	//void XMLFileNode::Read(const std::string &name, float &value)
	//{
	//	if (element->Attribute(name))
	//	{
	//		std::istringstream read(*element->Attribute(name));
	//		read >> value;
	//	}
	//}

	//void XMLFileNode::Read(const std::string &name, std::string &value)
	//{
	//	if (element->Attribute(name))
	//	{
	//		value = *element->Attribute(name);
	//	}
	//}

	//void XMLFileNode::Read(const std::string &name, Vector2 &value)
	//{
	//	if (element->Attribute(name))
	//	{
	//		std::istringstream read(*element->Attribute(name));
	//		read >> value.x >> value.y;
	//	}
	//}

	//void XMLFileNode::Read(const std::string &name, Color &value)
	//{
	//	if (element->Attribute(name))
	//	{
	//		std::string string = *element->Attribute(name);
	//		std::istringstream read(string);
	//		if (string.find('#') != std::string::npos)
	//		{
	//			// TODO: read hex color
	//		}
	//		else
	//		{
	//			read >> value.r >> value.g >> value.b >> value.a;
	//		}
	//	}
	//}

	//void XMLFileNode::Read(const std::string &name, bool &value)
	//{
	//	if (element->Attribute(name))
	//	{
	//		std::string string = *element->Attribute(name);
	//		std::istringstream read(string);
	//		read >> value;
	//	}
	//}


	//void XMLFileNode::SaveChildNode(const std::string &name, Entity *entity)
	//{
	//	TiXmlElement newElement(name);
	//	XMLFileNode xmlFileNode(&newElement);
	//	entity->Save(&xmlFileNode);
	//	element->InsertEndChild(newElement);
	//}
}
