#ifndef BitNode_h
#define BitNode_h

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

#include <cmath>

namespace john {

	class BitNode {
	private:
		BitNode* parent, child_zero, child_one;
		unsigned short depth, branch_leaves; //does depth need to be a field anymore?
		float boundary;
		
		void set_depth(const short new_depth);
		void set_parent(BitNode* pParent);
		
	public:
		bool value;
		
		BitNode();
		BitNode(BitNode* pParent, const bool child);
		BitNode(const bool bValue, const float range, BitNode* child0=NULL, BitNode* child1=NULL);
		BitNode(const BitNode& rhs);
		BitNode(BitNode&& rhs);
		BitNode& operator=(const BitNode& rhs);
		BitNode& operator=(BitNode&& rhs);
		~BitNode();
		
		void split(const bool bValue);
		void update_boundary(const float lower_bound, const float upper_bound);
		void update_leaves();
		unsigned short query(const float number) const;
		
	}; //class BitNode
	
} //namespace john

#endif

