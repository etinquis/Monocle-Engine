#pragma once

#include <cpptest/cpptest-suite.h>

namespace Monocle
{
    class FileNode;
    
    namespace Test
    {
        namespace Unit
        {            
            class FileNodeTest: public ::Test::Suite
            {
            public:
                FileNodeTest();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Constructor();
                void BuildTree();
                void GetChild();
				void GetChildren();
				void InsertChild();
                void WriteAttributes();
                void ReadAttributes();
                
                Monocle::FileNode *node;
            };

        }
    }
}
