TITLE : GENERIC BTREE IMPLEMENTATION ON HARD DISK.
COURSE : GENERIC PROGRAMMING ( 10CS368 )



INTRODUCTION :
	B-Tree is an excellent data structure for storing huge amounts of data for
fast retrieval. As accessing any part of the tree for reading or writing requires
visiting only a few nodes unlike traditional binary trees. The idea is to create a generic
homogeneous B-Tree data structure with the keys in RAM and Records in hard disk.

OBJECTIVE :
	=>To reduce the burden on RAM for Btrees with huge records.

INTERFACE SUPPORTED:
	=>Creation of The Btree
		Pass 2 sequences one for the keys and one for records.

	=>Insertion into a btree.
		Inserting the keys into a Btree in ascending order based on less<key_type> specified.

	=>Bidirectional iterator.
		=>The Iterator moves through the Btree based on the logical inorder traversal for the forward traversal.

	=>Searching for a particular key.
		Returns a pair of key and record objects.

	=>Deletion of a tree.
		In the destructor of the tree.

IMPLEMENTATION ISSUES AND PHILOSOPHIES:
	=>Base class __Btree< object type , order> handles the contruction of Btree on the RAM and provides other functionalities
		required for Btree manipulation as mentioned above
	=>The derived class Btree< key , record , order> wraps the key in an object called KeyObj and uses the
		the base class for all RAM manipulations.
		The additional functionality is that of storing the record in a file.
		There is offset stored in the KeyObj that corresponds to the record of a particular key.

	=>L value for the key not supported as the logical structure of the tree could go for a toss.

Possible expansions to the project
	The other 2 possibilty of
		key in RAM record in RAM.
		Key in hard disk and record in Hard disk.
	can be supported by deriving the base class in 2 diff derived classes.
	=>L val for records could be supported for record modification.

ANOMOLIES :
	=>Duplicate keys are not supported.
	=>Deletion of keys not supported.	
	=>Btree<1> specialization( binary tree ) to be done as the general version does not currectly support binary tree.

MEMBERS:
Akshay Mallya		1PI10CS010
Abhishek Patil		1PI10CS004
Abdus Salam Khazi	1PI10CS001
