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

namespace john {
	
	template<unsigned int N, unsigned int I, unsigned int O> 
	std::pair< Genotype<N,I,O>*, Genotype<N,I,O>* > Fitness<N,I,O>::breed() {
		/*
		This method chooses two genotypes to breed based on their Node values. 
		The values must be polled from each Genotype. If Node values are not 
		positive, this will not work.
		*/
		//poll genotypes for value and compute the total value
		vector<real_type> values( population.size(), 0.0 );
		real_type total=0.0;
		auto itv = values.begin();
		auto it = population.begin();
		auto ite = population.end();
		while(it != ite) {
			*itv = it->second->value;
			total += *itv;
			++it; ++itv;
		}
		
		//probabilistically select two genotypes by their pointers
		Genotype<N,I,O>* mother( select(value, total) );
		Genotype<N,I,O>* father( select(value, total) );
		while(mother == father) father = select(value,total); //ensure parents differ
		std::pair< Genotype<N,I,O>*, Genotype<N,I,O>* > parents(mother, father);
		return parents;
	} //breed
	
	template<unsigned int N, unsigned int I, unsigned int O>
	Genotype<N,I,O>* Fitness<N,I,O>::select(const vector<real_type>& values, 
						const real_type total) {
		/*
		This method iterates through the population, selecting one genotype
		using a random number. The random number is scaled between 0 and the 
		total population value, which corresponds to the total fitness.
		*/
		real_type choice = total * std::generate_canonical<float, 15>(generator);
		
		auto it = values.begin();
		auto ite = values.end();
		auto itm = population.begin();
		while(it != ite) {
			choice -= *it;
			if(choice < 0) break; //this one
			++it; ++itm;
		}
		return itm->second; //should be valid if choice is between zero and one
	} //select
	
	template<unsigned int N, unsigned int I, unsigned int O> 
	bool Fitness<N,I,O>::add(const ID_type address, Genotype<N,I,O>* new_genome) {
		/*
		Adds new Genotype and ID to the population. The map::insert method checks
		to see whether that ID is already being used, and rejects the new value if
		an old value exists. It also returns whether the new ID was added. 
		*/
		pair< ID_type, Genotype<N,I,O>* > element;
		auto result = population.insert( std::make_pair(address, new_genome) );
		return result->second; //whether element was inserted
	} //add
	
	template<unsigned int N, unsigned int I, unsigned int O> 
	void Fitness<N,I,O>::remove(const ID_type address) {
		population.erase(address); //does nothing if address is invalid
	} //remove
	
	template<unsigned int N, unsigned int I, unsigned int O> 
	bool Fitness<N,I,O>::update(const ID_type address, Genotype<N>* pGenotype) {
		/*
		Updates the pointer to an existing Genotype. Returns false if the address
		is invalid.
		*/
		auto it = population.find(address);
		if(it != population.end()) { it->second = pGenotype; return true; }
		else return false; //whether element existed in the first place
	} //update
	
} //namespace john

