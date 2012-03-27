#ifndef Fitness_h
#define Fitness_h

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

#include <map>
#include <vector>

namespace john {

	template<unsigned int N, unsigned int I, unsigned int O>
	class Fitness {
	/*
		A Fitness object is necessary to manage the various Genotypes in a 
		population. Its main function is to decide which two existing Genotypes 
		will serve as parents for a new Genotype. This decision is made with
		probabilities weighted by the value of each Genotype. 
	*/
	private:
		map<unsigned int, Genotype<N,I,O>*> population;
		std::minstd_rand generator; //for choosing individuals for breeding
		
		Genotype<N,I,O>* select(vector<info_type> values, info_type total);
		
	public:
		Fitness() = default;
		Fitness(const Fitness& rhs) = delete;
		//Fitness(Fitness&& rhs);
		Fitness& operator=(const Fitness& rhs) = delete;
		//Fitness& operator=(Fitness&& rhs);
		~Fitness() = default;
		
		unsigned population_size() { return population.size(); }
		
		std::pair< Genotype<N,I,O>*, Genotype<N,I,O>* > breed() const;
		void add(const unsigned int address, Genotype<N>* new_genome);
		void remove(const unsigned int address);
		void update(const unsigned int address, Genotype<N>* pGenotype);
		
	}; //class Fitness

} //namespace john

#endif
