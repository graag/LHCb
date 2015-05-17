#include "NeuralNetReader.h"

#include <string>

#ifndef __CINT__
#include <iostream>
#endif
#include <cstdlib>
#include <math.h>
#include <iostream>

// ROOT XML libs
#include <TDOMParser.h>
#include <TXMLNode.h>
#include <TXMLAttr.h>
#include <TList.h>
#include <TDirectory.h>

NeuralNetReader::NeuralNetReader(const char* xmlFileName)
{
	try { SetupNetwork(xmlFileName); }
	catch (char const* e) { std::cout << e << std::endl; throw "NNetNotCreated"; }
}

void NeuralNetReader::SetupNetwork(const char* xmlFile)
{
	std::string type, node_name, attr_name ;
	int size = 0; 

	TDOMParser *xml_parser = new TDOMParser();  // creates TDOMParser root object
	TXMLNode *node; 
	TXMLNode *vec_node;
	TXMLAttr *attr;
	TList *attr_list;

	xml_parser->SetValidate(0);  // switch validation off (no need for dtd file)
	int err_code = xml_parser->ParseFile(xmlFile);  // parses xmlFile and creates TXMLDocument root object
	if (err_code) throw "ErrXmlParser";
	node = xml_parser->GetXMLDocument()->GetRootNode()->GetChildren();  // node = pointer to the first child of RootNode (<model> in our case)

	std::string lay("layer");
	std::string typ("type");	
	std::string siz("size");
	std::string inp("Input");
	std::string sig("Sigmoid");
	std::string smx("Softmax");
	std::string lin("Linear");
	std::string vec("vector");

	bool no_layer = true;
	for (; node; node = node->GetNextNode())  // searches all children of <model>
	{   
		node_name = node->GetNodeName();
		if (node_name.compare(lay) == 0)
		{
			no_layer = false;
			bool no_size = true;
			bool no_type = true;

			if (node->HasAttributes())										
			{	
				attr_list = node->GetAttributes();
				for (attr = (TXMLAttr*)attr_list->First (); attr; attr = (TXMLAttr*)attr_list->After(attr))  // searches through all <layer> attributes
				{
					attr_name = attr->Key();
					if (attr_name.compare(typ) == 0) { type = attr->GetValue(); no_type = false; }
					if (attr_name.compare(siz) == 0) { size = atoi(attr->GetValue()); no_size= false; }
				}
			}
			if (no_size) { throw "ErrLayerSizeNotSpecified"; }
			if (no_type) { throw "ErrLayerTypeNotSpecified"; }

			if (type.compare(inp) == 0) 
			{ 
				if (_layers.size() == 0) { _layers.push_back(new LayerInp(size)); _nInps = size; }
			}
			else
			{
				if (type.compare(sig) == 0) _layers.push_back(new LayerSig(size, _layers.back()));			
				else if (type.compare(smx) == 0) _layers.push_back(new LayerSoftmax(size, _layers.back()));	
				else if (type.compare(lin) == 0) _layers.push_back(new LayerLin(size, _layers.back()));
				else throw "ErrLayeTypeNotSupported";
				_nOuts = size;

				int i = 0;					
				for (vec_node = node->GetChildren(); vec_node; vec_node = vec_node->GetNextNode() )	// searches through all <layer> children
				{
					node_name = vec_node->GetNodeName();
					if (node_name.compare(vec) == 0)
					{
						std::string vec_coef;
						int vec_size = 0;
						if (vec_node->HasAttributes())
						{
							attr_list = vec_node->GetAttributes();	// creats root object TList of root objects TXMLAttr
							attr = (TXMLAttr*)attr_list->First ();	// we assume the one and only attribute of <vector> is 'length'
							vec_size = atoi(attr->GetValue());
						}
						else { throw "ErrVectorSizeNotSpecified"; }
						if (vec_node -> HasChildren()) { vec_coef = vec_node->GetChildren()->GetContent(); }	//gets the numbers specifying <vector> 	
						else { throw "ErrVectorCoefNotSpecified"; }

						int c, start;
						c = 0; start = 0;
						while (start != -1)	// changes string 'vec_coef' to floats. we assume numbers in vector are separated by ' ' 
						{				// this needs to be improved a bit, in fact numbers are separated by ';'
							(*_layers.back()).SetCoeff(i, c, (float)atof(vec_coef.substr(start, vec_coef.size() - start ).c_str()));
							start = (int)vec_coef.find_first_of( ' ', start + 1 );
							c++;
						}
						if ( c!= vec_size ) { throw "ErrVectorSize"; }     // checks if the number of above floats meats the <vector> length 
						i++;
					}
				}
				if (i!= size) { throw "ErrLayerSize"; }				// checks if the number of <vector>s meats the size of the layer	
			}
		}
	}
	if (no_layer) { throw "ErrNoLayerInNetwork"; }
}

NeuralNetReader::~NeuralNetReader()
{
	while (_layers.size() > 0)
	{
		delete _layers.back();
		_layers.pop_back();
	}
}

void NeuralNetReader::Run(const GVector<float>& vInput)
{
	if (vInput.Size() == _nInps)
	{
		((LayerInp*)_layers[0])->Activate(vInput);
		for (unsigned int i = 1; i < _layers.size(); i++) _layers[i]->Activate();
		return;
	}
	else throw "InpSizeErr";
}

void NeuralNetReader::Run(const std::vector<float>& vInput)
{
	if (vInput.size() == _nInps)
	{
		((LayerInp*)_layers[0])->Activate(vInput);
		for (unsigned int i = 1; i < _layers.size(); i++) _layers[i]->Activate();
		return;
	}
	else throw "InpSizeErr";
}

void NeuralNetReader::Run(const std::vector<double>& vInput)
{
	if (vInput.size() == _nInps)
	{
		((LayerInp*)_layers[0])->Activate(vInput);
		for (unsigned int i = 1; i < _layers.size(); i++) _layers[i]->Activate();
		return;
	}
	else throw "InpSizeErr";
}

float NeuralNetReader::GetOneOutput(int neuronIndex) const
{
	return _layers.back()->GetOutput()[neuronIndex];
}

GVector<float> NeuralNetReader::GetAllOutputs() const
{
	GVector<float> vret(_layers.back()->GetOutput());
	return vret;
}

///   Layer Class (nested in NeuralNetReader)   ///
NeuralNetReader::Layer::Layer(int nNeurons, Layer* prevLayer)
{
	_nLength = nNeurons;
	_nInputs = prevLayer->_nLength;
	_previous = prevLayer;

	_output = new GVector<float>(nNeurons + 1);
	_coeffs = new GVector<float>*[nNeurons];
	for (unsigned int i = 0; i < nNeurons; i++)
	{
		_coeffs[i] = new GVector<float>(_nInputs + 1);
	}
	(*_output)[nNeurons] = 1.0F;
}

NeuralNetReader::Layer::Layer()
{
	_previous = 0;
	_output = 0;
	_coeffs = 0;
	_nLength = 0;
	_nInputs = 0;
}

NeuralNetReader::Layer::~Layer()
{
	if (_coeffs != 0)
	{
		for (unsigned int i = 0; i < _nLength; i++) delete _coeffs[i];
		delete [] _coeffs;
	}
	delete _output;
}

void NeuralNetReader::Layer::SetCoeff(int neuronIndex, int coeffIndex, float value)
{
	(*_coeffs[neuronIndex])[coeffIndex] = value;
}

///   LayerSig Class (nested in NeuralNetReader)   ///
float NeuralNetReader::LayerSig::f_f[F_TABLE_LENGTH];
float NeuralNetReader::LayerSig::f_d[F_TABLE_LENGTH];
float NeuralNetReader::LayerSig::f_factor;

NeuralNetReader::LayerSig::LayerSig(int nNeurons, Layer* prevLayer) : Layer(nNeurons, prevLayer)
{
	f_factor = (float)(F_TABLE_LENGTH - 1) / F_TABLE_MAX;
	for (unsigned int i = 0; i < F_TABLE_LENGTH; i++)
	{
		f_f[i] = (float)(1.0 / ( 1.0 + exp(-((double)i) / f_factor)));
		if (i > 0) f_d[i-1] = f_f[i] - f_f[i-1];
	}
}

float NeuralNetReader::LayerSig::_fsig(double dotprod)
{
	int i;
	float xd;

	if (dotprod >= 0.0)
	{
		xd = (float)(dotprod * f_factor);

		i = (int)xd;
		if (i < (F_TABLE_LENGTH - 1))
			return f_f[i] + f_d[i] * (xd - i);
		return f_f[F_TABLE_LENGTH - 1];
	}
	else
	{
		xd = (float)(-dotprod * f_factor);

		i = (int)xd;
		if (i < (F_TABLE_LENGTH - 1))
			return 1.0F - (f_f[i] + f_d[i] * (xd - i));
		return 1.0F - f_f[F_TABLE_LENGTH - 1];
	}
}

void NeuralNetReader::LayerSig::Activate()
{
	for (unsigned int i = 0; i < _nLength; i++)
		(*_output)[i] = _fsig(_previous->GetOutput() * (*_coeffs[i]));
}

/// LayerSoftmax Class (nested in NeuralNetReader) ///
void NeuralNetReader::LayerSoftmax::Activate()
{
	double o, sum = 0.0;
	for (unsigned int i = 0; i < _nLength; i++)
	{
		o = _previous->GetOutput() * (*_coeffs[i]);
		if (o > 30.0) o = 30.0;

		o = exp(o); sum += o;
		(*_output)[i] = (float)o;
	}

	if (sum > 0.0) _output->Scale((float)(1.0 / sum));
	else _output->Set(1.0F / _nLength);
}

///   LayerLin Class (nested in NeuralNetReader)   ///
void NeuralNetReader::LayerLin::Activate()
{
	for (unsigned int i = 0; i < _nLength; i++)
		(*_output)[i] = (float)(_previous->GetOutput() * (*_coeffs[i]));
}

///   LayerInp Class (nested in NeuralNetReader)   ///
NeuralNetReader::LayerInp::LayerInp(int nInps)
{
	_output = new GVector<float>(nInps + 1);
	(*_output)[nInps] = 1.0F;
	_nLength = nInps;
	_nInputs = nInps;
	_normalize = false;
}

NeuralNetReader::LayerInp::LayerInp(int nInps, const GVector<float>& vMean, const GVector<float>& vStd)
{
	if ((vMean.Size() != nInps) || (vStd.Size() != nInps)) throw "VectorSizeErr";

	_output = new GVector<float>(nInps + 1);
	(*_output)[nInps] = 1.0F;
	_nLength = nInps;
	_nInputs = nInps;

	_normalize = true;
	_mean = new GVector<float>(nInps + 1);
	for (unsigned int i = 0; i < nInps; i++) (*_mean)[i] = vMean[i];
	(*_mean)[nInps] = 0.0F;
	_istdev = new GVector<float>(nInps + 1);
	for (unsigned int i = 0; i < nInps; i++ ) (*_istdev)[i] = vStd[i];
	(*_istdev)[nInps] = 1.0F;
}

NeuralNetReader::LayerInp::~LayerInp()
{
	if (_normalize)
	{
		delete _mean;
		delete _istdev;
	}
}

void NeuralNetReader::LayerInp::Activate()
{
	_output->Set(0.0F);
}

void NeuralNetReader::LayerInp::Normalize()
{
	_output->Sub(*_mean);
	_output->Scale(*_istdev);
}

void NeuralNetReader::LayerInp::Activate(const GVector<float>& vInput)
{
	vInput.CopyTo(*_output);
	if (_normalize) Normalize();
}

void NeuralNetReader::LayerInp::Activate(const std::vector<float>& vInput)
{
	for (unsigned int i = 0; i < vInput.size(); i++) (*_output)[i] = vInput[i];
	if (_normalize) Normalize();
}

void NeuralNetReader::LayerInp::Activate(const std::vector<double>& vInput)
{
	for (unsigned int i = 0; i < vInput.size(); i++) (*_output)[i] = (float)vInput[i];
	if (_normalize) Normalize();
}			
						

	
