// Simple example of propagating a vector through the network obtained from NetMaker.
// input: vector of 2 variables;
// output: vector of 4 variables.

// file list:
// cmdNeuralNet.c - this file
// NeuralNetReader.h, NeuralNetReader.cc - neural network class
// GVector.h - math vector class used by the network
// network.xml - network description created with NetMaker

// compilation: make
// note: libxml2-devel must be available before the ROOT is compiled

#include <iostream>
#include "NeuralNetReader.h"

int main(int argc, char **argv)
{
	// network takes as an input one of the following vectors
	// (uncomment the one of your choice):

	// GVector input(2);
	// std::vector<double> input(2);
	std::vector<float> input(4);

	// here, the values are hardcoded...
    double Xbj = 0.0380538;
    double Q2 = 3.18727;
    double pt1 = 0.660422;
    double pl1 = 30.8956;
      input[0]=(Xbj-0.0426)/0.0543;
      input[1]=(Q2-3.6376)/4.8415;
      input[2]=(pt1-0.6829)/0.3904;
      input[3]=(pl1-12.76)/12.68;

      std::cout<<input[0]<<std::endl;
    std::cout<<input[1]<<std::endl;
    std::cout<<input[2]<<std::endl;
    std::cout<<input[3]<<std::endl;

	// create the network object from an XML file from NetMaker
	NeuralNetReader* net = NULL;
	try
	{
		net = new NeuralNetReader("qcdc.xml");
	}
	catch (char const* e)
	{
		std::cout << e << std::endl;
		return 0;
	}

	// ---loop over the following lines should be encoded when processing set of vectors---

		// run the network over the input vector
		net->Run(input);

		// get the network answer at the output neuron #0:
		// float answer = net.GetOneOutput(0);

		// all outputs in a single vector:
		GVector<float> vanswer = net->GetAllOutputs();

		// and do something with that answer
		std::cout << "network output is: "
			<< vanswer[0] << " " << vanswer[1] << " "
			<< vanswer[2] << " " << vanswer[3] << std::endl;

	// ---end of possible loop---

	delete net;
	return 0;
}

