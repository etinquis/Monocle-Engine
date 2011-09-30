#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {
            class jsonFileTest: public ::Test::Suite
            {
            public:
                jsonFileTest();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Read();
                void Write();
            };

        }
    }
}
