#include "Simulation.h"

Simulation::Simulation()
{
}

void Simulation::update()
{
	if (running) imguiStats();
	if (!running) imguiSetup();
}

void Simulation::imguiStats()
{
	ImGui::Begin("Simulation Stats");
	ImGui::Value("Prey pop", preyAmount);
	ImGui::Value("Predator pop", predatorAmount);
	ImGui::End();
}

void Simulation::imguiSetup()
{
	ImGui::Begin("Simulation Setup");
	ImGui::Text("Initial population:");
	ImGui::InputInt("Prey", &initialPrey);
	ImGui::InputInt("Predator", &initialPredator);
	if (ImGui::Button("BEGIN SIMULATION")) beginSimulation();
	ImGui::End();
}

void Simulation::beginSimulation()
{
	printf("SIM HAS BEGUN");

	running = true;
}
