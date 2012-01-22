//#include "jsonTest.h"
//#include "types.h"
//#include <cpptest/cpptest-assert.h>
//
//#include <File/FileNode.h>
//#include <Vector2.h>
//#include <Color.h>
//#include <File/Types/json.h>
//#include <File/File.h>
//#include <Platform.h>
//
//namespace Monocle
//{    
//    namespace Test
//    {
//        namespace Unit
//		{
//			jsonFileTest::jsonFileTest()
//			{
//				TEST_ADD(jsonFileTest::WriteAndRead);
//			}
//
//			void jsonFileTest::setup()
//			{
//
//			}
//
//			void jsonFileTest::tear_down()
//			{
//
//			}
//
//			void jsonFileTest::WriteAndRead()
//			{
//				std::string path = Monocle::Platform::GetDefaultContentPath() + "test.json";
//
//				WriteTest(File<FileType::json>(path));
//
//				File<FileType::json> f(path);
//				f.Load();
//
//				TEST_ASSERT( f.GetRootNode()->GetChild("Cat") )
//					
//				TEST_ASSERT( f.GetRootNode()->GetChild("Computer") )
//				TEST_ASSERT( f.GetRootNode()->GetChild("Computer")->GetChild("Output")->GetChild("Monitor") )
//
//				bool debug = false;
//				f.GetRootNode()->GetChild("Computer")->GetChild("Output")->Read("debug", debug);
//
//				TEST_ASSERT( debug );
//			}
//        }
//    }
//}
