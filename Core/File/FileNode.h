#pragma once

#include <string>
#include <list>
#include <map>
#include <unordered_map>
#include <sstream>

namespace Monocle
{
	class Vector2;
	class Color;
	class Entity;

	class FileNode
	{
	public:
		typedef std::map<std::string, std::string> AttributeList;
		typedef std::unordered_map<std::string, FileNode> ChildList;

		//! Creates an unnamed root node
		FileNode();
		//! Creates a named root node
		FileNode(const std::string &name);

		template <typename T>
		void Write(const std::string &name, const T &value)
		{
			std::ostringstream os;
			os << value;
			attributes[name] = os.str();
		}

		template <typename T>
		void Read(const std::string &name, T &value, std::string ordefault = "")
		{
			std::map<std::string, std::string>::iterator it = attributes.find(name);

			if(it != attributes.end())
			{
				std::istringstream is(it->second);
				is >> value;
			}
			else
			{
				std::istringstream is(ordefault);
				is >> value;
			}
		}

		void Read(const std::string &name, std::string &value, std::string ordefault = "")
		{
			std::map<std::string, std::string>::iterator it = attributes.find(name);

			if(it != attributes.end())
			{
				value = it->second;
			}
			else
			{
				value = ordefault;
			}
		}

		//virtual void Read(const std::string &name, int &value)=0;
		//virtual void Read(const std::string &name, float &value)=0;
		//virtual void Read(const std::string &name, std::string &value)=0;
		//virtual void Read(const std::string &name, Vector2 &value);
		//virtual void Read(const std::string &name, Color &value)=0;
		//virtual void Read(const std::string &name, bool &value)=0;

		//virtual FileNode* FirstChildNode(const std::string &name)=0;
		//virtual FileNode* NextChildNode(const std::string &name)=0;
		
		std::string GetName();

		//virtual void SaveChildNode(const std::string &name, Entity *entity)=0;
		//virtual FileNode* NewNode(const std::string &name)=0;
		virtual FileNode *InsertEndChildNode(const std::string &name);
		virtual FileNode *GetChild(const std::string &name);

		bool HasChildren();

		AttributeList attributes;
		ChildList children;
	private:
		std::string name;
	};
}
