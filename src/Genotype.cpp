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
	Genotype::Genotype(const ID_type nID, 
			   Fitness<N,I,O>* pFitness) 
		: ID(nID), fitness(pFitness), value(0.0), generator(nID),
		  link_chromosome(), decision_chromosome() {
		  
		fitness->add(ID, this);
		
		//create random bit-string for decision_chromosome
		std::bernoulli_distribution random_bit(0.5);
		for(int i=decision_chromosome.size()-1; i>=0; --i) {
			decision_chromosome[i] = random_bit(generator);
		}
		
		//create an array of random bitsets, each of which contains two ones
		int x, y;
		std::uniform_int_distribution<> random_int(0, link_chromosome[0].size() - 1);
		for(i=link_chromosome.size()-1; i>=0; --i) {
			x = random_int(generator);
			y = random_int(generator);
			while(x == y) y = random_int(generator);
			
			link_chromosome[i][x] = true;
			link_chromosome[i][y] = true;
		}
	} //constructor
	
	template<unsigned int N, unsigned int I, unsigned int O>
	Genotype::Genotype(const ID_type nID, const std::pair<Genotype*, Genotype*> parents) 
		: ID(nID), decision_chromosome(), link_chromosome(),
		  fitness(parents.first->fitness), value(0.0), generator(nID) {
		
		real_type mutation_rate = 0.2, crossover_rate = 0.5;
		int i, j, ii, jj;
		
		//breed new chromosomes from parents, use hardcoded mutation and crossover rates
		fitness->add(ID, this);
		
		//what does crossover rate mean?
		//generate 2 random numbers for decision_chromosome crossover
		std::uniform_int_distribution<> random_int(0, decision_chromosome.size() - 1);
		std::bernoulli_distribution crossover(crossover_rate);
		if( crossover(generator) ) {
			i = random_int(generator);
			
			//take beginning of chromosome from first parent ...
			for(ii=i; ii>=0; --ii) 
				decision_chromosome[ii] = parents.first->decision_chromosome[ii];
			
			//... and the rest from the second parent
			for(ii=decision_chromosome.size()-1; ii>i; --ii) 
				decision_chromosome[ii] = parents.second->decision_chromosome[ii];
		}
		
		//decide whether to mutate
		std::bernoulli_distribution mutate(mutation_rate);
		random_int = std::uniform_int_distribution<>(0, decision_chromosome.size() - 1);
		if( mutate(generator) ) ~decision_chromosome[random_int(generator)]; //flip a bit
		
		//generate 2 random numbers for link_chromosome crossover
		if( crossover(generator) ) {
			ii = random_int(generator);
			
			//take beginning of chromosome from first parent ...
			for(i=i; i>=0; --i) //over genetic nodes
				for(j=link_chromosome[0].size()-1; j>=0; --j) //over each connectivity vector
					link_chromosome[i][j] = parents.first->link_chromosome[i][j];
			
			//... and the rest from the second parent
			for(i=decision_chromosome.size()-1; i>ii; --i) 
				for(j=link_chromosome[0].size()-1; j>=0; --j)
					link_chromosome[i][j] = parents.second->link_chromosome[i][j];
		}
		
		//decide whether to mutate
		//mutations must occur in pairs to preserve K=2 connectivity
		random_int = std::uniform_int_distribution<>(0, link_chromosome.size() - 1);
		i = random_int(generator); //which bitset
		
		random_int = std::uniform_int_distribution<>(0, link_chromosome[0].size() - 1);
		j = random_int(generator); //which false bit to flip
		
		std::bernoulli_distribution random_bit(0.5);
		bool first = random_bit(generator); //which true bit to flip (there are only 2)
		
		ii = link_chromosome[0].size() - 1;
		if( mutate(generator) ) {
			for(ii; ii>=0; --ii) {
				if( link_chromosome[i][ii] ) { //true bit found
					if(first) { link_chromosome[i][ii] = false; break; }
					else first = true; //flip next true bit
				} 
			} 
			
			while(link_chromosome[i][j]) { //make sure bit is false to start with
				j = random_int(generator);
			} 
			//may reflip the same bit, but it doesn't matter; happens rarely
			link_chromosome[i][j] = true;
		} //if
		
	} //constructor
	
	template<unsigned int N, unsigned int I, unsigned int O>
	Genotype::~Genotype() {
		fitness->remove(ID);
	} //destructor

} //namespace john

