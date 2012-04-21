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

	BitNode::BitNode(const bool bValue, const float fBoundary)
		: parent(NULL), child_zero(NULL), child_one(NULL), depth(0),
		  boundary(fBoundary), value(bValue) {}
	
	BitNode::BitNode(const bool bValue, const BitNode* pParent) 
		: parent(pParent), child_zero(NULL), child_one(NULL), 
		  depth(++parent->depth), value(bValue) {
		
		boundary = //function of bValue, parent->bValue, and parent->boundary
	}
	
	BitNode::BitNode(const BitNode& rhs) 
		: parent(rhs.parent), child_zero(rhs.child_zero), child_one(rhs.child_one),
		  depth(rhs.depth), boundary(rhs.boundary), value(rhs.value) {
	
		update_pointers();
		rhs.parent = NULL;
		rhs.child_zero = NULL;
		rhs.child_one = NULL;
	}
	
	BitNode& BitNode::operator=(const BitNode& rhs) {
		
		parent = rhs.parent;
		child_zero = rhs.child_zero;
		child_one = rhs.child_one;
		depth = rhs.depth;
		boundary = rhs.boundary;
		value = rhs.value;
		
		update_pointers();
		rhs.parent = NULL;
		rhs.child_zero = NULL;
		rhs.child_one = NULL;
	}
	
	bool BitNode::operator<(const BitNode& rhs) const {
		if(depth < rhs.depth) return true;
		else if(depth > rhs.depth) return false;
		else return this<(&rhs);	
	}
	
	void BitNode::update_boundary() {
	
	}
	
	void BitNode::update_pointers() {
		if(parent!=NULL && child_zero!=NULL && child_one!=NULL) {
			if(value) parent->child_one = this;
			else parent->child_zero = this;
			child_zero->parent = this;
			child_one->parent = this;
		}
	}
	
	void BitNode::split() {
	
	}
	
	void BitNode::join() {
	
	}
	
	void query(const float number, std::vector<bool>& binary) const {
		if(number < boundary) 
			if(child_one!=NULL) child_one->query(number, binary);
		else 
			if(child_zero!=NULL) child_zero->query(number, binary);
			
		binary.push_back(value);
	}

} //namespace john
