#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {
            class XMLFileTest: public ::Test::Suite
            {
            public:
                XMLFileTest();
                
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
