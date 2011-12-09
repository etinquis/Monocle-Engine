#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    class Audio;
    
    namespace Test
    {
        namespace Unit
        {            
            class Audio: public ::Test::Suite
            {
            public:
                Audio();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Init();
                void MultipleInstances();

                Monocle::Audio *audio;
            };

        }
    }
}