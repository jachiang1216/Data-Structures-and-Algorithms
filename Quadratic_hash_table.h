/*****************************************
 * Instructions
 *  - Replace 'uwuserid' with your uWaterloo User ID
 *  - Select the current calendar term and enter the year
 *  - List students with whom you had discussions and who helped you
 *
 * uWaterloo User ID:  jachiang@uwaterloo.ca
 * Submitted for ECE 250
 * Department of Electrical and Computer Engineering
 * University of Waterloo
 * Calender Term of Submission:  (Winter) 2018
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *
 * The following is a list of uWaterloo User IDs of those students
 * I had discussions with in preparing this project:
 *  
 *
 * The following is a list of uWaterloo User IDs of those students
 * who helped me with this project (describe their help; e.g., debugging):
 * 
 ****************************************/

//Implement Gaurd Statements to avoid having the same file included twice. Avoids Duplicate Definitions.
#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#ifndef nullptr 
#define nullptr 0 
#endif

#include <iostream> //Input/Output Library
#include "Exception.h" //Include Exception file to throw exceptions at methods that the compiler cannot handle.
#include "ece250.h" //Testing Dynamic Memory File

enum bin_state_t {UNOCCUPIED, OCCUPIED, ERASED}; //The states of the bin could either be the unoccupied, occupied, or erased.

template <typename Type>
class Quadratic_hash_table {
	private: //Member Variables
		int count; //Holds the number of elements stored in the hash table.
		int power; //Variable that stores the argument m to create a hash table with 2^m bins.
		int array_size; //Variable that stores the size of the array.
		int mask; //Variable that tells us which bits to set to 0.  
		Type *array; //Array Pointer
		bin_state_t *occupied; //Pointer that points and assigns the state.
		
		int num_unoccupied; //Variable storing number of bins unoccupied.

		int hash(Type const &) const; //Hash Function. Mapping down to a given range.

	public:
		//Accessors 
		Quadratic_hash_table(int = 5); //Constructor. Takes an argument m and creates hash table with 2^m bins, indexed from 0 to 2^m - 1. Default value is 5.
		~Quadratic_hash_table(); //Destructor
		int size() const; //Returns the number of elements store in the hash table.
		int capacity() const; //Returns the number of bins in the hash table.
		double load_factor() const; //Returns the load factor of the hash table.
		bool empty() const; //Checks if the hash table is empty or not.
		bool member(Type const &) const; //Checks if the object specified is in the hash table.
		Type bin(int n) const; //Returns the entry in bin n.

		void print() const; //Prints the class in the testing environment.
		
		//Mutators
		void insert(Type const &); //Inserts argument into hash table in the appropriate bin.
		bool erase(Type const &); //Removes argument from hash table. 
		void clear(); //Removes all the elements in the hash table.

	// Friends
	template <typename T>
	friend std::ostream &operator<<( std::ostream &, Quadratic_hash_table<T> const & );
};

//Hash Function
template <typename Type>
int Quadratic_hash_table<Type>::hash(Type const &obj) const{ 
	int hash_value = static_cast<int>(obj) & mask; //Bitwise AND Operation
	if (hash_value < 0){
		return hash_value & (mask+1); //If negative, add size M.
	}else{
		return hash_value; 
	}	
}

//Constructor
template <typename Type>
Quadratic_hash_table<Type>::Quadratic_hash_table(int m):
count(0), //Number of elements is currently 0. 
power(m<0 ? 5 : m), //The power is default m when creating 2^m bins. If user supplies negative number, m=5.
array_size(1 << power), //The object will be turned to an integer bit array. By default, it will be 32-bit which is done by bitwise operations.
mask(array_size - 1), //The mask is the index range in which we want to map the integer into.
array(new Type[array_size]), //The array will hold 32 bits.
occupied(new bin_state_t[array_size]){ //This pointer will point to an array containing the status of each bin.
	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = UNOCCUPIED; //All of the bin statuses are initially set to unoccupied. 
	}
	num_unoccupied = array_size;
}

//Destructor
template <typename Type>
Quadratic_hash_table<Type>::~Quadratic_hash_table(){
	clear(); //Implement the clear function
	delete []array; //Deallocate the array and bin status pointers.
	delete []occupied;
}

template <typename Type>
int Quadratic_hash_table<Type>::size() const{ 
	return count; //Return count variable storing the number of elements in the bin.
}

template <typename Type>
int Quadratic_hash_table<Type>::capacity() const{ 
	return array_size; //The capacity of the hash table is the number of bins which is 2^m.
}

template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const{ 
	//Return the ratio of occupied and erased bins over the total number of bins.
	return (static_cast<double>(array_size)-static_cast<double>(num_unoccupied))/static_cast<double>(array_size); 
}

template <typename Type>
bool Quadratic_hash_table<Type>::empty() const{ 
	return count==0; //If the number of elements in the hash table is 0, then it is empty.
}

template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &obj) const{
	int value = hash(obj); //Use the hash function to hash the value so we know what bin to start looking for the value.
	for (int k=0;k<=pow(2,power);++k){ //Keep checking until we checked each bin at least once which is the hash table size.
		if (occupied[value]==UNOCCUPIED){
			return false; //If the bin is empty, then the value is not there and there is no point searching further.
		}
		else if (occupied[value]==ERASED){
			//Do Nothing. Don't check the value because it is labelled as "ERASED" even if the value is in the bin.
		}
		else{ //For OCCUPIED bins, check the object inside.
			if (obj==array[value]){
				return true; //If the value matches, then return true.
			}
		}
		value = hash(value+k); //Quadratic Probing when searching the bins.
	}	
	return false; //If the object was not found when probing, return false.
}

template <typename Type>
Type Quadratic_hash_table<Type>::bin(int n) const{
    return array[n]; //Return the entry in bin n.
}

//Mutators

template <typename Type>
void Quadratic_hash_table<Type>::insert(Type const &obj){
	if (size()==capacity()){
		throw overflow(); //If the table is full, throw an exception.
	}
	
	if (member(obj)==true){ //Use the member function to check.
		//Do nothing if the object is in the Hash Table already.
	}else{
		int value = hash(obj); //Hash the value.
		int k=0; //Implement a counter.
		while (occupied[value]==OCCUPIED){
			value = hash(value+k); //Quadratic probe through the table until there is an empty bin.
			++k;
		}
		
		array[value] = obj; //Insert the value.
		if (occupied[value]==UNOCCUPIED){
			--num_unoccupied; //We would need to decrement the number of unoccupied bins in order to determine the load factor.
		}
		occupied[value] = OCCUPIED; //Rename the bin as OCCUPIED.
		
		++count; //Increment the size.
	}
}

template <typename Type>
bool Quadratic_hash_table<Type>::erase(Type const &obj){
	int value = hash(obj); //Use the hash function to hash the value so we know what bin to start looking for the value.
	for (int k=0;k<=pow(2,power);++k){ //Keep checking until we checked each bin at least once which is the hash table size.
		if (occupied[value]==UNOCCUPIED){
			return false; //If the bin is empty, then the value is not there and there is no point searching further.
		}
		else if (occupied[value]==ERASED){
			//Do Nothing. Don't check the value because it is labelled as "ERASED" even if the value is in the bin.
		}
		else{ //For OCCUPIED bins, check the object inside.
			if (obj==array[value]){
				occupied[value] = ERASED; //Mark the bin as erased.
				--count; //Decrement the size.
				return true; //Return true since successful deletion.
			}
		}
		value = hash(value+k); //Quadratic Probing when searching the bins.
	}	
	return false; //If the object was not found when probing, return false.
}

template <typename Type>
void Quadratic_hash_table<Type>::clear(){
	count=0; //Number of elements is set back to 0.
	for ( int i = 0; i < array_size; ++i ) {
		occupied[i] = UNOCCUPIED; //All of the bin statuses are set back to unoccupied. 
	}
	num_unoccupied = array_size; //Number of unoccupied bins is set back to the number of elements in the array.
}


//Print Function. NOT TESTED
template <typename Type>
void Quadratic_hash_table<Type>::print() const{
	for (int i = 0; i < capacity(); ++i ) {
		std::cout << array[i] << std::endl; //Print out the Bins.
	}
}




template <typename T>
std::ostream &operator<<( std::ostream &out, Quadratic_hash_table<T> const &hash ) {
	for ( int i = 0; i < hash.capacity(); ++i ) {
		if ( hash.occupied[i] == UNOCCUPIED ) {
			out << "- ";
		} else if ( hash.occupied[i] == ERASED ) {
			out << "x ";
		} else {
			out << hash.array[i] << ' ';
		}
	}

	return out;
}

#endif
