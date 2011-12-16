#pragma once

#include "Component/GameComponent.h"

namespace Monocle
{
	class Random : public GameComponent
	{
	public:
		Random();
		
		static const std::string ComponentName;
		const std::string& GetName() { return Random::ComponentName; }

		void Init(Game* game) { GameComponent::Init(game); }
		void Update() { }
		void Unload() { }

		static void Seed(unsigned int seed);
		//! not inclusive
		static int Int(int start, int end);
		static float Float(float start, float end);
		static float Percent();
		
		Random *Clone() const;
	private:
		//static Random *instance;
	};
}