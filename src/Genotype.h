#ifndef Genotype_h
#define Genotype_h

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

#include <array>
#include <bitset>

namespace john {

	template<unsigned int N, unsigned int I, unsigned int O>
	class Fitness;

	//N is # of attractors (sqrt of # of nodes)
	//I is # of non-boolean inputs to gene network
	//O is # of non-boolean outputs from the network
	template<unsigned int N, unsigned int I, unsigned int O> 
	class Genotype {
	/*
		Genotype stores a neuron's genome and associates it with the neuron's value
		(the value is used as a measure of evolutionary fitness). Genotypes also store
		a pointer to the Fitness object that coordinates the population. The major work
		is done in the constructors: the default constructor generates a random genome, 
		and the second constructor performs crossover and mutation from two parents. The
		Phenotype class has private access in order to parse the genome. 
		
	*/
	private:
		Fitness<N>* fitness;
		//17 bit numbers, K=2 connectivity
		std::bitset< //only need one chromosome now
		std::bitset<N*N*2*2 + (I+O+1)*17*N> decision_chromosome;
		std::array< std::bitset<N*N + N>, N*N > link_chromosome;
		//mutation and crossover rates? probably just hardcode these for now
		
		std::minstd_rand generator;
		
	public:
		const ID_type ID;
		real_type value;
		
		Genotype() = delete;
		Genotype(const ID_type nID,  
			 Fitness<N,I,O>* pFitness);
		Genotype(const ID_type nID, const Genotype& mother, const Genotype& father);
		Genotype(const Genotype& rhs) = delete;
		//Genotype(Genotype&& rhs); 
		Genotype& operator=(const Genotype& rhs) = delete;
		//Genotype& operator=(Genotype&& rhs);
		~Genotype();
		
		friend class Phenotype<N,I,O>; //only Phenotype constructor actually needs access
		
	}; //class Genotype

} //namespace john

#endif

