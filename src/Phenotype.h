#ifndef Phenotype_h
#define Phenotype_h

/*
    John: an evolutionary algorithm for genetic networks
    Copyright (C) 2012  Jack Hall

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    e-mail: jackwhall7@gmail.com
*/

#include <random>
#include <array>
#include <bitset>

namespace john {

	class Phenotype {
	/*
		This class parses the genome (stored in Genotype) into a genetic switching 
		network, runs that network, and stores the resulting learning parameters. 
		Parsing is done in the constructor. The run() method steps the gene network
		forward one step and recalculates the learning parameters. 
	*/
	private:
		//unsigned long binary_to_gray(unsigned long num) { return (num>>1) ^ num; }
		unsigned long gray_to_binary(unsigned long num);
		bool flip_coin(const float probability);
		bool gene_fcn(const unsigned int gene_index, const bool a, const bool b) const;
		float sigmoid(const float x) const;
		
		//following only called by constructor
		template<unsigned int N> 
		unsigned long get_integer(std::bitset<N>& sequence, unsigned int start) const;
		template<unsigned int N>
		float get_real(std::bitset<N>& sequence, unsigned int start) const;
		
		///////////////////////
		//current output values
		//learning parameters
		float learning_rate_val, momentum_val, weight_decay_val, forget_factor_val; 
		//action probabilities
		float kill_link_prob, make_link_prob, make_node_prob; 
		
		////////////////////////////
		//genetic network parameters
		//array of pairs of input links
		std::array< std::array<unsigned int,2>, 25 > links; //genes<inputs>
		//boolean functions of two inputs
		std::array< std::bitset<4>, 25 > functions; //order: 00, 01, 10, 11
		//input decision boundaries and biases to decide boolean inputs
		std::array< std::array<float, 4>, 5 > input_decisions; //rows<columns>
		//weights to calculate outputs from boolean network
		std::array< std::array<float, 5>, 7 > output_weights; //rows<columns>
		
		/////////////////////////
		//current internal states
		//current states of boolean switches (genes)
		std::bitset<30> state;
		//random number generator
		std::minstd_rand generator;
		
	public:
		Phenotype() = delete;
		template<unsigned int N> explicit Phenotype(Genotype<N>& genome);
		Phenotype(const Phenotype& rhs) = delete;
		//Phenotype(Phenotype&& rhs);
		Phenotype& operator=(const Phenotype& rhs) = delete;
		//Phenotype& operator=(Phenotype&& rhs);
		~Phenotype() = default;
		
		void run(const float value; const float dvalue; const float persistence);
		
		inline float learning_rate() const { return learning_rate_val; }
		inline float momentum() const { return momentum_val; }
		inline float weight_decay() const { return weight_decay_val; }
		inline float forget_factor() const { return forget_factor_val; }
		inline bool kill_link() const { return flip_coin(kill_link_prob); }
		inline bool make_link() const { return flip_coin(make_link_prob); }
		inline bool make_node() const { return flip_coin(make_node_prob); }
		
	}; //class Phenotype

/*
	class ChanceFunction {
	private:
		vector<float> steepness;
		float vertical_bias, lateral_bias, scale_factor;
    		std::minstd_rand generator;
    		
    		bool result(const float z) {
    			float chance = vertical_bias + scale_factor/(1 + exp(-z));
			float critical_float = 1/(1 + exp(-chance)); //force between 0 and 1
			float random_float = std::generate_canonical<float, 10>(generator);
			return random_float < critical_float;
    		}
		
	public:
		ChanceFunction()
			: steepness(), vertical_bias(-1000000.0), //makes sigmoid result tiny
			  lateral_bias(0.0), scale_factor(0.0),
			  generator(std::random_device) {}
		ChanceFunction(const vector<float>& steep, 
			       const float vbias, 
			       const float lbias, 
			       const float scale)
			: steepness(steep), vertical_bias(vbias), 
			  lateral_bias(lbias), scale_factor(scale),
			  generator(std::random_device) {}
		ChanceFunction(const ChanceFunction& rhs) = delete;
		ChanceFunction& operator=(const ChanceFunction& rhs) = default;
		~ChanceFunction() = default;
		
		bool operator()() const {
			return 1/(1 + exp(-vertical_bias)); //constant value between 0 and 1
		}
		
		bool operator()(const float input) const { 
			if( steepness.size() != 1 ) throw;
			float z = lateral_bias + input*steepness[0];
			return result(z);
		}
		
		bool operator()(const float input1, const float input2) const {
			if( steepness.size() != 2 ) throw;
			float z = lateral_bias + input1*steepness[0] + input2*steepness[1];
			return result(z);
		}
	}; //struct ChanceFunction
*/	

} //namespace john

#endif
