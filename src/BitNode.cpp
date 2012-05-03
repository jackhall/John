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

	BitNode::BitNode() 
		: parent(NULL), child_zero(NULL), child_one(NULL), 
		  boundary(0.0), value(false), branch_leaves(0) {} //use this for first root node
	
	BitNode::BitNode(BitNode* pParent, const bool zeroth_child) 
		: parent(pParent), child_zero(NULL), child_one(NULL), 
		  value(false), branch_leaves(1), boundary(0.0) {
		//for new leaf nodes (no boundary need be calculated yet)
		if(parent!=NULL) {
			if(zeroth_child) parent->child_zero = this;
			else parent->child_one = this;
			
			update_leaves();
		}
	}
	
	BitNode::BitNode(const bool bValue, const float fBoundary, 
			 BitNode* child0, BitNode* child1) 
		: parent(NULL), child_zero(child0), child_one(child1), 
		  boundary(fBoundary), value(bValue) {
		//for new root nodes (to expand an existing tree)
		if(child_zero!=NULL && child_one!=NULL) {
			child_zero->set_depth(1);
			child_one->set_depth(1);
			child_zero->parent = this;
			child_one->parent = this;
			branch_leaves = child_zero->branch_leaves + child_one->branch_leaves;
		}
	}
	
	BitNode::BitNode(const BitNode& rhs)
		: parent(NULL), child_zero(NULL), child_one(NULL), 
		  boundary(rhs.boundary), value(rhs.value),
		  branch_leaves(rhs.branch_leaves) {
		
		if(rhs.child_zero != NULL) {
			child_zero = new BitNode(*rhs.child_zero);
			child_zero->parent = this;
		}
		
		if(rhs.child_one != NULL) {
			child_one = new BitNode(*rhs.child_one);
			child_one->parent = this;
		}
	}
	
	BitNode::BitNode(BitNode&& rhs) 
		: parent(rhs.parent), child_zero(rhs.child_zero), child_one(rhs.child_one),
		  boundary(rhs.boundary), value(rhs.value),
		  branch_leaves(rhs.branch_leaves) {
		  	
		if(parent!=NULL) { //repeated in move operator
			if(parent->child_zero == &rhs) parent->child_zero = this;
			else parent->child_one = this;
		}
		
		if(child_zero!=NULL && child_one!=NULL) { //repeated in move operator
			child_zero->parent = this;
			child_one->parent = this;
		}
		
		rhs.parent = NULL;
		rhs.child_zero = NULL;
		rhs.child_one = NULL;
	}
	
	BitNode& BitNode::operator=(const BitNode& rhs) {
		if(this != &rhs) {
			if(child_one != NULL) delete child_one;
			if(child_zero != NULL) delete child_zero;
			
			value = rhs.value;
			boundary = rhs.boundary;
			branch_leaves = rhs.branch_leaves;
			
			child_zero = new BitNode(*rhs.child_zero);
			child_zero->parent = this;
			child_one = new BitNode(*rhs.child_one);
			child_one->parent = this;
		}
		
		return *this;
	}
	
	BitNode& BitNode::operator=(BitNode&& rhs) {	
		if(this != &rhs) {
			parent = rhs.parent;
			child_zero = rhs.child_zero;
			child_one = rhs.child_one;
			boundary = rhs.boundary;
			value = rhs.value;
			branch_leaves = rhs.branch_leaves;
			
			if(parent!=NULL) { //repeated in move constructur
				if(parent->child_zero == &rhs) parent->child_zero = this;
				else parent->child_one = this;
			}
			
			if(child_zero!=NULL && child_one!=NULL) { //repeated in move constructor
				child_zero->parent = this;
				child_one->parent = this;
			}
			
			rhs.parent = NULL;
			rhs.child_zero = NULL;
			rhs.child_one = NULL;
		}
		
		return *this;
	}
	
	BitNode::~BitNode() {
		if(child_zero!=NULL) delete child_zero;
		if(child_one!=NULL) delete child_one;
		if(parent!=NULL) {
			if(parent->child_one == this) parent->child_one = NULL;
			else parent->child_zero = NULL;
		}
	}
	
	float BitNode::find_boundary() const {
		float parent_upper_bound, parent_lower_bound;
		if(parent!=NULL && parent->parent!=NULL) {
			if(parent->parent->child_zero == parent) {
				parent_upper_bound = parent->parent->boundary;
				if(parent->child_zero == this) {
					if(parent->value)
						
					else
						
				} else {

				}
			} else {
				parent_lower_bound = parent->parent->boundary;
				if(parent->child_zero == this) {
					
				} else {
					
				}
			}
		} else return boundary;
	}
	
	void BitNode::split(const bool bValue) {
		//how does the node know where to place its new boundary? 
		//could just leave it alone and require the tree to
		//call update_boundary
		value = bValue;
		if(child_zero==NULL && child_one==NULL) {
			child_zero = new BitNode(this, true);
			child_one = new BitNode(this, false);
			branch_leaves = 2;
		}
	}
	
	void BitNode::update_boundary(const float lower_bound, const float upper_bound) {
		float ratio = 2.0/(1.0 + sqrt(5));
		if(child_zero!=NULL && child_one!=NULL) {
			if(value) boundary = lower_bound + ratio*(upper_bound - lower_bound);
			else boundary = lower_bound + (1-ratio)*(upper_bound - lower_bound);
			
			child_zero->update_boundary(lower_bound, boundary);
			child_one->update_boundary(boundary, upper_bound);
		}
	}
	
	void BitNode::update_leaves() {
		branch_leaves = child_zero->branch_leaves + child_one->branch_leaves;
		if(parent != NULL) parent->update_leaves();
	}
	
	unsigned short query(const float number) const {	
		if(child_zero != NULL) { //if child_zero is NULL, so should child_one be
			if(number > boundary) 
				return child_zero->branch_leaves + child_one->query(number);
			else 
				return child_zero->query(number);
		} else return 1; //for self
	}

} //namespace john

