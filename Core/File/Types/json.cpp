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

		void json::WriteTo(std::ostream &os, FileNode *node)
		{
			Json::Value root(Json::objectValue);
			WriteNode(&root, node);

			Json::StyledWriter writer;
			
			os << writer.write(root);
		}

		void json::WriteNode(Json::Value *element, FileNode *node)
		{
			 for(FileNode::AttributeList::iterator it = node->attributes.begin(); it != node->attributes.end(); it++)
			 {
			 	//Json::Value attrib(it->first);
			 	//attrib.append(it->second);
			 	Json::Value value = Json::Value(it->second);

			 	(*element)[it->first] = value;
			 }

			 for(FileNode::ChildList::iterator it = node->children.begin(); it != node->children.end(); it++)
			 {
			 	Json::Value *child = new Json::Value();
			 	WriteNode(child, &(*it));
			 	(*element)[it->GetName()] = *child;
			 }
		}

		void json::ReadFrom(std::istream &is, FileNode *root)
		{
			 Json::Reader reader;
			 Json::Value jroot;

			 std::string line;

			 std::ostringstream lines;
			
			 while(!is.eof())
			 {
			 	std::getline(is, line);
			 	lines << line;
			 }
			
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