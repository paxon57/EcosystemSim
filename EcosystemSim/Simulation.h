#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

static class Simulation
{
	public:
		Simulation();

		int preyAmount = 0;
		int predatorAmount = 0;

		void update();

	private:
		bool running = false;
		int initialPrey = 0;
		int initialPredator = 0;

		void imguiStats();
		void imguiSetup();

		void beginSimulation();
};

