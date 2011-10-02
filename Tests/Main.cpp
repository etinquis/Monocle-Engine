#include <memory>

#include <cpptest/cpptest-suite.h>
#include <cpptest/cpptest.h>

#include <Unit/AssetTest.h>
#include <Unit/FontAssetTest.h>
#include <Unit/AssetsTest.h>
#include <Unit/CameraTest.h>
#include <Unit/VectorTest.h>
#include <Unit/File/FileNodeTest.h>
#include <Unit/File/Types/types.h>

//FileTypes
#include <File/Types/json.h>
#include <File/Types/XML.h>

int main()
{
    Test::Suite unit;
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Assets()));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Asset() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::FontAsset() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Camera() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Vector() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::FileNodeTest() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::FileTypeTest<Monocle::FileType::json>("Tests/Types/test.json") ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::FileTypeTest<Monocle::FileType::XML>("Tests/Types/test.xml") ));
    
    Test::TextOutput output(Test::TextOutput::Verbose);
    unit.run(output, false);
	system("Pause");
}
