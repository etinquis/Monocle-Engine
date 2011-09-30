#include "json.h"
#include "../FileNode.h"
#include <json/json.h>
#include <fstream>

namespace Monocle
{
	namespace FileType
	{
		json::json()
		{

		}

		void json::WriteToFile(const std::string &filename, FileNode *node)
		{
			Json::Value root(Json::ValueType::objectValue);
			WriteNode(&root, node);

			Json::StyledWriter writer = Json::StyledWriter();
			std::ofstream fout(filename);
			fout << writer.write(root);
			fout.close();
		}

		void json::WriteNode(Json::Value *element, FileNode *node)
		{
			for(FileNode::AttributeList::iterator it = node->attributes.begin(); it != node->attributes.end(); it++)
			{
				//Json::Value attrib(it->first);
				//attrib.append(it->second);
				Json::StaticString name(it->first.c_str());
				Json::Value value = Json::Value(it->second);

				(*element)[name] = value;
			}

			for(FileNode::ChildList::iterator it = node->children.begin(); it != node->children.end(); it++)
			{
				Json::Value *child = new Json::Value();
				WriteNode(child, &it->second);
				Json::StaticString name(it->first.c_str());
				(*element)[name] = *child;
			}
		}

		void json::ReadFromFile(const std::string &filename, FileNode *root)
		{
			Json::Reader reader = Json::Reader();
			Json::Value jroot;

			std::ifstream fin(filename);
			std::string line;

			std::ostringstream lines = std::ostringstream();
			
			while(!fin.eof())
			{
				std::getline(fin, line);
				lines << line;
			}
			fin.close();
			
			if(reader.parse(lines.str(), jroot))
			{
				ReadNode(&jroot, root);
			}
			else
			{
				std::cout  << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
				return;
			}

		}

		void json::ReadNode(Json::Value *element, FileNode *node)
		{
			Json::Value::Members members = element->getMemberNames();
			for(Json::Value::Members::iterator it = members.begin(); it != members.end(); it++)
			{
				if( (*element)[*it].isObject() )
				{
					FileNode *childNode = node->InsertEndChildNode( *it );
					ReadNode( &(*element)[*it], childNode );
				}
				else if( (*element)[*it].isString() )
				{
					node->Write( *it, (*element)[*it].asString() );
				}
			}
		}
	}
}