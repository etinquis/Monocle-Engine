#pragma once

#include "Platform.h"
#include "Monocle.h"
#include "Graphics.h"
#include "Assets.h"
#include "Audio/Audio.h"
#include "Level/Level.h"

#include <unordered_map>

namespace Monocle
{
	class Scene;

	//! Base class for creating a new game. Manages the main loop, timer and high-level updating, rendering.
	//!	Some games will be able to get away with just instantiating this class and adding Scenes.
	//! Some developers may want to extend Game and customize it, instead.
	class Game
	{
	public:
		typedef std::unordered_map<std::string, GameComponent*> ComponentList;

		//! Initializes all the default sub-systems. Platform, Input, Graphics, Debug, Assets, Tween, Collision, Random, Audio, Level
		Game(const std::string &name="MonoclePowered.org", int w=1024, int h=768, int bits = MONOCLE_DETECT_COLOR_DEPTH, bool fullscreen=false);
		~Game();

		//! Runs the main game loop. Handles timing and high-level updating, rendering.
		void Main();
        
        //! One game loop.
        void OneLoop();

		//! Updates the game state.  Derived classes may override this to provide extra functionality that must be
		//! executed in each game loop iteration.
		virtual void Update();

		//! Sets the current Scene. Games can run one Scene at a time.
		static void SetScene(Scene* scene);
		//! Returns a pointer to the currently running Scene.
		static Scene* GetScene();

		//! Call Game::Quit to quit the main loop. (exit your game)
		static void Quit();

		//! Receives a string-based message, referred to as a "note." Can be overloaded.
		//virtual void ReceiveNote(const std::string &note);
		
		//virtual void Init();
        static float frames_per_sec;
        
        //! Called particularly for iOS to reset isDone and other variables. Caused by a Game::Quit
        void PlatformReset();
        
        //! Getter for isDone
        bool IsDone();

		template <class t_component>
		t_component* AddComponent()
		{
			t_component *comp = new t_component();

			components[comp->GetName()] = comp;
			comp->Init(this);
			return comp;
		}

		GameComponent* operator[](std::string component_name);

	protected:
		Platform platform;
		Graphics graphics;
		Audio audio;

		ComponentList components;

	private:
		static Game *instance;

		//! Notifies the Game if it needs to exit sanely at the next opportunity
		bool isDone;

		//! The scene that is currently running
		Scene* scene;

		//! Scene to switch to at the end of the frame; if NULL, no switch
		Scene* switchTo;
        
        // Loop stuff
        long lastTick;
        long firstTick;
	};
}
