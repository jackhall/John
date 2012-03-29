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

	template<unsigned int N, unsigned int I, unsigned int O>
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
		bool flip_coin(const real_type probability);
		bool gene_fcn(const unsigned int gene_index, const bool a, const bool b) const;
		real_type sigmoid(const real_type x) const;
		
		//following only called by constructor
		template<unsigned int N> 
		unsigned long get_integer(std::bitset<N>& sequence, unsigned int start) const;
		template<unsigned int N>
		real_type get_real(std::bitset<N>& sequence, unsigned int start) const;
		
		///////////////////////
		//current output values
		//learning parameters
		real_type learning_rate_val, momentum_val, weight_decay_val, forget_factor_val; 
		//action probabilities
		real_type kill_link_prob, make_link_prob, make_node_prob; 
		
		////////////////////////////
		//genetic network parameters
		//array of pairs of input links
		std::array< std::array<unsigned int,2>, N*N > links; //genes<inputs>
		//boolean functions of two inputs
		std::array< std::bitset<4>, N*N > functions; //order: 00, 01, 10, 11
		//input decision boundaries and biases to decide boolean inputs
		std::array< std::array<real_type, I+1>, N > input_decisions; //rows<columns>
		//weights to calculate outputs from boolean network
		std::array< std::array<real_type, N>, O > output_weights; //rows<columns>
		
		/////////////////////////
		//current internal states
		//current states of boolean switches (genes)
		std::bitset<N*N+N> state;
		//random number generator
		std::minstd_rand generator;
		
	public:
		Phenotype() = delete;
		explicit Phenotype(Genotype<N>& genome);
		Phenotype(const Phenotype& rhs) = delete;
		//Phenotype(Phenotype&& rhs);
		Phenotype& operator=(const Phenotype& rhs) = delete;
		//Phenotype& operator=(Phenotype&& rhs);
		~Phenotype() = default;
		
		void run(const real_type value; const real_type dvalue; const real_type persistence);
		
		inline real_type learning_rate() const { return learning_rate_val; }
		inline real_type momentum() const { return momentum_val; }
		inline real_type weight_decay() const { return weight_decay_val; }
		inline real_type forget_factor() const { return forget_factor_val; }
		inline bool kill_link() const { return flip_coin(kill_link_prob); }
		inline bool make_link() const { return flip_coin(make_link_prob); }
		inline bool make_node() const { return flip_coin(make_node_prob); }
		
	}; //class Phenotype

} //namespace john

#endif

