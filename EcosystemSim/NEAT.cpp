#include "NEAT.h"

Node::Node(int _id) :
	id(_id)
{
}

void Node::addValue(float val)
{
	value += val;
}

void Node::set(float val)
{
	value = val;
	lastValue = val;
}

float Node::getValue()
{
	return tanhf(lastValue);
	//return lastValue;
}

void Node::reset()
{
	lastValue = value;
	value = 0.f;
}

NEAT::NEAT(int _numInputs, int _numOutputs) :
	numInputs(_numInputs), 
	numOutputs(_numOutputs)
{
	for (int i = 0; i < _numInputs + _numOutputs; i++)
	{
		nodes.emplace_back(Node(i));
	}
	lastNodeId = _numInputs + _numOutputs - 1;
}

void NEAT::run()
{
	for (Link& link : links) {
		if (link.active) {
			link.end.addValue(link.start.getValue() * link.weight);
		}
	}
}

void NEAT::setInputs(std::vector<float> inputs)
{
	// Reset node values
	for (Node& node : nodes) {
		node.reset();
	}

	// Set inputs
	for (int i = 0; i < numInputs; i++)
	{
		nodes[i].set(inputs[i]);
	}
}

std::vector<float> NEAT::getOutputs()
{
	std::vector<float> outputs;

	for (int i = numInputs; i < numInputs + numOutputs; i++)
	{
		outputs.emplace_back(nodes[i].getValue());
	}

	return outputs;
}

void NEAT::mutate()
{
	while(!mutate_link());
}

bool NEAT::mutate_link()
{
	// Pick random first node excluding outputs
	int startNode = rand() % (nodes.size() - numOutputs);
	if (startNode > numInputs - 1 && startNode <= numInputs + numOutputs - 1)
		startNode += numOutputs;
	// Pick random second node excluding inputs
	int endNode = rand() % (nodes.size() - numInputs) + numInputs;
	// Check if second node is equal to first one, if it is pick again
	while (startNode == endNode)
		endNode = rand() % (nodes.size() - numInputs) + numInputs;

	// Check if given link exists
	for (Link& link : links) {
		if (link.start.id == nodes[startNode].id && link.end.id == nodes[endNode].id)
			return false;
	}

	// Create link
	float weight = ((float)rand() / RAND_MAX) * 4.f - 2.f;
	printf("Weight: %f\n", weight);
	links.emplace_back(Link(nodes[startNode], nodes[endNode], weight));
	return true;
}

Link::Link(Node& startNode, Node& endNode, float _weight) :
	start(startNode), 
	end(endNode),
	active(true),
	weight(_weight)
{
}
