#pragma once

#ifndef MONOCLE_GUI_BERKELIUMJSFUNCTIONBIND
#define MONOCLE_GUI_BERKELIUMJSFUNCTIONBIND

#include <vector>
#include <map>
#include <string>

#include <berkelium/ScriptVariant.hpp>

#include <iostream>

namespace Monocle
{
    namespace GUI
    {
        namespace JSBinding
        {
            class JSFunctionCallback
            {
			public:
            	std::wstring name;
            	//std::vector<Berkelium::Script::Variant::Type> argTypes;
            	char numArgs;
            	void (*func)(const std::vector<Berkelium::Script::Variant>& arguments);
            	
            	JSFunctionCallback(void (*f)(const std::vector<Berkelium::Script::Variant>& arguments), std::wstring name, int numArgs)
            	{
            		func = f;
            		this->name = name;
            		this->numArgs = numArgs;
            	}
            	
            	virtual void operator ()(const std::vector<Berkelium::Script::Variant>& arguments) { func(arguments); };
            	
            	bool operator<(const JSFunctionCallback& other)
            	{
            		if(name == other.name)
            		{
            			return numArgs < other.numArgs;
            		}else{
						return name < other.name;
            		}
            	}
            };
        }
    }
}

#endif
