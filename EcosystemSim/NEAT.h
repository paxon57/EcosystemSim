#pragma once
#include <vector>

struct Node
{
	public:
		Node(int _id);

		int id;

		void addValue(float val);
		void set(float val);
		float getValue();
		void reset();

	private:
		float value;
		float lastValue;
};

struct Link
{
	public:
		Link(Node& startNode, Node& endNode, float _weight);

		bool active;
		float weight;

		Node& start;
		Node& end;
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
	private:
		int lastNodeId = 0;

		bool mutate_link();
};

