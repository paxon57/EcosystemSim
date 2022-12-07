# EcosystemSim [WIP]

Visual Studio Community 2022 Project

SFML required and not included in this repo

Idea of the project is to create continuous simulation of hundreds or thousands of creatures of two types: Prey and Predator

Each creature will contain small Neural Network which will evolve and adapt using a NEAT inspired algorithm and description of that network will be encoded as a gene of the creature. 

Both populations will spawn at the beginning with random genes. Survival rules are simple:

Prey gets to reproduce if it survives long enough

Predator gets to reproduce if it eats Prey and can die of starvation

Reproduction is asexual and randomly slightly mutates the genome of the offspring

Both creatures will have health and attack points

Simulation ends when either Prey or Predator goes extinct
