#include "SpriteTest.h"
#include "../Tests.h"

#include <Component/Entity/Sprite.h>
#include <Component/Entity/Transform.h>

#include <cpptest/cpptest-assert.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {
            ValidEntity::ValidEntity()
			{
				AddComponent<Transform>();
				AddComponent<Sprite>();
			}

			InvalidEntity::InvalidEntity()
			{
				AddComponent<Sprite>();
			}
			
			SpriteSuite::SpriteSuite()
            {
                TEST_ADD(SpriteSuite::Init)
            }
            
            void SpriteSuite::setup()
            {
                valid = new ValidEntity();
				invalid = new InvalidEntity();
            }
            
            void SpriteSuite::tear_down()
            {
                delete valid;
				valid = NULL;
				delete invalid;
				invalid = NULL;
            }
            
            void SpriteSuite::Init()
            {
                TEST_ASSERT( ((Sprite*)(*valid)["Sprite"]) != NULL );
                TEST_ASSERT( ((Sprite*)(*invalid)["Sprite"]) == NULL );
            }
        }
    }
}
