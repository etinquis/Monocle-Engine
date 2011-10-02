#include <memory>

#include <cpptest/cpptest-suite.h>
#include <cpptest/cpptest.h>

#include <Unit/Addons/Physics/Box2D.h>


int main()
{
    Test::Suite unit;
    unit.add(std::auto_ptr< ::Test::Suite>( new Monocle::Test::Unit::Box2D()));

    Test::TextOutput output(Test::TextOutput::Verbose);
    unit.run(output, false);
	system("Pause");
}
