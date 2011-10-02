#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {            
			class Box2D: public ::Test::Suite
            {
            public:
                Box2D();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Simulate();
            };
        }
    }
}
