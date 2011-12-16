#pragma once

#include "AudioTest.h"
#include <Audio/Audio.h>

namespace Monocle
{
    class Audio;
    
    namespace Test
    {
        namespace Unit
        {            
			Audio::Audio()
			{
				TEST_ADD(Audio::Init);
				TEST_ADD(Audio::MultipleInstances);
			}

			void Audio::setup()
			{
				audio = new Monocle::Audio();
			}

			void Audio::tear_down()
			{
				delete audio;
				audio = NULL;
			}

			void Audio::Init()
			{ 
				audio->Init();
			}

			void Audio::MultipleInstances()
			{
				Monocle::Audio *a2 = new Monocle::Audio();
				a2->Init();
				
				delete a2;
			}
        }
    }
}