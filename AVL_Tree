/*****************************************
 * UW User ID:  Jachiang
 * Submitted for ECE 250
 * Semester of Submission:  (Winter) 2018
 *
 * By  submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

//Implement Gaurd Statements to avoid having the same file included twice. Avoids Duplicate Definitions.
#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <iostream> //Input/Output Library
#include "Exception.h" //Include Exception file to throw exceptions at methods that the compiler cannot handle.
#include "ece250.h" //Testing Dynamic Memory File
#include <cassert> //Includes the macro, assert, to verify assumptions made by the program. Program crashes if expression is false


template <typename Type> //Add Template to store/manipulate different variable types for the deque.
class Search_tree { 
	public:
		class Iterator; //Iterator will be used to access nodes and their values from within the AVL tree.

	private:
		class Node {
			public:
				Type node_value; //Variable containing the object stored at this node.
				int tree_height; //Variable storing the height of the node.

				//The left and right sub-trees
				Node *left_tree; //Pointer that points to the left sub-tree.
				Node *right_tree; //Pointer that points to the right sub-tree.

				//Iterator Variables:
				//Point to the previous and next nodes in linear order
				//These pointers will be used for the Iterator.
				Node *previous_node; //Pointer that points to the previous node (i.e. the largest value lower than current value) in linear order.
				Node *next_node; //Pointer that points to the next node (i.e. the smallest value higher than current value) in linear order.

				// Member functions
				Node( Type const & = Type() ); //Node Constructor

				void update_height(); //Function that will update the height of the tree depending on where the node was added.
				
				int height() const; //Returns the height of the node from the leaf node.
				bool is_leaf() const; //Checks if the node is a leaf node (i.e. No children).
				Node *front(); //Function that returns the address of the minimum object in the tree.
				Node *back(); //Function that returns the address of the maximum object in the tree.
				Node *find( Type const &obj ); //Function will traverse the tree and checks if the object is stored within the tree.

				void clear(); //Deletes all descendents of the root node and then the root node itself.
				bool insert( Type const &obj, Node *&to_this ); //Inserts a new node into the tree. Accounts for Balancing.
				bool erase( Type const &obj, Node *&to_this ); //Erases a node in the tree. Accounts for Balancing.
				
				//Additional Variables/Functions for Balancing
				int balance_factor() const; //Function that returns the balance factor for respective node.
				Search_tree<Type>::Node* balance_check(); //Function that checks goes through insertion node path and implements balance_factor(). It then decides if a balance is needed. 

				Search_tree<Type>::Node* LL_Rotate(); //Performs Left-Left Rotation. a-b-c (height of a>c) where b is inserted node.
				Search_tree<Type>::Node* LR_Rotate(); //Performs Double Rotation (Left-Right). c-a-b (height of a>c) where b is inserted node.
				Search_tree<Type>::Node* RL_Rotate(); //Performs Double Rotation (Right-Left). c-a-b (height of a<c) where b is inserted node.
				Search_tree<Type>::Node* RR_Rotate(); //Performs Right-Right Rotation. a-b-c (height of a<c) where b is inserted node.
		
		};

		Node *root_node; //Root Node Pointer.
		int tree_size; //Variable stores the number of descendents of the root node.

		// Hint as to how to start your linked list of the nodes in order
		//Used for Iterator. 
		Node *front_sentinel; //Pointer pointing to the front sentinel.
		Node *back_sentinel; //Pointer pointing to the back sentinel.

	public:
		class Iterator {
			private:
				Search_tree *containing_tree; //Pointer that can access member variables/functions in the search tree.
				Node *current_node; //Pointer that can access member variables/functions in the current node.
				bool is_end; 

				// The constructor is private so that only the search tree can create an iterator
				Iterator(Search_tree *tree, Node *starting_node); //Constructor

			public:
				// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
				Type operator*() const;
				Iterator &operator++(); //Update iterator to point to the next position.
				Iterator &operator--(); //Update iterator to point to the previous position.
				bool operator==( Iterator const &rhs ) const;
				bool operator!=( Iterator const &rhs ) const;

			// Make the search tree a friend so that it can call the constructor
			friend class Search_tree;
		};

		// DO NOT CHANGE THE SIGNATURES FOR ANY OF THESE
		Search_tree(); //AVL Tree Constructor
		~Search_tree(); //AVL Tree Destructor

		bool empty() const; //Checks if AVL Tree is empty. 
		int size() const; //Returns the number of objects in the AVL tree.
		int height() const; //Returns the height of the AVL Tree.
		Type root_value();

		Type front() const; //Function that returns the address of the minimum object in the tree.
		Type back() const; //Function that returns the address of the maximum object in the tree.

		Iterator begin(); //Function returning the first position of the tree pointed to by head sentinel (i.e. minimum object address). 
		Iterator end(); //Function returning the tail sentinel address.
		Iterator rbegin(); //Function returning the last position of the tree pointed to by tail sentinel (i.e. maximum object address).
		Iterator rend(); //Function returining the head sentinel address.
		Iterator find( Type const & ); //Function finding the object of interest.

		void clear(); //Clears the AVL Tree by deleting all nodes.
		bool insert( Type const & ); //Function that inserts a new node into the AVL Tree.
		bool erase( Type const & ); //Function that erases a node in the AVL Tree.

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Search_tree<T> const & );
};

//////////////////////////////////////////////////////////////////////
//                Search Tree Public Member Functions               //
//////////////////////////////////////////////////////////////////////

// The initialization of the front and back sentinels is a hint
template <typename Type>
Search_tree<Type>::Search_tree(): //Constructor
root_node(nullptr), //Root node initially has nothing.
tree_size(0), //Number of Descendents is currently 0
front_sentinel(new Search_tree::Node(Type())), //Dynamically allocate the front sentinel. 
back_sentinel(new Search_tree::Node(Type())){ //Dynamically allocate the back sentinel.
	//Both sentinels will point at each other since the tree is empty.
	front_sentinel->next_node = back_sentinel; 
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
Search_tree<Type>::~Search_tree() { //Destructor
	//Since we have a function that already dynamically deletes our tree, we can just call it.
	clear();  // might as well use it...
	delete front_sentinel; //Dynamically delete the sentinels.
	delete back_sentinel;
}

template <typename Type>
bool Search_tree<Type>::empty() const {
	return (root_node==nullptr);  // If the root node has nothing and points to nullptr, we can say that the tree is empty.
}
template <typename Type>

Type Search_tree<Type>::root_value()  {
	return (root_node->node_value);  // If the root node has nothing and points to nullptr, we can say that the tree is empty.
}

template <typename Type>
int Search_tree<Type>::size() const { 
	return tree_size; //Returns the number of objects stored in the AVL tree.
}

template <typename Type>
int Search_tree<Type>::height() const { //Returns the height of the tree.
	return root_node->height(); //Start at the root node and implement the node height function (Node::height()) to get the height of the tree.
}

template <typename Type>
Type Search_tree<Type>::front() const {
	if (empty()){
		throw underflow(); //If there are no values stored, throw underflow exception.
	}
	//Start at the root node and implement the node front function (Node::front()) to get the minimum address of the object and then extracts the value stored.
	return root_node->front()->node_value;
}

template <typename Type>
Type Search_tree<Type>::back() const {
	if (empty()){
		throw underflow(); //If there are no values stored, throw underflow exception.
	}
		//Start at the root node and implement the node back function (Node::back()) to get the maximum address of the object and then extracts the value stored.
	return root_node->back()->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::begin() { 
	//Search tree pointer will be the object which we created. We will use the object to tell the tree what to do.
	//The current node will be initialized to the minimum node address. If the tree is empty, the node will be front sentinel pointed to by back sentinel.
	return empty() ? Iterator(this, back_sentinel) : Iterator(this, root_node->front()); //Use node front (Node::front()) function to find the address of the minimum object. 
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::end() {
	return Iterator( this, back_sentinel ); //The node will always be whatever the back sentinel points to which contains the maximum object.
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rbegin() {
	//Search tree pointer will be the object which we created. We will use the object to tell the tree what to do.
	//The current node will be initialized to the maximum node address. If the tree is empty, the node will be back sentinel pointed to by front sentinel.
	return empty() ? Iterator(this, front_sentinel) : Iterator(this, root_node->back()); //Use node back (Node::back()) function to find the address of the maximum object. 
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::rend() {
	return Iterator( this, front_sentinel ); //The node will always be whatever the front sentinel points to which contains the minimum object.
}

template <typename Type>
typename Search_tree<Type>::Iterator Search_tree<Type>::find( Type const &obj ) {
	if (empty()) {
		return Iterator(this, back_sentinel); //If the tree is empty, then obj is not there.
	}
	
	//Create a pointer that will store the address of the node containing the object.
	typename Search_tree<Type>::Node *search_result = root_node->find(obj); //Starting at the root node, use the node find function (Node::find()) to locate the object. 

	if (search_result == nullptr) {
		return Iterator(this, back_sentinel); //If the node find function couldn't find the object, it will return nullptr to our pointer. 
	} else {
		return Iterator( this, search_result ); //If the node find function finds the object, then the Iterator will store the address of that node.
	}
}

template <typename Type>
void Search_tree<Type>::clear() {
	if (!empty()){ 
		root_node->clear(); //Use the node clear function (Node::clear()) to clear every variable/pointer of the associated object.
		root_node = nullptr; //Set root node back to nullptr.
		tree_size = 0; //The number of objects in the tree is set back to 0.
	}

	// Reinitialize the sentinels
	front_sentinel->next_node = back_sentinel;
	back_sentinel->previous_node = front_sentinel;
}

template <typename Type>
bool Search_tree<Type>::insert(Type const &obj) {
	if (empty()){ 
		root_node = new Search_tree::Node(obj); //If the tree is empty, make the root node a new node
		tree_size = 1; //Increment tree size by 1.
		
		root_node->previous_node = back_sentinel->previous_node;
		root_node->next_node = front_sentinel->next_node;
		front_sentinel->next_node = root_node;
		back_sentinel->previous_node = root_node;
		
		return true; //Return true if insertion is successful
	} else if (root_node->insert(obj, root_node)) { //If the root node is present, use the node insert function (Node::insert()) to insert descendent nodes.
		
		Node object;
		front_sentinel->next_node = object.front();
		back_sentinel->previous_node = object.back();
		
		++tree_size; //Increment the tree size.
		return true; //Return true if insertion is successful.
	} else {
		return false; //Return false if insertion is unsuccessful. Typically will happen if the user wants to insert a duplicate value.
	}
}

template <typename Type>
bool Search_tree<Type>::erase(Type const &obj) {
	if (!empty() && root_node->erase(obj, root_node)) { //If the tree is not empty, start at root node and use the node erase function (Node::erase()) to find and delete the node of interest.
		--tree_size; //Decrement the tree size if erase is successful.
		return true; //Return true if deletion is successful.
	} else {
		return false; //Return false if deletion is unsuccessful. Typically will happen if the node of interest is not there.
	}
}

//////////////////////////////////////////////////////////////////////
//                   Node Public Member Functions                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Node::Node( Type const &obj ):
node_value(obj), //Variable will store the node value.
left_tree(nullptr), //Node does not contain any children thus far.
right_tree(nullptr),
next_node(nullptr), //Node does not have any nodes in the iterator.
previous_node(nullptr),
tree_height(0){ //This node is located at tree height 0.
	// Leave Constructor Empty since we used an Initilization list.
}

template <typename Type>
void Search_tree<Type>::Node::update_height() { //We will use this function whenever we insert or remove a node.
	//Left_tree and right_tree for the parent node have to be updated before proceeding with the child nodes. Height will be updated starting at the root node.
	tree_height=std::max(left_tree->height(),right_tree->height()) + 1; //Add 1 always. The height function will determine the new node height.
}

template <typename Type>
int Search_tree<Type>::Node::height() const {
	//If the node is empty, then it is a leaf node. Therefore, returning -1 will cancel +1  when calculating tree_height in the update_height function.
	//Otherwise, the node is not a leaf node and will return the tree height of its children node which will be added by 1 in the update_height function.
	return (this==nullptr) ? -1 : tree_height;
}

template <typename Type>
bool Search_tree<Type>::Node::is_leaf() const {
	return left_tree==nullptr && right_tree==nullptr; //If current node's left tree and right tree have nothing, the current node is a leaf.	
}

// Return a pointer to the front node; Finds the minimum object address in the AVL tree.
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::front() {
	//If the node has no more left sub trees, we can say that we are at the minimum node. If not, go to the next left subtree and repeat.
	return (left_tree==nullptr) ? this : left_tree->front(); //"this" pointer would be the current node address.
}

// Return a pointer to the back node; Finds the maximum object address in the AVL tree.
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::back() {
	//If the node has no more right sub trees, we can say that we are at the maximum node. If not, go to the next right subtree and repeat.
	return (right_tree==nullptr) ? this : right_tree->back(); //"this" pointer would be the current node address.
}

// Recursively traverse tree to find nodes containing value specified in argument. This function does not look for duplicates.
template <typename Type>
typename Search_tree<Type>::Node *Search_tree<Type>::Node::find( Type const &obj ) {
	if (obj==node_value){
		return this; //If node contains value equaled to the argument, return the value's address.
	}else if (obj<node_value){
		return (left_tree==nullptr) ? nullptr : left_tree->find(obj); //If the node value is smaller than argument, go to the left sub tree. 
		//Nullptr will be returned to the Search Tree Pointer if value is not found.
	}else{
		return (right_tree==nullptr) ? nullptr : right_tree->find(obj); //If the node value is larger than argument, go to the right sub tree.
		//Nullptr will be returned to the Search Tree Pointer if value is not found.
	}
}

// Recursively clear the tree
template <typename Type>
void Search_tree<Type>::Node::clear() {
	if (left_tree!=nullptr){
		left_tree->clear(); // Clear the tree using post order traversal by clearing the bottom nodes first, left to right.
	}
	if (right_tree!=nullptr){
		right_tree->clear();
	}
	delete this; //Deallocate the Node.
}

template <typename Type>
bool Search_tree<Type>::Node::insert( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	Node *potential_new_root;
	if (obj<node_value){ //If the value is less than the current value, look at the right tree.
		if (left_tree==nullptr) { 
			left_tree=new Search_tree<Type>::Node(obj);  //If the left tree has no child, we can insert the node there.
			
			//Set Pointers for Iterating. Set these since there may be a parent node that is smaller than this value.
			left_tree->previous_node = this->previous_node; //Set the new node (left tree) to point to the parent node of the current node.
			this->previous_node->next_node = left_tree; //Set the parent node of the current node to point to the new node (left tree).
			//Set Pointers for Iterating.
			left_tree->next_node = this; //The next larger node will be the parent node.
			this->previous_node = left_tree; //The previous smaller node of the parent node is the parent's node left tree. 
			
			
			update_height(); //Update the height of the node.
			
			return true; //Return true since insertion is successful
		} else { 
			if (left_tree->insert(obj,left_tree)) { //If the left tree has a node already, we will recursively insert by going to the left tree.
				update_height(); //Update the height of the node.
				
				//AVL Balance Checking. We will recursively go down the same path and check each node when balancing.
				potential_new_root = balance_check(); //Balance_check function will return a potential new root if a rotation was done at the root.
				if (this->height()<potential_new_root->height()){ 
					to_this = potential_new_root; //If this node has a height larger than our current root height, then this will be our new root node.
				}
				
				return true; //Return true since insertion is successful
			} else {
				return false; //Return false if insertion is unsuccessful (Duplicate value).
			}
		}
}
	else if (obj>node_value){ //If the value is more than the current value, look at the right tree.
		if (right_tree==nullptr){
			right_tree = new Search_tree<Type>::Node(obj); //If the right tree has no child, we can insert the node there.
			
			//Set Pointers for Iterating: Set these since there may be a parent node that is larger than this value. 
			right_tree->next_node = this->next_node; //Set the new node (right tree) to point to the parent node of the current node.
			this->next_node->previous_node = right_tree; //Set the parent node of the current node to point to the new node (right tree).
			
			//Set Pointers for Iterating.
			right_tree->previous_node = this; //The previous smaller node will be the parent node.
			this->next_node = right_tree; //The next larger node of the parent node is the parent's node right tree.
			
			
			update_height(); //Update the height of the node.
			
			return true; //Return true since insertion is successful
		}else{
			if (right_tree->insert(obj,right_tree)){ //If the right tree has a node already, we will recursively insert by going to the right tree.
			    update_height(); //Update the height of the node.
				
				//AVL Balance Checking. We will recursively go down the same path and check each node when balancing.
				potential_new_root=balance_check(); //Balance_check function will return a potential new root if a rotation was done at the root.
				if (this->height()<potential_new_root->height()){ 
					to_this = potential_new_root; //If this node has a height larger than our current root height, then this will be our new root node.
				}
				
				return true; //Return true if the next right sub tree insertion was successful.
			}else{
				return false; //Return false if insertion is unsuccessful (Duplicate value).
			}
		}
	}else{
		return false; //We do not allow duplicates so the system will return false.
	}
}

template <typename Type>
bool Search_tree<Type>::Node::erase( Type const &obj, Search_tree<Type>::Node *&to_this ) {
	Node *potential_new_root;
	if (obj<node_value){ //If the value is less than the value to be erased, look at the left tree.
		if (left_tree==nullptr){
			return false; //If the left tree has no values, then the value does not exist.
		} else {
			if (left_tree->erase(obj,left_tree)){ //Recursively implement the erase function for the next subtree.
				update_height(); //Update the height of the nodes.
				
				potential_new_root=balance_check(); //Balance_check function will return a potential new root if a rotation was done at the root.
				if (this->height()<potential_new_root->height()){ 
					to_this = potential_new_root; //If this node has a height larger than our current root height, then this will be our new root node.
				}
				
				return true; //Return true if the value is erased somewhere in the left sub tree.
			}
			return false; //Return false if there was no value to be erased somewhere in the left sub tree.
		}
	} else if (obj>node_value){ //If the value is more than the value to be erased, look at the right tree.
		if (right_tree==nullptr){
			return false; //If the right tree has no values, then the value does not exist.
		} else {
			if (right_tree->erase(obj,right_tree)){ //Recursively implement the erase function for the next subtree.
				update_height(); //Update the height of the nodes.
				
				potential_new_root=balance_check(); //Balance_check function will return a potential new root if a rotation was done at the root.
				if (this->height()<potential_new_root->height()){ 
					to_this = potential_new_root; //If this node has a height larger than our current root height, then this will be our new root node.
				}
				
				return true; //Return true if the value is erased somewhere in the right sub tree.
			}
			return false; //Return false if there was no value to be erased somewhere in the right sub tree.
		}
	} else {
		assert(obj==node_value);

		if (is_leaf()){ //If the node is a leaf node, we can safely delete it without having to repoint any subtrees.
			to_this = nullptr; //Turn this node into an empty node for its parent node.
			delete this; 
		} else if (left_tree==nullptr){  
			to_this = right_tree; //If the node has a right child, we will have to repoint its right subtree to its parent.
			delete this;
		} else if (right_tree==nullptr){
			to_this = left_tree; //If the node has a left child, we will have to repoint its left subtree to its parent.
			delete this;
		} else{ 
			node_value = right_tree->front()->node_value; //If the node had two child nodes, we will have to make the current node hold the right value in its right tree.
			right_tree->erase( node_value, right_tree ); //The right tree will then be erased instead of the current node.
			update_height(); //Update the height of the nodes.
		}
		//Redirecting pointers in nodes for Iterating.
		this->previous_node->next_node = this->next_node; //Set the deleted node's previous node to point to the deleted node's next node.
		this->next_node = this->previous_node; //Set the deleted node's next node to point to the deleted node's previous node.
		return true; //Erasing the node means that the command was successful to the user.
	}

}

template <typename Type>
int Search_tree<Type>::Node::balance_factor() const { //The balance factor function will tell us if the left and right side of a node is balanced or not.
	int left_height=1; //Initialize the left and right heights. These will be 1 because the height function will return -1 if it is nullptr.
	int right_height=1;

	if (left_tree!=nullptr){ //Check left sub tree for node of interest.
		left_height=left_tree->height()+1; //If the left sub tree is a leaf node, then height function will return -1 cancelling the +1. If not, then it will be that tree height +1.
	}
	if (right_tree!=nullptr){ //Check right sub tree for node of interest.
		right_height=right_tree->height()+1; //If the right sub tree is a leaf node, then height function will return -1 cancelling the +1. If not, then it will be that tree height +1.
	}
	return (left_tree->height()-right_tree->height()); //The difference between the heights is the balance factor.
}

template <typename Type>
typename Search_tree<Type>::Node* Search_tree<Type>::Node::balance_check() { //The balance_check function will tell us whether we would need to balance the tree.
	Node *root; //Initialize a temporary node to store a potential root node.
	if (balance_factor()==-2){ //If the right tree higher than the left tree by 2, implement a balance.
		//Determine if the inserted node was inserted to the left or right sub tree of the child nodes.
		if (right_tree->balance_factor()==-1) {
			root = LL_Rotate(); //If child right_tree is higher than the child left_tree by 1, then we know that the path is a-b-c (height a>c), where b is inserted node.

		}
		else{
			root = LR_Rotate(); //Else, the case will be vice versa and a left-right rotation would be required.
		}  
    } else if (balance_factor()==2){ //If the left tree higher than the right tree by 2, implement a balance.
		if (left_tree->balance_factor()==1){
			root = RR_Rotate(); //If child left_tree is higher than the child right_tree by 1, then we know that the path is a-b-c (height a<c), where b is inserted node.
		}
		else {
			root = RL_Rotate(); //Else, the case will be vice versa and a right-left rotation would be required.
		}
	}
//	std::cout << root->node_value;
	return root; //Return temporary node storing the potential root node.
}

template <typename Type>
typename Search_tree<Type>::Node* Search_tree<Type>::Node::LL_Rotate(){ //BALANCE CASE 1: IN THE LECTURE SLIDES
	Node *temp = right_tree; //temp stores b.
	right_tree = temp->left_tree; //a's right tree will contain b's left tree.
	temp->left_tree = this; //b's left tree will now be a.
	update_height(); //Update the heights
	temp->update_height();
	
	return temp; //Return this node since it could potentially be the new root node.
}

template <typename Type>
typename Search_tree<Type>::Node* Search_tree<Type>::Node::RR_Rotate(){ //BALANCE CASE 1: IN THE LECTURE SLIDES
	Node *temp = left_tree; //temp stores b.
	left_tree = temp->right_tree; //c's left tree will contain b's right tree.
	temp->right_tree = this; //b's right tree will now be c.
	update_height(); //Update the heights.
	temp->update_height();
	
	return temp; //Return this node since it could potentially be the new root node.
}

template <typename Type>
typename Search_tree<Type>::Node* Search_tree<Type>::Node::LR_Rotate(){ //BALANCE CASE 2: IN THE LECTURE SLIDES. KEEP IN MIND THAT AVL TREE TEST ON WEBSITE DOES NOT ALWAYS BALANCE THIS WAY.
	Node *temp = right_tree->left_tree; //temp stores b.
	right_tree->left_tree = temp->right_tree; //c will store whatever b was holding in its right subtree.
	temp->right_tree = right_tree; //b right tree will point to c.
	right_tree = temp->left_tree; //a's right tree will store whatever b was holding in its left subtree.
	temp->left_tree = this; //b left tree will point to a.
	
	update_height(); //Update the heights.
	temp->right_tree->update_height();
	temp->update_height();
	
	return temp; //Return this node since it could potentially be the new root node.
}

template <typename Type>
typename Search_tree<Type>::Node* Search_tree<Type>::Node::RL_Rotate(){ //BALANCE CASE 2: IN THE LECTURE SLIDES. KEEP IN MIND THAT AVL TREE TEST ON WEBSITE DOES NOT ALWAYS BALANCE THIS WAY.
	Node *temp = left_tree->right_tree; //temp stores b.
	left_tree->right_tree = temp->left_tree; //a will store whatever b was holding in its left subtree.
	temp->left_tree = left_tree; //b's left tree will point to a.
	left_tree = temp->right_tree; //a's left tree will store whatever b was holding in its right subtree.
	temp->right_tree = this; //b right tree will point to c.
	
	update_height(); //Update the heights.
	temp->left_tree->update_height();
	temp->update_height();
	
	return temp; //Return this node since it could potentially be the new root node.
}



//////////////////////////////////////////////////////////////////////
//                   Iterator Private Constructor                   //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Search_tree<Type>::Iterator::Iterator( Search_tree<Type> *tree, typename Search_tree<Type>::Node *starting_node ):
containing_tree( tree ), //The Object.
current_node( starting_node ) { //The Node.
	// This is done for you...
	// Does nothing...
}

//////////////////////////////////////////////////////////////////////
//                 Iterator Public Member Functions                 //
//////////////////////////////////////////////////////////////////////

template <typename Type>
Type Search_tree<Type>::Iterator::operator*() const {
	// This is done for you...
	return current_node->node_value;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator++() {
	// Update the current node to the node containing the next higher value
	// If we are already at end do nothing
	current_node = current_node->next_node; //Iterator's current node will go to the next node.
	// Your implementation here, do not change the return value

	return *this;
}

template <typename Type>
typename Search_tree<Type>::Iterator &Search_tree<Type>::Iterator::operator--() {
	// Update the current node to the node containing the next smaller value
	// If we are already at either rend, do nothing
	current_node = current_node->previous_node; //Iterator's current node will go to the previous node.
	// Your implementation here, do not change the return value
	return *this;
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator==( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node == rhs.current_node );
}

template <typename Type>
bool Search_tree<Type>::Iterator::operator!=( typename Search_tree<Type>::Iterator const &rhs ) const {
	// This is done for you...
	return ( current_node != rhs.current_node );
}

//////////////////////////////////////////////////////////////////////
//                            Friends                               //
//////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Search_tree<T> const &list ) {
	
	for ( Search_tree<int>::Iterator itr = list.begin(); itr != list.end(); ++itr ) {
		out << *itr << " ";
	}

	return out;
}

#endif
