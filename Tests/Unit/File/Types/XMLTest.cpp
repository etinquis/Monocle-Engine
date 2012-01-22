#include "XMLTest.h"
#include "types.h"

#include <cpptest/cpptest-assert.h>

#include <File/FileNode.h>
#include <Vector2.h>
#include <Color.h>
#include <File/Types/XML.h>
#include <File/File.h>
#include <Platform.h>

namespace Monocle
{    
    namespace Test
    {
        namespace Unit
		{

			XMLFileTest::XMLFileTest()
			{
				TEST_ADD(XMLFileTest::Read);
				TEST_ADD(XMLFileTest::Write);
			}

			void XMLFileTest::setup()
			{

			}

			void XMLFileTest::tear_down()
			{

			}

			void XMLFileTest::Read()
			{
				File<FileType::XML> f(Monocle::Platform::GetDefaultContentPath() + "test.xml");
				f.Load();

				TEST_ASSERT( f.GetRootNode()->GetChild("Cat") )
					
				TEST_ASSERT( f.GetRootNode()->GetChild("Computer") )
				TEST_ASSERT( f.GetRootNode()->GetChild("Computer")->GetChild("Output")->GetChild("Monitor") )
			}

			void XMLFileTest::Write()
			{
				WriteTest(File<FileType::XML>(Monocle::Platform::GetDefaultContentPath() + "test.xml"));
				/*File<FileType::XML> f(Monocle::Platform::GetDefaultContentPath() + "test.xml");
				FileNode *comp = f.GetRootNode()->InsertEndChildNode("Computer");
				f.GetRootNode()->InsertEndChildNode("Cat")->Write("color", Monocle::Color::black);
				
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

				f.Save();*/
			}
        }
    }
}
