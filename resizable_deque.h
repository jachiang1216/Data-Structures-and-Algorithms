/*****************************************
 * UW User ID:  Jachiang
 * Submitted for ECE 250
 * Semester of Submission: (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided  code.
 *****************************************/
 
//Implement Gaurd Statements to avoid having the same file included twice. Avoids Duplicate Definitions.
#ifndef DYNAMIC_DEQUE_H
#define DYNAMIC_DEQUE_H

#include <iostream> //Input/Output Library
#include "Exception.h" //Include Exception file to throw exceptions at methods that the compiler cannot handle.
using namespace std;
template <typename Type> //Add Template to store/manipulate different variable types for the deque.
class Resizable_deque {
	public:
		Resizable_deque( int = 16 ); //Constructor 
		Resizable_deque( Resizable_deque const & ); //Copy Constructor
		Resizable_deque( Resizable_deque && ); //Move Constructor
		~Resizable_deque(); //Destructor
		
		//Accessors
		
		Type front() const; //Returns the object at the front of the deque.
		Type back() const; //Returns the object at the back of the deque.
		int size() const; //Returns the number of objects in the deque.
		bool empty() const; //Checks whether the deque is empty or not. Returns true if it is empty.
		int capacity() const; //Returns the current array capacity (How many elements it can currently store).
		
		Type value(Type const &);

		//Mutators

		void swap( Resizable_deque & ); //Swaps the contents of deque with contents in the argument.
		Resizable_deque &operator=( Resizable_deque const& ); //Copy Assignment Operators
		Resizable_deque &operator=( Resizable_deque && );
		void push_front( Type const & ); //Inserts a new element at the front of the deque. May increase array capacity if deque is filled. 
		void push_back( Type const & ); //Inserts a new element at the back of the deque.  May increase array capacity if deque is filled.
		void pop_front(); //Removes the first element at the front of the deque. May decrease array capacity if deque is half filled.
		void pop_back(); //Removes the last element at the back of the deque. May decrease array capacity if deque is half filled.
		void clear(); //Empties the deque by resetting member variables. The capacity is set back to the initial one.
		

	private:
		// Your member variables
		int deque_size; //Stores the size of the deque.
		int initial_deque_capacity; //Stores the initial capacity of the array.
		int deque_capacity; //Stores the capacity of the array.
		int ifront; //Index of the front entry.
		int iback; //Index of the most recently pushed object.
		Type *deque_array; //This will store the pointer to the array. Memory will be dynamically called.
	
		// Any private member functions
		//   - helper functions for resizing your array?
		Type resize(); //Resizes the Array if the deque is too small or too big

	// Friends

	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Resizable_deque<T> const & );
};

/////////////////////////////////////////////////////////////////////////
//                   Constructors and Destructors                      //
/////////////////////////////////////////////////////////////////////////

// Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque(int n): //n defines our capacity.
deque_size(0), //The initial size will be 0 since the deque created has no elements yet.
initial_deque_capacity(std::max(16,n)), //The initial capacity that we could store is specified by our argument. If user enters a size less than 16, use 16.
deque_capacity(initial_deque_capacity), //The current capacity is equal to the initial capacity.
ifront(0), //The index is set to 0 since it will be decremented back to deque capacity when we push_front.
iback(deque_capacity-1), //The index is set to the last index since it will be incremented to 0 when we push_back. 
deque_array(new Type[initial_deque_capacity]) //Create a new deque with the desired capacity.
{
	//Constructor is left empty since an initialization list was used.
}

// Copy Constructor
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque const &deque ):
deque_size(deque.deque_size), //All the parameters below are the same as the original deque with the exception of the pointer to the deque.
initial_deque_capacity(deque.initial_deque_capacity),
deque_capacity(deque.deque_capacity),
ifront(deque.ifront),
iback(deque.iback),
deque_array(new Type[deque_capacity]) //Create a new empty deque at a new address.
{
	//Iterate from iback all the way to ifront while copying each value. This is a O(n) process.
	for (int itr = iback;itr!=ifront-1;itr=(deque_capacity-1)*(itr==0)+(itr-1)*(itr!=0)){ 
		deque_array[itr] = deque.deque_array[itr]; //transfer objects into new array.
	}
}

// Move Constructor. Swaps values from the copy constructor and automatically calls the destructor on the copy deque. This is an O(1) process.
template <typename Type>
Resizable_deque<Type>::Resizable_deque( Resizable_deque &&deque ):
deque_size(deque.deque_size), //All the parameters below are the same as the original deque.
initial_deque_capacity(deque.initial_deque_capacity),
deque_capacity(deque.deque_capacity),
ifront(deque.ifront),
iback(deque.iback),
deque_array(deque.deque_array)
{
	//Move Constructor is left empty since an initialization list was used.
}

// Destructor
template <typename Type>
Resizable_deque<Type>::~Resizable_deque() {
	delete []deque_array; //Deallocate the deque which we allocated memory for.
}

/////////////////////////////////////////////////////////////////////////
//                     Public Member Functions                         //
/////////////////////////////////////////////////////////////////////////

template <typename Type>
int Resizable_deque<Type>::size() const { //This is a O(1) process.
	return deque_size; //Returns the current size of the deque which is the variable we made.
}

template <typename Type>
int Resizable_deque<Type>::capacity() const { //This is a O(1) process.
	return deque_capacity; //Returns the current capacity of the deque which is the variable we made.
}

template <typename Type>
bool Resizable_deque<Type>::empty() const { //This is a O(1) process.
	// Chcek if the deque is empty. We will return true if it is empty.
	return (deque_size==0); //Use the size of the deque to determine whether the list is empty or not.
}

template <typename  Type>
Type Resizable_deque<Type>::front() const { //This is a O(1) process.
	if (empty()){
		throw underflow(); //If there are no elements in the deque, throw an exception.
	}
	return deque_array[ifront]; //Returns the object at the first index of the deque array defined by ifront.
}

template <typename  Type>
Type Resizable_deque<Type>::back() const { //This is a O(1) process.
	if (empty()){
		throw underflow(); //If there are no elements in the deque, throw and exception.
	}
	return deque_array[iback]; //Returns the object at the back of the deque array defined by iback. 
}

template <typename Type>
void Resizable_deque<Type>::swap( Resizable_deque<Type> &deque ) { //This is a O(1) process.
	// Swap the member variables which is defined in our private list.
	   std::swap( deque_array, deque.deque_array );
	   std::swap( deque_size, deque.deque_size );
	   std::swap( deque_capacity, deque.deque_capacity );
	   std::swap( ifront, deque.ifront );
	   std::swap( iback, deque.iback );
	   std::swap( initial_deque_capacity, deque.initial_deque_capacity );
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> const &rhs ) {
	// This is done for you...
	Resizable_deque<Type> copy( rhs );
	swap( copy );

	return *this;
}

template <typename Type>
Resizable_deque<Type> &Resizable_deque<Type>::operator=( Resizable_deque<Type> &&rhs ) {
	// This is done for you...
	swap( rhs );

	return *this;
}

template <typename Type>
void Resizable_deque<Type>::push_front( Type const &obj ) { //This is on average a O(1) process. If we have to resize, the process becomes O(n).
	if (deque_size==deque_capacity){
		resize(); //If the deque is full, we will need to resize our array. Use Resize Function.
	}
	ifront=(deque_capacity-1)*(ifront==0)+(ifront-1)*(ifront!=0); //Move the front index down by 1. If front index is at 0, wrap it back to the end of the deque.
	deque_array[ifront] = obj; //The object will be inserted into the new index defined by ifront.
	deque_size++; //Increase the number of elements by 1.
}

template <typename Type>
void Resizable_deque<Type>::push_back( Type const &obj ) { //This is on average a O(1) process. If we have to resize, the process becomes O(n).
	if (deque_size==deque_capacity){
		resize(); //If the deque is full, we will need to resize our array. Use Resize Function.
	}
	iback=(0)*(iback==(deque_capacity-1))+(iback+1)*(iback!=(deque_capacity-1)); //Move the back index up by 1. If back index reaches the end of the deque, wrap it back to the beginning of the array.
	deque_array[iback] = obj; //The object will be inserted into the new index defined by iback.
	deque_size++; //Increase the number of elements by 1.
}

template <typename Type>
void Resizable_deque<Type>::pop_front() { //This is on average a O(1) process. If we have to resize, the process becomes O(n).
	if (empty()){
		throw underflow(); //If there are no elements in the deque, throw an exception since there is nothing to pop.
	}
	deque_array[ifront]=0; //For printing purposes, we will reassign the current value to null/0. 
	ifront=(0)*(ifront==(deque_capacity-1))+(ifront+1)*(ifront!=(deque_capacity-1)); //To pop front, simply move the front index up by 1. This new index is now the front.
	deque_size--; //Decrease the number of elements by 1.
	if (deque_size==deque_capacity/4 && deque_capacity>16){  
		resize(); //If the deque is 1/4 the current capacity, we will have to resize our array. However, if the deque capacity is 16, do nothing since this is the smallest we can have.
	}
}

template <typename Type>
void Resizable_deque<Type>::pop_back() { //This is on average a O(1) process. If we have to resize, the process becomes O(n).
	if (empty()){
		throw underflow(); //If there are no elements in the deque, throw an exception since there is nothing to pop.
	}
	deque_array[iback]=0; //For printing purposes, we will reassign the current value to null/0. 
	iback=(deque_capacity-1)*(iback==0)+(iback-1)*(iback!=0); //To pop back, simply move the back index down by 1. This new index is now the back.
	deque_size--; //Decrease the number of elements by 1.
	if (deque_size==deque_capacity/4 && deque_capacity>16){
		resize(); //If the deque is 1/4 the current capacity, we will have to resize our array. However, if the deque capacity is 16, do nothing since this is the smallest we can have.
	}
}

template <typename Type>
void Resizable_deque<Type>::clear() { //This is a O(1) process.
	//We will empty this deque by resetting all the member variables. Everything is set like it would be in the constructor.
	ifront=0; 
	iback=deque_capacity-1;
	deque_size=0;
	delete []deque_array; //Delete the old array since it may have a different capacity.
	deque_array = new Type[initial_deque_capacity];
	deque_capacity = initial_deque_capacity;
}

/////////////////////////////////////////////////////////////////////////
//                      Private member functions                       //
/////////////////////////////////////////////////////////////////////////

// Enter any private member functions (helper functions) here
template <typename Type>
Type Resizable_deque<Type>::value(Type const &obj) {
	if (empty()){
		throw underflow(); //If there are no elements in the deque, throw an exception.
	}
	return deque_array[obj]; 
}

template <typename Type>
Type Resizable_deque<Type>::resize(){ //This is an O(n) process due to copying elements onto the new deque array.
	int old_capacity = deque_capacity; //Store the old capacity in a variable. We will need this when reading through the old deque array.
	if (deque_size==deque_capacity){ //Create an if statement to determine if we would increase or decrease our deque capacity.
		deque_capacity *= 2; //Increase to 2 times the Capacity. 
	}
	else{
		deque_capacity /= 2; //Decrease to half the Capacity.
	}
	int tempback = iback; //Store the iback index since we would have to change this when going through the old deque.
	int itr = iback; //Initialize an iterator to the iback index. This would be our iterator when going through the new deque.
	Type *delete_array = deque_array; //Create a pointer that points to the old deque array since later we will need to deallocate the deque.
	Type *temp_array = new Type[deque_capacity]; //Create a new array with double the capacity of the old array.
	for (int i=0;i!=deque_size;i++){ //Use a for loop to copy each object.
		temp_array[itr] = back(); //Copy the object into new array.
		iback=(old_capacity-1)*(iback==0)+(iback-1)*(iback!=0); //Change the index to access the next object of the old deque.
		itr=(deque_capacity-1)*(itr==0)+(itr-1)*(itr!=0); //Change the index to access the next index of the new deque.
	}
	int tempfront = (itr+1)*(itr!=(deque_capacity-1))+(0)*(itr==(deque_capacity-1)); //Tempfront is going to be our ifront index. 
	deque_array = temp_array; //The pointer deque_array will point to the newly created deque.
	ifront = tempfront; //ifront reinitialized to the front index position of the new deque.
	iback = tempback; //iback reinitialized to the back index position of the new deque.
	delete []delete_array; //Deallocate the deque pointed to by the delete_array pointer.
}

/////////////////////////////////////////////////////////////////////////
//                               Friends                               //
/////////////////////////////////////////////////////////////////////////

// You can modify this function however you want:  it will not be tested

template <typename T>
std::ostream &operator<<( std::ostream &out, Resizable_deque<T> const &list ) {

//Use a For Loop to print out all the indexes in the array. This way, we can see what indexes are deemed null with no element.
	for (int i=0;i<list.deque_capacity;i++){
		out << list.deque_array[i] << endl;
	}
	
return out;

}

#endif
