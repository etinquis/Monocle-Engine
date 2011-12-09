#pragma once

#include <cpptest/cpptest-suite.h>
#include <Entity.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
		{
			class ValidEntity : public Entity
			{
			public:
				ValidEntity();
			};

			class InvalidEntity : public Entity
			{
			public:
				InvalidEntity();
			};

            class SpriteSuite: public ::Test::Suite
            {
            public:
                SpriteSuite();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Init();
                
				ValidEntity *valid;
				InvalidEntity *invalid;
            };

        }
    }
}
