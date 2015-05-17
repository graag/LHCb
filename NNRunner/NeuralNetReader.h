#pragma once

#include "GVector.h"
#include <vector>

#ifndef NeuralNetReader_H
#define NeuralNetReader_H

#define F_TABLE_MAX 12.0F
#define F_TABLE_LENGTH 128

/// Class for running neural network prepared with NetMaker (www.ire.pw.edu.pl/~rsulej/NetMaker).
/// Example of application is a stopping particle identification. Network model and its coefficients
/// are stored in the XML file prepared with NetMaker. Then tracks are processed with ParticleIdNN
/// class. See pid_pattern and pid_test_eff examples in the AnalysisLoop package.
class NeuralNetReader
{
public:
	NeuralNetReader(const char* xmlFileName);
	~NeuralNetReader();

	inline int GetInputLength() const { return _nInps; }
	inline int GetOutputLength() const { return _nOuts; }

	void Run(const GVector<float>& vInput);
	void Run(const std::vector<float>& vInput);
	void Run(const std::vector<double>& vInput);

	float GetOneOutput(int neuronIndex) const;
	GVector<float> GetAllOutputs() const;

private:

	class Layer
	{
	public:
		~Layer();

		virtual void Activate() = 0;
		GVector<float>& GetOutput() { return *_output; }
		void SetCoeff(int neuronIndex, int coeffIndex, float value);

	protected:
		Layer(int nNeurons, Layer* prevLayer);
		Layer();

		int _nLength, _nInputs;
		GVector<float>* _output;
		GVector<float>** _coeffs;
		Layer* _previous;
	};

	class LayerSig : public Layer
	{
	public:
		LayerSig(int nNeurons, Layer* prevLayer);

		void Activate();

	private:
		float _fsig(double dotprod);

		static float f_factor;
		static float f_f[F_TABLE_LENGTH], f_d[F_TABLE_LENGTH];
	};

	class LayerSoftmax : public Layer
	{
	public:
		LayerSoftmax(int nNeurons, Layer* prevLayer) : Layer(nNeurons, prevLayer) {};

		void Activate();
	};

	class LayerLin : public Layer
	{
	public:
		LayerLin(int nNeurons, Layer* prevLayer) : Layer(nNeurons, prevLayer) {};

		void Activate();
	};


	class LayerInp : public Layer
	{
	public:
		LayerInp(int nInps);
		LayerInp(int nInps, const GVector<float>& vMean, const GVector<float>& vStd);
		~LayerInp();

		void Activate();
		void Activate(const GVector<float>& vInput);
		void Activate(const std::vector<float>& vInput);
		void Activate(const std::vector<double>& vInput);

	private:
		void Normalize();
		bool _normalize;
		GVector<float> *_mean, *_istdev;
	};


	unsigned int _nInps; 
	int _nOuts;
	std::vector<Layer*> _layers;

	//void SetupTransformation( vector<string>& info );
	void SetupNetwork(const char* xmlfile);
};
#endif
