#pragma once
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

struct Node
{
	public:
		Node(int _id);

		int id;
		bool isIO = false;

		sf::Vector2f graphPos = sf::Vector2f(0.f, 0.f);

		void addValue(float val);
		void set(float val);
		float getValue();
		void reset();

	private:
		float value = 0.f;
		float lastValue = 0.f;
};

struct Link
{
	public:
		Link(int _startNodeIndex, int _endNodeIndex, float _weight);

		bool active = true;
		float weight = 0.f;

		int startNodeIndex;
		int endNodeIndex;

		Link& operator= (const Link& x) {
			if (&x == this) return *this;

			startNodeIndex = x.startNodeIndex;
			endNodeIndex = x.endNodeIndex;
			weight = x.weight;
			active = x.active;
			return *this;
		}
};

class NEAT
{
	public:
		NEAT(int _numInputs, int _numOutputs);

		int numInputs;
		int numOutputs;

		std::vector<Node> nodes;
		std::vector<Link> links;

		void run();
		void setInputs(std::vector<float> inputs);
		std::vector<float> getOutputs();
		void mutate();

		void draw_net();

	private:
		int lastNodeId = 0;

		bool mutate_link();
		void mutate_node();
		void mutate_toggle();
		void mutate_weight();
		void mutate_rand_weight();

		void setup_IO_nodes_graph_pos();
};

