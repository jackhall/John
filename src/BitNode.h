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

namespace john {

	class BitNode {
	private:
		BitNode* parent, child_zero, child_one;
		unsigned short depth;
		float boundary;
		bool value;
		
	public:
		BitNode() = delete;
		BitNode(const bool bValue, const float fBoundary);
		explicit BitNode(const bool bValue, const BitNode* pParent);
		BitNode(const BitNode& rhs);
		BitNode& operator=(const BitNode& rhs);
		~BitNode();
	
		bool operator<(const BitNode& rhs) const;
		void update_boundary();
		void update_pointers();
		void split();
		void join();
		
		void query(const float number, std::vector<bool>& binary) const;
		
	}; //class BitNode
	
} //namespace john

#endif

