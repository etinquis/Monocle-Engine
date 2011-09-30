#pragma once

#include <File/File.h>
#include <File/FileNode.h>

template <typename T>
void WriteTest( Monocle::File<T> &file )
{
	Monocle::FileNode *comp = file.GetRootNode()->InsertEndChildNode("Computer");
	file.GetRootNode()->InsertEndChildNode("Cat")->Write("color", Monocle::Color::black);
				
	Monocle::FileNode *monitor = comp->InsertEndChildNode("Output")->InsertEndChildNode("Monitor");
	monitor->Write("model", "Widget Co. T86");
	monitor->Write("age", 2);
	monitor->Write("dimensions", Monocle::Vector2(1900, 1080));
	monitor->Write("bezelcolor", Monocle::Color::black);

	comp->GetChild("Output")->Write("debug", true);

	std::string model;
	monitor->Read("model", model);
	int age;
	monitor->Read("age", age);
	Monocle::Vector2 dimensions;
	monitor->Read("dimensions", dimensions);
	Monocle::Color bezel;
	monitor->Read("bezelcolor", bezel);
	
	file.Save();
}