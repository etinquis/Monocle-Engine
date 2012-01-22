#pragma once

#include <cpptest/cpptest-suite.h>

#include <File/File.h>
#include <File/FileNode.h>

#include <Platform.h>
#include <Assets.h>
#include <Color.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {
			template <typename FType>
            class FileTypeTest: public ::Test::Suite
            {
            public:
                FileTypeTest(std::string testfilename)
				{
					testpath = Monocle::Platform::GetDefaultContentPath() + testfilename;
					TEST_ADD(FileTypeTest<FType>::WriteAndRead);
				}
                
            protected:
                virtual void setup()
				{

				}
                virtual void tear_down()
				{

				}
                
            private:
				void WriteAndRead()
				{
					std::string path = Monocle::Platform::GetDefaultContentPath() + "test.out";

					Monocle::File<FType> file(path);

					Monocle::FileNode *comp = file.GetRootNode()->InsertEndChildNode("Computer");
					file.GetRootNode()->InsertEndChildNode("Cat")->Write("color", Monocle::Color::black);
				
					Monocle::FileNode *monitor = comp->InsertEndChildNode("Output")->InsertEndChildNode("Monitor");
					monitor->Write("model", "Widget Co. T86");
					monitor->Write("age", 2);
					monitor->Write("dimensions", Monocle::Vector2(1900, 1080));
					monitor->Write("bezelcolor", Monocle::Color::black);

					comp->GetChild("Output")->Write("debug", true);
	
					file.Save();

					File<FType> file2(path);
					Monocle::File<FType> ftest(testpath);

					file2.Load();
					ftest.Load();

					TEST_ASSERT( file2.GetRootNode()->GetChild("Cat") )
					
					TEST_ASSERT( file2.GetRootNode()->GetChild("Computer") )
					TEST_ASSERT( file2.GetRootNode()->GetChild("Computer")->GetChild("Output")->GetChild("Monitor") )

					bool debug = false;
					file2.GetRootNode()->GetChild("Computer")->GetChild("Output")->Read("debug", debug);

					TEST_ASSERT( debug );

					TEST_ASSERT(ftest == file2);
				}

			private:
				std::string testpath;
            };

        }
    }
}
