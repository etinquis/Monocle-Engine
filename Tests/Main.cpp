#include <memory>

#include <cpptest/cpptest-suite.h>
#include <cpptest/cpptest.h>

#include <Unit/AssetTest.h>
#include <Unit/FontAssetTest.h>
#include <Unit/AssetsTest.h>
#include <Unit/CameraTest.h>
#include <Unit/VectorTest.h>
#include <Unit/File/FileNodeTest.h>
#include <Unit/File/Types/XMLTest.h>
#include <Unit/File/Types/jsonTest.h>

int main()
{
    Test::Suite unit;
    //unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Assets()));
    //unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Asset() ));
    //unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::FontAsset() ));
    //unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Camera() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Vector() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::FileNodeTest() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::XMLFileTest() ));
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::jsonFileTest() ));
    
    Test::TextOutput output(Test::TextOutput::Verbose);
    unit.run(output, false);
	system("Pause");
}
