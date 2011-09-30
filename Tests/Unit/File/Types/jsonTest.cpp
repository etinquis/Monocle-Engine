#include "jsonTest.h"
#include "types.h"
#include <cpptest/cpptest-assert.h>

#include <File/FileNode.h>
#include <Vector2.h>
#include <Color.h>
#include <File/Types/json.h>
#include <File/File.h>
#include <Platform.h>

namespace Monocle
{    
    namespace Test
    {
        namespace Unit
		{
			jsonFileTest::jsonFileTest()
			{
				TEST_ADD(jsonFileTest::Read);
				TEST_ADD(jsonFileTest::Write);
			}

			void jsonFileTest::setup()
			{

			}

			void jsonFileTest::tear_down()
			{

			}

			void jsonFileTest::Read()
			{
				File<FileType::json> f(Monocle::Platform::GetDefaultContentPath() + "test.json");
				f.Load();

				TEST_ASSERT( f.GetRootNode()->GetChild("Cat") )
					
				TEST_ASSERT( f.GetRootNode()->GetChild("Computer") )
				TEST_ASSERT( f.GetRootNode()->GetChild("Computer")->GetChild("Output")->GetChild("Monitor") )

				bool debug = false;
				f.GetRootNode()->GetChild("Computer")->GetChild("Output")->Read("debug", debug);

				TEST_ASSERT( debug );
			}

			void jsonFileTest::Write()
			{
				WriteTest(File<FileType::json>(Monocle::Platform::GetDefaultContentPath() + "test.json"));
				/*File<FileType::json> f(Monocle::Platform::GetDefaultContentPath() + "test.json");
				FileNode *comp = f.GetRootNode()->InsertEndChildNode("Computer");
				f.GetRootNode()->InsertEndChildNode("Cat")->Write("color", Monocle::Color::black);
				
				FileNode *monitor = comp->InsertEndChildNode("Output")->InsertEndChildNode("Monitor");
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

				f.Save();*/
			}
        }
    }
}
