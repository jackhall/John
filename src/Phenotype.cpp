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
	Phenotype<N,I,O>::Phenotype(Genotype<N,I,O>& genome) {	

		int n = N*N*2*2 + (I+O+1)*17*N;
		
		//extract boolean functions (4 bits each)
		auto itf = functions.begin(); //iterators over genetic nodes (25)
		auto itfe = functions.end();
		int i = 0; //current index in decision_chromosome
		int j; //temporary loop counter, reused
		while(itf != itfe) {
			//grab four bits and store them in the proper bitset
			for(j=3; j>=0; --j, ++i) (*itf)[j] = genome.decision_chromosome[i];
			++itf;
		}
		
		//extract input decision boundaries
		auto iti = input_decisions.begin(); //iterators for boolean genetic inputs (5)
		auto itie = input_decisions.end();
		array<real_type, I+1>::iterator it, ite; //iterators for inner arrays
		while(iti != itie) {
			//get iterators for inner array (one set of boundary parameters)
			it = iti->begin();
			ite = iti->end();
			
			//extract a float for each inner array element
			while(it != ite) {
				*it = get_real<n>(genome.decision_chromosome, i);
				++it; i+=17;
			}
			++iti;
		}
		
		//extract output weights
		auto ito = output_weights.begin(); //iterators for real outputs (7)
		auto itoe = output_weights.end();
		array<real_type, N>::iterator iter, itere; //iterators for inner arrays
		while(ito != itoe) {
			//get iterators for inner array (one set of output weights)
			iter = ito->begin();
			itere = ito->end();
			
			//extract a float for each inner array element
			while(iter != itere) {
				*iter = get_real<n>(genome.decision_chromosome, i);
				++iter; i+=17;
			}
			++ito;
		}
		
		//extract gene connectivity
		auto itl = genome.link_chromosome.begin();
		auto itc = links.begin();
		auto itce = links.end();
		while(itc != itce) {
			j = 0; //no links recorded for current genetic node
			for(i=itl->size()-1; i>=0; --i) {
				if( (*itl)[i] ) { 
					(*itc)[j] = i; //record index of origin genetic node
					++j; //increment link counter
				}
				if(j == 2) break; //if both links are found, stop looking
			}
			++itc; ++itl;
		}
		
	} //constructor

	template<unsigned int N, unsigned int I, unsigned int O>
	real_type Phenotype<N,I,O>::get_real(std::bitset<N*N*2*2 + (I+O+1)*17*N>& sequence, 
					     const unsigned int start) const {
		/*
		Extracts a floating point number from a bit sequence. The number is encoded
		with a sign and two integers, as shown below. This coding is compact and keeps
		numbers from getting unreasonably small or large. It should improve the 
		evolutionary fitness surface over normal floating point numbers. 
		*/
		if(sequence[start]) sign = 1.0;
		else sign = -1.0;
		a = get_integer(sequence, start+1); 
		b = get_integer(sequence, start+9); //8-bit integers
		return sign*a/(1.0 + b); 
	} //get_real

	template<unsigned int N, unsigned int I, unsigned int O>
	unsigned long Phenotype<N,I,O>::get_integer(std::bitset<N*N*2*2 + (I+O+1)*17*N>& sequence, 
						    unsigned int start) const {
		/*
		Translates an 8-bit portion of a bit sequence to an integer. The integers are
		coded in the genome with Gray's coding, which hopefully improves the fitness
		surface by smoothing it out. This method may segfault if the sequence isn't long
		enough (there is no bounds checking).
		*/
		std::bitset<8> integer;
		unsigned int i=start, j=0;
		for(i, j; i<(start+8); ++i, ++j) integer[j] = sequence[i];
		return gray_to_binary( integer.to_ulong() );
	} //get_integer
	
	template<unsigned int N, unsigned int I, unsigned int O>
	unsigned long Phenotype<N,I,O>::gray_to_binary(unsigned long num) const {
		/*
		Translates a binary number in Gray's code to the same number in base 2. Got
		this from the internet, so I hope it works. 
		*/
		unsigned int numBits = 8 * sizeof(num);
		unsigned int shift;
	    	for (shift = 1; shift < numBits; shift *= 2) {
			num ^= num >> shift;
	    	}
	    	return num;
	} //gray_to_binary
	
	template<unsigned int N, unsigned int I, unsigned int O>
	void Phenotype<N,I,O>::run(const real_type value, const real_type dvalue, const real_type persistence) {
		//run decision boundaries on inputs
		std::array< std::array<real_type, I+1>, N >& w = input_decisions; //shorthand
		int i;
		for(i=(N-1); i>=0; --i) 
			state[i] = ( w[i][0]*value + w[i][1]*dvalue 
				    +w[i][2]*persistence + w[i][3] ) > 0;
		
		//evaluate boolean network (update state)
		bitset<N*N> new_state;
		for(i=(N*N-1); i>=0; --i) 
			new_state[i] = gene_fcn( i, state[ links[i][0] ], state[ links[i][1] ] );
		
		for(i=(N*N-1), i>=0; --i) state[i+N] = new_state[i];

		//calculate and store current output values
		std::array< std::array<real_type, N>, O >& v = output_weights; //shorthand
		std::array<real_type, O> outputs;
		for(i=(O-1); i>=0; --i) {
			//calculate values from v and states
			outputs[i] = 0.0;
			for(int j=(N-1); j>=0; --j) outputs[i] += v[i][j] * state[N*N+j];
		}
		//run each element of output through a sigmoid
		learning_rate_val = sigmoid(outputs[0]);
		momentum_val 	  = sigmoid(outputs[1]);
		weight_decay_val  = sigmoid(outputs[2]);
		forget_factor_val = sigmoid(outputs[3]);
		kill_link_prob	  = sigmoid(outputs[4]);
		make_link_prob 	  = sigmoid(outputs[5]);
		make_node_prob	  = sigmoid(outputs[6]);
	} //run
	
	template<unsigned int N, unsigned int I, unsigned int O>
	real_type Phenotype<N,I,O>::sigmoid(const real_type x) const {
		return 1/(1 + exp(-x));
	}
	
	template<unsigned int N, unsigned int I, unsigned int O>
	bool Phenotype<N,I,O>::gene_fcn(const unsigned int gene_index, const bool a, const bool b) const {
		if(a) {
			if(b) return functions[gene_index][3];
			else  return functions[gene_index][2];
		} else {
			if(b) return functions[gene_index][1];
			else  return functions[gene_index][0];
		}
	}
	
	template<unsigned int N, unsigned int I, unsigned int O>
	bool Phenotype<N,I,O>::flip_coin(const real_type probability) {
		//generate random bit from given probability
		random_bit = std::bernoulli_distribution(probability);
		return random_bit(generator);
	} //flip_coin
	
} //namespace john


////////////////////////////////////////////////////
/*
//        The purpose of this function is to convert an unsigned
//        binary number to reflected binary Gray code.

unsigned int binaryToGray(unsigned int num)
{
        return (num>>1) ^ num;
}
 
//        The purpose of this function is to convert a reflected binary
//        Gray code number to a binary number.

unsigned int grayToBinary(unsigned int num)
{
    unsigned int numBits = 8 * sizeof(num);
    unsigned int shift;
    for (shift = 1; shift < numBits; shift *= 2)
    {
        num ^= num >> shift;
    }
    return num;
}
*/

