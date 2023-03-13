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
	// Create IO nodes
	for (int i = 0; i < _numInputs + _numOutputs; i++)
	{
		Node& node = nodes.emplace_back(Node(i));
		node.isIO = true;
	}
	lastNodeId = _numInputs + _numOutputs - 1;
	
	setup_IO_nodes_graph_pos();
}

void NEAT::run()
{
	for (Link& link : links) {
		if (link.active) {
			nodes[link.endNodeIndex].addValue(nodes[link.startNodeIndex].getValue() * link.weight);
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
	float random = ((float)rand() / RAND_MAX);
	if (random < 0.2f) {
		if (!mutate_link())
			mutate_node();
	}
	else if (random < 0.4f)
		mutate_node();
	else if (random < 0.6f)
		mutate_toggle();
	else if (random < 0.8f)
		mutate_weight();
	else if (random < 1.f)
		mutate_rand_weight();
		
}

void NEAT::draw_net()
{
	// Calculate Middle Node positions
	for (Node& node : nodes) 
	{
		if (node.isIO) continue;

		// Find all connected links
		std::vector<Link*> inLinks;
		std::vector<Link*> outLinks;
		for (Link& link : links)
		{
			if (link.endNodeIndex == node.id)
				inLinks.emplace_back(&link);
			else if (link.startNodeIndex == node.id)
				outLinks.emplace_back(&link);
		}

		sf::Vector2f inputAveragePos;
		for (Link* link : inLinks)
			inputAveragePos += nodes[link->startNodeIndex].graphPos;
		inputAveragePos /= (float)inLinks.size();

		sf::Vector2f outputAveragePos;
		for (Link* link : outLinks)
			outputAveragePos += nodes[link->endNodeIndex].graphPos;
		outputAveragePos /= (float)outLinks.size();

		node.graphPos = (inputAveragePos + outputAveragePos) / 2.f;
	}
	
	// Draw Links
	sf::Vector2f offset(12.5f, 12.5f);
	for (Link& link : links) {
		sf::Color color;
		if (!link.active)
			color = sf::Color(236, 240, 241);
		else {
			if (link.weight >= 0)
				color = sf::Color(46, 204, 113);
			else
				color = sf::Color(231, 76, 60);
		}
		ImGui::DrawLine(
			nodes[link.startNodeIndex].graphPos + offset, 
			nodes[link.endNodeIndex].graphPos + offset, 
			color, 
			fabs(link.weight) * 3.f);
	}

	// Draw Nodes
	for (Node& node : nodes)
	{
		ImGui::DrawRectFilled(
			sf::FloatRect(node.graphPos, sf::Vector2f(25.f, 25.f)),
			sf::Color(52, 152, 219),
			25.f);
	}

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
		//if (link.start.id == nodes[startNode].id && link.end.id == nodes[endNode].id)
		if (nodes[link.startNodeIndex].id == nodes[startNode].id && nodes[link.endNodeIndex].id == nodes[endNode].id)
			return false;
	}

	// Create link
	float weight = ((float)rand() / RAND_MAX) * 4.f - 2.f;
	printf("Weight: %f\n", weight);
	links.emplace_back(Link(startNode, endNode, weight));
	return true;
}

void NEAT::mutate_node()
{
	// If there are no links then create one instead and return
	if (links.size() == 0) { mutate_link(); return; }
	// Pick random link
	int randLinkIndex = (int)rand() % links.size();
	// Create a new node
	lastNodeId++;
	nodes.emplace_back(Node(lastNodeId));
	// Save it's information and remove it
	float weight = links[randLinkIndex].weight;
	int startNode = links[randLinkIndex].startNodeIndex;
	int endNode = links[randLinkIndex].endNodeIndex;
	links.erase(links.begin() + randLinkIndex);
	// Create new links
	links.emplace_back(Link(startNode, nodes.size() - 1, 1.f));
	links.emplace_back(Link(nodes.size() - 1, endNode, weight));
	int x = 1;
}

void NEAT::mutate_toggle()
{
	// If there are no links then create one instead and return
	if (links.size() == 0) { mutate_link(); return; }
	// Pick random link
	int randLinkIndex = rand() % links.size();
	// Toggle the link
	links[randLinkIndex].active = !links[randLinkIndex].active;
}

void NEAT::mutate_weight()
{
	// If there are no links then create one instead and return
	if (links.size() == 0) { mutate_link(); return; }
	// Pick random link
	int randLinkIndex = rand() % links.size();
	// Adjust the weight and clamp
	links[randLinkIndex].weight += (((float)rand() / RAND_MAX) * 0.4f - 0.2f);
	links[randLinkIndex].weight = fmaxf(-2.f, fminf(2.f, links[randLinkIndex].weight));
}

void NEAT::mutate_rand_weight()
{
	// If there are no links then create one instead and return
	if (links.size() == 0) { mutate_link(); return; }
	// Pick random link
	int randLinkIndex = rand() % links.size();
	// Adjust the weight and clamp
	links[randLinkIndex].weight = ((float)rand() / RAND_MAX) * 4.f - 2.f;
}

void NEAT::setup_IO_nodes_graph_pos()
{
	// Setup inputs
	for (int i = 0; i < numInputs; i++)
	{
		nodes[i].graphPos = sf::Vector2f(0.f, 30.f * i);
	}
	// Setup outputs
	float offset = (numInputs - numOutputs) / 2.f * 30.f;
	for (int i = 0; i < numOutputs; i++)
	{
		nodes[numInputs + i].graphPos = sf::Vector2f(500.f, offset + 30.f * i);
	}
}

Link::Link(int _startNodeIndex, int _endNodeIndex, float _weight) :
	startNodeIndex(_startNodeIndex),
	endNodeIndex(_endNodeIndex),
	active(true),
	weight(_weight)
{
}
