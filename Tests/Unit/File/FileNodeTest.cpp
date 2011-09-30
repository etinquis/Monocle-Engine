#include "FileNodeTest.h"
#include <cpptest/cpptest-assert.h>

#include <File/FileNode.h>
#include <Vector2.h>
#include <Color.h>

namespace Monocle
{    
    namespace Test
    {
        namespace Unit
		{

			FileNodeTest::FileNodeTest()
			{
				TEST_ADD(FileNodeTest::Constructor);
				TEST_ADD(FileNodeTest::BuildTree);
				TEST_ADD(FileNodeTest::GetChild);
				TEST_ADD(FileNodeTest::WriteAttributes);
				TEST_ADD(FileNodeTest::ReadAttributes);
			}

			void FileNodeTest::setup()
			{
				node = new FileNode();
			}

			void FileNodeTest::tear_down()
			{
				delete node;
				node = NULL;
			}

			void FileNodeTest::Constructor()
			{
				Monocle::FileNode namedNode("test");
				Monocle::FileNode unnamedNode = FileNode();

				TEST_ASSERT(namedNode.GetName() == "test");
				TEST_ASSERT(unnamedNode.GetName() == "root");
			}

			void FileNodeTest::BuildTree()
			{
				FileNode *comp = node->InsertEndChildNode("Computer");
				
				FileNode *output = comp->InsertEndChildNode("Output");
				FileNode *input = comp->InsertEndChildNode("Input");

				input->InsertEndChildNode("Mouse");
				input->InsertEndChildNode("Keyboard");

				output->InsertEndChildNode("Printer");
				output->InsertEndChildNode("Monitor");
			}

			void FileNodeTest::GetChild()
			{
				FileNode *comp = node->InsertEndChildNode("Computer");
				
				FileNode *output = comp->InsertEndChildNode("Output");
				FileNode *input = comp->InsertEndChildNode("Input");

				input->InsertEndChildNode("Mouse");
				input->InsertEndChildNode("Keyboard");

				output->InsertEndChildNode("Printer");
				output->InsertEndChildNode("Monitor");

				TEST_ASSERT(comp->GetChild("Output")->GetChild("Printer"))
				TEST_ASSERT(comp->GetChild("Input")->GetChild("Keyboard"))

				TEST_ASSERT(comp->GetChild("Nonexistant") == NULL)
			}

			void FileNodeTest::WriteAttributes()
			{
				FileNode *comp = node->InsertEndChildNode("Computer");
				
				FileNode *monitor = comp->InsertEndChildNode("Output")->InsertEndChildNode("Monitor");
				monitor->Write("model", "Widget Co. T86");
				monitor->Write("age", 2);
				monitor->Write("dimensions", Monocle::Vector2(1900, 1080));
				monitor->Write("bezelcolor", Monocle::Color::black);

				std::string model;
				monitor->Read("model", model);
				int age;
				monitor->Read("age", age);
				Monocle::Vector2 dimensions;
				monitor->Read("dimensions", dimensions);
				Monocle::Color bezel;
				monitor->Read("bezelcolor", bezel);

				TEST_ASSERT(model == "Widget Co. T86")
				TEST_ASSERT(age == 2)
				TEST_ASSERT(dimensions == Monocle::Vector2(1900, 1080))
				TEST_ASSERT(bezel == Monocle::Color::black)
			}

			void FileNodeTest::ReadAttributes()
			{
				FileNode *comp = node->InsertEndChildNode("Computer");
				
				FileNode *monitor = comp->InsertEndChildNode("Output")->InsertEndChildNode("Monitor");
				monitor->Write("model", "Widget Co. T86");
				monitor->Write("age", 2);
				monitor->Write("dimensions", Monocle::Vector2(1900, 1080));
				monitor->Write("bezelcolor", Monocle::Color::black);

				std::string model;
				monitor->Read("model", model);
				int age;
				monitor->Read("age", age);
				Monocle::Vector2 dimensions;
				monitor->Read("dimensions", dimensions);
				Monocle::Color bezel;
				monitor->Read("bezelcolor", bezel);
				int numberoflegs = 0xDEADBEEF;
				monitor->Read("numberoflegs", numberoflegs);
				Monocle::Color legcolor = Monocle::Color::green;
				monitor->Read("legcolor", legcolor);

				TEST_ASSERT(model == "Widget Co. T86")
				TEST_ASSERT(age == 2)
				TEST_ASSERT(dimensions == Monocle::Vector2(1900, 1080))
				TEST_ASSERT(bezel == Monocle::Color::black)
				TEST_ASSERT(numberoflegs == 0xDEADBEEF)

				TEST_ASSERT(legcolor == Monocle::Color::green);
			}
        }
    }
}
