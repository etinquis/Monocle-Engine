#pragma once

#include <cpptest/cpptest-suite.h>
#include <Entity.h>

namespace Monocle
{
	class Game;
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

            class SpriteTest: public ::Test::Suite
            {
            public:
                SpriteTest();
                
            protected:
                virtual void setup();
                virtual void tear_down();
                
            private:
                void Init();
                
				Game *game;
				ValidEntity *valid;
				InvalidEntity *invalid;
            };

        }
    }
}
