/*****************************************
 * UW User ID:  Jachiang
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

//Implement Gaurd Statements to avoid having the same file included twice. Avoids Duplicate Definitions.
#ifndef DOUBLE_SENTINEL_LIST_H 
#define DOUBLE_SENTINEL_LIST_H

#include <iostream> //Input/Output Library
#include "Exception.h" //Include this so that we can throw exceptions in certain methods if we know the compiler cannot handle it.
using namespace std;
template <typename Type> //Add a Template so we can store/manipulate different variable types in the Linked List.
class Double_sentinel_list {
	public:
		class Double_node {
			public:
				Double_node(Type const & = Type(), Double_node * = nullptr , Double_node * = nullptr ); //Each Double_node contains a value with previous and next pointers.
				
				Type value() const; //Function that returns the value of the object.
				Double_node *previous() const; //Function that returns address of previous node. 
				Double_node *next() const; //Function that returns address of next node 

				Type         node_value; //Variable that stores the value.
				Double_node *previous_node; //Pointer storing the previous address.
				Double_node *next_node; //Pointer storing the next address.
	};

		Double_sentinel_list(); //Constructor
		Double_sentinel_list( Double_sentinel_list const & ); //Copy Constructor. Create a copy of a Doubly Linked List.
		Double_sentinel_list( Double_sentinel_list && ); //Move Constructor
		~Double_sentinel_list(); //Destructor

		// Accessors

		int size() const; //Returns number of items in the doubly linked list.
		bool empty() const; //Returns true if list is empty.

		Type front() const; //Returns object of the node pointed to by the next pointer of the sentinel head.
		Type back() const; //Returns object of the node pointed to by the previous pointer of the sentinel tail.

		Double_node *begin() const; //Returns address of next node pointed to by head sentinel node.
		Double_node *end() const; //Returns address of tail sentinel node.
		Double_node *rbegin() const; //Returns address of of previous node pointed to by tail sentinel node.
		Double_node *rend() const; //Returns address of head sentinel node.

		Double_node *find( Type const & ) const; //Returns the address of the first node in doubly linked list that stores a value equal to the argument
		int count( Type const & ) const; //Returns the number of nodes in doubly linked list that stores a value equal to the argument.

		// Mutators

		void swap( Double_sentinel_list & ); //Swap contents in different lists.
		Double_sentinel_list &operator=( Double_sentinel_list );  //Copy Assignment Operators
		Double_sentinel_list &operator=( Double_sentinel_list && );

		void push_front( Type const & ); //Creates new double node storing a value in which the sentinel head's next pointer will point to it. 
		void push_back( Type const & ); //Creates new double node storing a value in which the sentinel tail's previous pointer will point to it.

		void pop_front(); //Deletes the first double node of the list and gets sentinel head's next pointer to point to the next double node after.
		void pop_back(); // Deletes the last double node of the list and gets sentinel tail's previous pointer to point to the previous double node after.

		int erase( Type const & ); //Deletes all double nodes that contain a value equal to the argument. All remaining double node pointers are updated. 

	private:
		Double_node *list_head; //List head. We will initialize this node to be a double node to act as our head sentinel.
		Double_node *list_tail; //List tail. We will initialize this node to be a double node to act as our tail sentinel.
		//Further comments regarding the list head and list tail will be referred to as head and tail sentinel respectively.
		int list_size; //Variable containing the number of items in the doubly linked list.
		
		// List any additional private member functions you author here.
	 	Double_node *new_node; //New Node - Use this to create new double nodes whenever push_front/push_back is called.

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Double_sentinel_list<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                      Public member functions                        //
/////////////////////////////////////////////////////////////////////////


template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list():
// Updated the initialization list here
list_head(new Double_node(0,nullptr,nullptr)), //List head is now a double node (Acts like a head sentinel) which contains null with 2 nullptrs. 
list_tail(new Double_node(0,list_head,nullptr)), //List tail is now a double node (Acts like a tail sentinel) and will contain null and 2 pointers. Update its 'previous' pointer to point to the head sentinel.
list_size(0) //THe list size is 0 since list is empty.
{
	list_head->next_node = list_tail; //Since list tail now contains an address, we can update list head's next pointer to point to the sentinel tail.
}

//Double_Sentinel_List Copy Constructor. Creates an Empty Copy List.
template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list( Double_sentinel_list<Type> const &list ):
// Updated the initialization list here
list_head(new Double_node(0,nullptr,nullptr)), //Create a new empty list with new addresses. 
list_tail(new Double_node(0,list_head,nullptr)),
list_size(0)
{
	list_head->next_node = list_tail;
	//We want to just copy the values in the original list and not the pointer addresses.
	for (Double_node *ptr = list.begin();ptr!=list.end();ptr=ptr->next()){ //Step through the old list. List is the object.
		push_back(ptr->value()); //Push Back each value from each node. Use push back because push front would put the last number in front which is not what we want.
	}
	list_size=list.list_size; //Copy the list size of the original. 
}

//Double_Sentinel_List Move Constructor. Swaps values from the copy constructor and automatically calls the destructor on the copy list.
template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list( Double_sentinel_list<Type> &&list ):
// Updated the initialization list here
list_head(new Double_node(0,nullptr,nullptr)), //Create new sentinel nodes with new addresses.
list_tail(new Double_node(0,list_head,nullptr)), 
list_size(0)
{
	list_head->next_node = list_tail;
	//Get the new sentinel nodes to point to nodes holding a value in the original list.
	//Don't need to go through the list and change anything because other nodes already point to one another.
	list_head->next_node=list.begin(); //Head sentinel will point to the first node
	list_tail->previous_node=list.rbegin(); //Tail sentinel will point tot the last node.
	begin()->previous_node=list_head; //First node will point to the head sentinel.
	rbegin()->next_node=list_tail; //Last node will point to the tail sentinel.
	
	list.list_head->next_node=list.list_tail; //Get the old sentinels to point to each other. 
	list.list_tail->previous_node=list.list_head; //The destructor will be automatically called after which deletes this list.
	list.list_size=0;
}

template <typename Type>
Double_sentinel_list<Type>::~Double_sentinel_list() {
	while (!empty()){
		pop_front(); //Whenever object is deleted, we have to deallocate memory used by deleting the linked list. Step through list and delete all nodes.
	}
	delete list_head; //Delete the sentinels since we allocated memory for them.
	delete list_tail;
}

template <typename Type>
int Double_sentinel_list<Type>::size() const { 
	return list_size; //Return list size variable containing the size of the list.
}
 
template <typename Type>
bool Double_sentinel_list<Type>::empty() const {
	return ((list_head->next()==list_tail) && (list_tail->previous()==list_head)); //If sentinel head and sentinel tail point at each other, the list is empty.  
}

template <typename Type>
Type Double_sentinel_list<Type>::front() const {
	if (empty()){
		throw underflow(); //Throw underflow if there is no value to retrieve since there is no double node.
	}
	return begin()->value(); //Go to first double node defined by begin() and retrieve the value stored.
}

template <typename Type>
Type Double_sentinel_list<Type>::back() const {
	if(empty()){
		throw underflow(); //Throw underflow if there is no value to retrieve since there is no double node.
	}
	return rbegin()->value(); //Go to last double node defined by rbegin() and retrieve the value .
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::begin() const {
	return list_head->next(); //Returns the first double node address defined by sentinel head's next pointer.
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::end() const {
	return list_tail; //Returns sentinel tail's address defined by list tail.
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::rbegin() const {
	return list_tail->previous(); //Returns the last double node address defined by sentinel tail's previous pointer.
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::rend() const {
	return list_head; //Returns sentinel head's address defined by list head.
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::find( Type const &obj ) const {
	for (Double_node *ptr = begin();ptr!=end();ptr=ptr->next()){ //Step through the Linked List.
		if (ptr->value()==obj){ 
			return ptr; //When we find our first value, we return the pointer which currently holds the address where we found our value.
		}
	}
	return end(); //If no value is found, we just return the sentinel tail address.
}

template <typename Type>
int Double_sentinel_list<Type>::count( Type const &obj ) const {
	int counter=0; //Initialize a variable to store the number of matched avlues.
	for (Double_node *ptr = begin();ptr!=end();ptr=ptr->next()){ //Step through the Linked List.
		if (ptr->value()==obj){ 
			++counter; //Whenever the pointer finds a value matching the argument, increase the counter.
		}
	}
	return counter; 
}

template <typename Type>
void Double_sentinel_list<Type>::swap( Double_sentinel_list<Type> &list ) {
	// This is done for you
	std::swap( list_head, list.list_head );
	std::swap( list_tail, list.list_tail );
	std::swap( list_size, list.list_size );
}

template <typename Type>
Double_sentinel_list<Type> &Double_sentinel_list<Type>::operator=( Double_sentinel_list<Type> rhs ) {
	// This is done for you
	swap( rhs );

	return *this;
}

template <typename Type>
Double_sentinel_list<Type> &Double_sentinel_list<Type>::operator=( Double_sentinel_list<Type> &&rhs ) {
	// This is done for you
	swap( rhs );

	return *this;
}

template <typename Type>
void Double_sentinel_list<Type>::push_front( Type const &obj ) {
 			Double_node *ptr = list_head->next_node; //Create a temporary pointer that stores the next double node that sentinel head is pointing to (i.e. Old front node).
 			new_node = new Double_node(obj,rend(),ptr); //Create the new double node. Next pointer points to the contents of temporary pointer. Previous pointer points to sentinel head. 
 			list_head->next_node=new_node; //Update sentinel head's next pointer to point to the new double node.
 			new_node->next_node=ptr; //Update new double node's next pointer to point to the contents of temporary pointer. 
 			ptr->previous_node=new_node; //Update the old front double node's previous pointer to point to the new double node.
 			++list_size; //Increase the list size whenever a new double node is added.
}

template <typename Type>
void Double_sentinel_list<Type>::push_back( Type const &obj ) {
 			Double_node *ptr = list_tail->previous_node; //Create a temporary pointer that stores the previous double node that sentinel tail is pointing to (i.e. Old back node).
 			new_node = new Double_node(obj,ptr,end()); //Create the new double node. Previous pointer points to the contents of temporary pointer. Next pointer points to sentinel tail.
 			list_tail->previous_node=new_node; //Update sentinel tail's previous pointer to point to the new double node.
 			new_node->previous_node=ptr; //Update the new double node's previous pointer to point to the contents of temporary pointer.
 			ptr->next_node=new_node; //Update the old back double node's next pointer to point to the new double node.
 			++list_size; //Increase the list size whenever a new double node is added.
}

template <typename Type>
void Double_sentinel_list<Type>::pop_front() {
	if (empty()){
		throw underflow(); //Throw underflow since there is nothing to delete if list is empty.
	}
	Double_node *ptr = begin(); //Create a temporary pointer that stores the double node address that is going to be deleted (i.e. first double node).
	begin()->next()->previous_node= list_head; //Update the next double node after to point to the sentinel head address.
	list_head->next_node = begin()->next(); //Update the sentinel head's next pointer to point at the next double node after.
	delete ptr; //Delete the double node of interest which temporary pointer contains.
	--list_size; //Reduce the number of items in the list by 1.
}

template <typename Type>
void Double_sentinel_list<Type>::pop_back() {
	if (empty()){
		throw underflow(); //Throw underflow since there is nothing to delete if list is empty.
	}
	Double_node *ptr = rbegin(); //Create a temporary pointer that stores the double node address that is going to be deleted (i.e. last double node).
	rbegin()->previous()->next_node= list_tail; //Update the previous double node after to point to the sentinel tail address.
	list_tail->previous_node = rbegin()->previous(); //Update the sentinel tail's previous pointer to point at the previous double node after.
	delete ptr; //Delete the double node of interest which temporary pointer contains.
	--list_size; //Reduce the number of items in the list by 1.
}

template <typename Type>
int Double_sentinel_list<Type>::erase( Type const &obj ) {
	int del_count=0; //Create a variable which will contain the number of items deleted.
	for (Double_node *ptr=begin();ptr!=end();ptr=ptr->next()){ //Step through Linked List.
		if (ptr->value()==obj){ //Execute whenever we find a match value.
			Double_node *ptr2 = ptr->next()->previous_node; //Create a temporary pointer which will store the address of the double node to be deleted.
			ptr->next()->previous_node = ptr->previous_node; //Update the next double node's previous pointer to point to the previous double node's address.
 			ptr->previous()->next_node = ptr->next_node; //Update the previous double node's next pointer to point to the next double node's address.
 			ptr=ptr->previous(); //Put pointer back to the previous double node because we won't be able to access it when we delete the double node.
			delete ptr2; //Delete the pointer storing the memory address of the double node.
 			++del_count; //Increment the number of nodes deleted by 1.
 			--list_size; //Decrease the number of items in the list by 1.
 		}
	}
	return del_count;
}

template <typename Type>
Double_sentinel_list<Type>::Double_node::Double_node(
	Type const &nv,
	typename Double_sentinel_list<Type>::Double_node *pn,
	typename Double_sentinel_list<Type>::Double_node *nn ):
// Updated the initialization list here
node_value(nv), // Assign the node value to variable, node_value.
previous_node(pn), //Assign previous pointer address to pointer, previous_node. 
next_node(nn) //Assign next pointer address to pointer, next_node.
{
	//Leave Constructor Empty since we used an initialization list.
}

template <typename Type>
Type Double_sentinel_list<Type>::Double_node::value() const {
	return node_value; // Returns the value that double node contains.
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::Double_node::previous() const {
	return previous_node; //Returns the address of the previous node.
}

template <typename Type>
typename Double_sentinel_list<Type>::Double_node *Double_sentinel_list<Type>::Double_node::next() const {
	return next_node; //Returns the address of the next node.
}

/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// If you author any additional private member functions, include them here

/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Double_sentinel_list<T> const &list ) {
	out << "head";

	for ( typename Double_sentinel_list<T>::Double_node *ptr = list.rend(); ptr != nullptr; ptr = ptr->next() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0" << std::endl << "tail";

	for ( typename Double_sentinel_list<T>::Double_node *ptr = list.end(); ptr != nullptr; ptr = ptr->previous() ) {
		if ( ptr == list.rend() || ptr == list.end() ) {
			out << "->S";
		} else {
			out << "->" << ptr->value();
		}
	}

	out << "->0";

	return out;
}

#endif

