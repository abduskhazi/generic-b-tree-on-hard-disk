#ifndef BTREE_H
#define BTREE_H


//BTREE IMPLEMENTAION OF ORDER OF BTREE N>1
//	N IS CONSIDERED AS THE NUMBER OF KEYS IN THE NODE

//#define MAX 15	//For simplicity assumption is that order of B TREE is 5
		//MAX IS THE NUMBER OF KEYS

#include<iostream>
#include<cmath>

#include<utility>
#include<stack>

#include<fstream>
#include<vector>
#include<typeinfo>
using namespace std;



#if 0
typedef pair< T , pair<__Node<T>*,__Node<T>*> > node_split;
#endif

#if 0
	//The following philosophy of __Btree is deprecated.
//The deletion of tree must be explicit
//	as it may be stored on the hard disk.
//	Philosophy : do not touch the hard disk except when the client wants you to change it.
#endif
template <typename T, int MAX>
class __Btree
{
	private:
		struct __Node;
		typedef pair< T, pair<__Node*,__Node*> > node_split;
		struct __Node //canonical?? yes
		{
			//data
				int count;
				T keys[MAX];
				__Node*  branches[MAX + 1];
			
			//methods	
				#if 0 //depricated
				__Node<T>( int count , T keys[] , __Node<T>* branches[]); //Supressing the parameterized contr
				#endif
			
				//The contr was made public only for testing.
				template<typename ptr_T>
				__Node( ptr_T begin_keys , ptr_T end_keys , __Node** begin_branches , __Node** end_branches);
				__Node();

				bool insert( T key ,__Node* left=NULL , __Node* right=NULL);
				bool search_node(T target , int* pos) const;

				T* search( T key ); //?????
				pair<T , pair<__Node*,__Node*> > split(T key , __Node* left_child = NULL, __Node* right_child = NULL);

				
				int get_count() const{ return count;}
				__Node* get_branch(int i) const { return branches[i];}
				T* get_object_ptr( int elem_pos ) { return keys + elem_pos;}
		};

		typedef __Btree<T,MAX>::__Node __Node_t;
	
	public : class __Iterator;
	private:
		static bool pushdown( __Node** root_ptr , T key , node_split& p);
		static void display_subtree(__Node* root);
		static void delete_subtree(__Node** root_ptr);
		static void copy_subTree(__Node** lhs_ptr,__Node* rhs );

		__Node* getRoot() const{ return root;}

		static __Iterator& begin_wr(__Iterator &it, __Node* root);
		static __Iterator& last_wr(__Btree::__Iterator& it , __Node* root);
		static __Iterator search_tree(T target,__Iterator& it, int* pos , __Node*);


		//displaying a node.
		template<typename T_,int MAX_>
		friend ostream& operator<<( ostream& o , const typename __Btree<T_,MAX_>::__Node& A);

		//As node is used in the implementation.
		template<typename Y , int MAX_Y>
		friend ostream& operator<<( ostream& o , const typename __Btree<Y,MAX_Y>::__Iterator& it);	


		//data		
		
		__Node* root;

	public:

		//The default contr
		__Btree(){ root = NULL; }
		template<typename ptr_T>
		__Btree( ptr_T begin , ptr_T end);
		~__Btree(); //Should the destructor delete the full tree?
			  //	yes it is deleting the full tree in this case.

		template<typename ptr_T>
		void insert_sequence( ptr_T begin , ptr_T end);

		bool insert(T key);
		__Iterator search(T key);
		pair<__Iterator,__Iterator> find_range( T key );

		//__Iterator functions
		//Begin and end.
		__Iterator begin();
		__Iterator end();
		__Iterator last();

		__Btree( const __Btree& );
		__Btree& operator=(const __Btree& );

		void display() const;

		//BIDIRECTIONAL ITERATOR
		class __Iterator
		{
			friend class __Btree;
			typedef pair<__Node*,int> stack_element;
			typedef stack<stack_element> stack_t;
			public:
				//canonical?? yes
				__Iterator(T* ptr = NULL, stack_t stk = stack_t());//is it T*??
		//		!= ==
		//		* dereference for rvalue
		//		* deference for l-value
		//		++ pre and post
		//		-- pre and post
		//		hold the value 
				bool operator==(const __Iterator&)const;
				bool operator!=(const __Iterator&)const;
				__Iterator& operator++();
				__Iterator operator++( int );
				__Iterator& operator--();
				__Iterator operator--(int);
				T operator*() const; //for r value
		//DEREFERENCE FOR LVAL WILL BREAK THE STRUCTURE OF THE BTREE

				void display() const;

			//For testing.
			template<typename Y , int MAX_Y>
			friend ostream& operator<<( ostream& o , const typename __Btree<Y,MAX_Y>::__Iterator& it);	
				
			private:
				//The implementations functions
				__Node* get_next_branch();
				void increment_index_top();
				T* get_object_pointer();
				__Node* get_previous_branch();

					//data
				T* ptr;
				stack_t stk;
					//The stack contains all of the branches that have been completed.
					//	or is in transition from the incrementation perspective.
		};
};

//IMPLEMENTATION---------------------------------------------------------------------------------------------------

//------------------------------------------------__Node-------------------------------------------------------
template<typename T,int MAX>
__Btree<T,MAX>::__Node::__Node()
{
	count = 0;
		//The program does everything with respect to count.


	for(int i = 0 ; i<MAX ; ++i)
	{
		keys[i] = T(); 	//The default value for the keys
		branches[i] = NULL;	//The corresponding left node of each key
	}
	branches[MAX] = NULL;

}

template<typename T,int MAX>
template<typename ptr_T>
__Btree<T,MAX>::__Node::__Node(ptr_T begin_keys , ptr_T end_keys , __Node** begin_branches , __Node** end_branches)
{
	//you get what you deserve..
	//	do not insert unstable data eg : #keys = #branches-2.
	this->count = 0;
	ptr_T ptr_keys = this->keys;
	while( begin_keys != end_keys )
	{
		*ptr_keys++ = *begin_keys++;
		++this->count;
	}

	//will count value exeed :no
	__Node** temp_branches = this->branches;
	while( begin_branches != end_branches)
	{
		*temp_branches++ = *begin_branches++;
	}

	//Init all unwanted values to 0/null
	for( int i = count ; i <MAX ; ++i)
		keys[i] = T();
	for(int i = count+1 ; i<=MAX ; ++i)
		branches[i] = NULL;
	if( this->count == 0) branches[0] = NULL;
}

template<typename T_ , int MAX_>
ostream& operator<<(ostream& o , const typename __Btree<T_,MAX_>::__Node& A)
{
	//displaying the node
	o << "[ ";
	for(int i = 0 ; i<MAX_ ; ++i)
	{
		o << A.keys[i] << " ";
	}
	o << "]:( ";
	for(int i = 0 ; i<=MAX_ ; ++i)
	{
		o << A.branches[i] << " ";
	}
	o << ")" << endl;

	return o;	
}

template<typename T,int MAX>
T* __Btree<T,MAX>::__Node::search(T key)
{
        for(int i=0; i<count; ++i)
        {
                if(keys[i]==key)
                        return (keys+i);
        }
	return NULL;
}

template<typename T,int MAX>
bool __Btree<T,MAX>::__Node::insert(T key, __Node* left , __Node* right)
{
	int i,j;
	if(count != MAX)
	{
			for(i=0; i< count && keys[i] <= key; ++i);
			if(i == count)
			{
				keys[i]= key;
				branches[i] = left;
				branches[i+1] = right;
			}
			else
			{
				for (j=count-1; j >=i; --j)
				{
					keys[j+1] = keys [j];
					branches[j+2] = branches[j+1];
				}
				branches[i+1] = right;
				branches[i] = left;
				keys[i] = key;
			}
			count ++;	//increment the number of elem.
			return true;
		
	}
	return false;
}



template<typename T,int MAX>
pair<T , pair< typename __Btree<T,MAX>::__Node* ,typename  __Btree<T,MAX>::__Node*> >
	__Btree<T,MAX>::__Node::split(T key , __Node* left_child , __Node* right_child)
{
	//We have a full node,
	//	a key to insert
	//		Do we check full node?? : no
	//		The prgrammer gets what he deserves

		//obtain the median
		// create 2 nodes
		//	return median and pair<nodes>
		//MEDIAN POS MAY BE : ODD OR EVEN

	int median_pos = floor( count/2 );
	if(key>=keys[median_pos])
	{
		//median= keys[median_pos]
		__Node* left = new __Node(keys, keys+median_pos, branches, branches+median_pos+1 );
		__Node* right= new __Node(keys+median_pos+1, keys+count,branches+median_pos+1, branches+count+1);
		right->insert(key , left_child , right_child);
		return pair<T, pair<__Node*,__Node*> >( keys[median_pos], pair<__Node*,__Node*>(left,right) );
	}
	else if(key>keys[median_pos-1])
	{
		//median= key itself
		//The shared link is got split as : left_child and right child.
		__Node* left = new __Node(keys, keys+median_pos, branches , branches+median_pos);
		__Node* right= new __Node(keys+median_pos, keys+count, branches+median_pos, branches+count+1 );
		left->branches[median_pos]=left_child;
		right->branches[0] = right_child;
		return pair<T,pair<__Node*,__Node*> >(key, pair<__Node*,__Node*>(left, right));
	}
	else
	{
		//median = keys[median_pos-1]
		__Node* left = new __Node(keys, keys+median_pos-1,branches, branches+median_pos);
		left->insert(key,left_child,right_child);
		__Node* right = new __Node(keys+median_pos,keys+count, branches+median_pos, branches+count+1);
		return pair<T , pair<__Node*,__Node*> >( keys[median_pos-1] , pair<__Node*,__Node*>(left,right));
	}
}

template<typename T,int MAX>
bool __Btree<T,MAX>::__Node::search_node(T target , int* pos) const
{
	if(target < keys[0])   // choose the leftmost branch
	{
		*pos =0;
		return false;
	}
	else
	{
		for(*pos=count; target < keys[(*pos)-1] && (*pos)>1; (*pos)--);
		
		return target==keys[(*pos)-1];
	}
}


//--------------------------------------------------End-__Node<T>--------------------------------------------------------

//--------------------------------------------------__Btree-----------------------------------------------------------
template<typename T,int MAX>
__Btree<T,MAX>::~__Btree()
{
	cout << "destructor called" << endl;


	__Btree<T,MAX>::delete_subtree( &root );
}

template<typename T,int MAX>
void __Btree<T,MAX>::delete_subtree(__Node** root_ptr)
{
	if( *root_ptr != NULL)
	{
		__Node* root = *root_ptr;
		for(int i = 0 ; i <= root->get_count() ; ++i)
		{
			__Btree<T,MAX>::delete_subtree( &root->branches[i] );
		}
		delete root;
		*root_ptr = NULL;
	}
}

template<typename T,int MAX>
__Btree<T,MAX>::__Btree( const __Btree<T,MAX>& rhs) : root(NULL)
{
	if(rhs.root!=NULL)
	{
		copy_subTree(&root, rhs.getRoot());
	}
}

template<typename T,int MAX>
__Btree<T,MAX>& __Btree<T,MAX>:: operator = (const __Btree<T,MAX> &rhs)
{
	if(this != &rhs)	//avoid self assignment
	{
		delete_subtree(&this->root);
		copy_subTree(&root, rhs.getRoot());
	}
	return *this;
}

template<typename T,int MAX>
void __Btree<T,MAX>::copy_subTree(__Node** lhs_ptr,__Node* rhs )
{
	if(rhs!=NULL)
	{
		*(lhs_ptr) = new __Node(rhs->keys, rhs->keys+rhs->get_count(), rhs->branches, rhs->branches+ rhs->get_count() + 1);
		for(int i=0;i<rhs->get_count();++i)
		{	
			__Btree<T,MAX>::copy_subTree(&((*lhs_ptr)->branches[i]),(rhs->branches[i]) );			
		}	
		__Btree<T,MAX>::copy_subTree(&((*lhs_ptr)->branches[(*lhs_ptr)->get_count()]),(rhs->branches[rhs->get_count()]));
	}
}


/*
//algorithm pushdown( key ,__Btree):
	input : key , __Btree
	output : true if 
		 false if it could not insert in the given btree without splitting
		The split values are valid only if false is returned.

	if __Btree is NULL
		return false and the split values containing key , NULL , NULL and 
	else if the key is not repeating in the given __Btree root node
		get the 'position' of the child __Btree to insert.
		[med,left,right] = pushdown(key , __Btree in 'position')
		if pushdown was not successful,
			insert med,left,right in this node.
			if the node is full and another insertion not possible,
				split this node
				return false , new_med , new_left , new_right.
		return true
	else
	    give warning that the key is not repeating.
*/
template<typename T,int MAX>
bool __Btree<T,MAX>::pushdown(__Node** root_ptr, T key , node_split& p)
{
	__Node* root = *root_ptr;
	if( root == NULL)
	{
		p.first = key;
		p.second.first = NULL;
		p.second.first = NULL;
#if 0
		cout << p.first << p.second.first << p.second.second << endl;
#endif
		return false;
	}
	else
	{
		int pos;
		if( ! root->search_node( key, &pos) ) //If the nodes are not repeating.
		{
			node_split pair_below;
			if(! __Btree<T,MAX>::pushdown( &root->branches[pos], key , pair_below))
			{
				//could not be inserted below.
				if(!root->insert( pair_below.first , pair_below.second.first , pair_below.second.second ) )
				{
					//could not be inserted in the current node.
					p = root->split(pair_below.first , pair_below.second.first , pair_below.second.second );

					delete root;
					*root_ptr = NULL;

					return false;
				}
			}
		}
		else
		{
			cout << key << "is repeating.." << endl;
		}
		return true;
	}
}

template<typename T,int MAX>
bool __Btree<T,MAX>::insert(T key)
{
	node_split var_split;
	if(!__Btree<T,MAX>::pushdown( &root , key ,  var_split))
	{
		delete this->root;
		root = new __Node();
		if(root == NULL) return false;
		root->insert(var_split.first, var_split.second.first ,var_split.second.second);
	}
	return true;
}

template<typename T,int MAX>
template<typename ptr_T>
__Btree<T,MAX>::__Btree( ptr_T begin , ptr_T end) : root(NULL)
{
	//Insert the keys one by one
	while( begin != end)
	{
		if( root == NULL )
			root = new __Node();
		if(!insert( *begin ))
		{
			cout << "Cannot insert because of lack of space"<<endl;
		}
		++begin;
	}
}

template<typename T,int MAX>
template<typename ptr_T>
void __Btree<T,MAX>::insert_sequence( ptr_T begin , ptr_T end)
{
	root = NULL;
	//Insert the keys one by one
	while( begin != end)
	{
		if( root == NULL )
			root = new __Node();
		if(!insert( *begin ))
		{
			cout << "Cannot insert because of lack of space"<<endl;
		}
		++begin;
	}	
}

template<typename T,int MAX>
void __Btree<T,MAX>::display() const
{
	__Btree<T,MAX>::display_subtree( root );
}

template<typename T,int MAX>
void __Btree<T,MAX>::display_subtree(__Node* root)
{
	//displaying the __Btree in the node by node fashion
	if(root!=NULL)
	{
		typename __Btree<T,MAX>::__Node* node_ptr = root;
#if 0
		cout << node_ptr << " : " << *node_ptr;
#endif
		cout << node_ptr << " : " ;
		operator<< <T,MAX>( cout , *node_ptr);
#if 0
		cout << typeid(*node_ptr).name();
#endif
		cout << "\n";
		for(int i =0 ; i < node_ptr->get_count() ; ++i)
		{
			display_subtree( node_ptr->get_branch(i) );
		}
		if(node_ptr->get_count()!=0)
                display_subtree(node_ptr->get_branch(node_ptr->get_count()));
	}
}


template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator __Btree<T,MAX>::search(T target)
{
	int pos =-1;
	__Btree<T,MAX>::__Iterator it;
	return __Btree<T,MAX>::search_tree(target, it, &pos , root);
}
template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator __Btree<T,MAX>::search_tree(T target,__Btree<T,MAX>::__Iterator& it, int* pos , __Node* root)
{
	if(root==NULL)		
		return it;
	else if(root->search_node(target,pos))
	{
		it.ptr = &(root->keys[(*pos)-1]);
		it.stk.push(pair<__Node*,int>(root,(*pos)-1));
		//cout<<it;
		return it;	
	}	
	else
	{
		it.ptr =&(root->keys[(*pos)-1]);
		it.stk.push(pair<__Node*,int>(root,(*pos)));
		//cout<<it;
		return __Btree<T,MAX>::search_tree(target,it,pos,root->branches[*pos]);
	}
	
	
}


template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator __Btree<T,MAX>::end()
{
	return __Iterator();
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator __Btree<T,MAX>::begin()
{
	__Iterator it;
	return begin_wr(it,root);	
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator& __Btree<T,MAX>::begin_wr(__Iterator &it, __Node* root)
{	
		it.stk.push(pair<__Node*,int>(root,0));
		if(root==NULL)
		{
			it.stk.pop();	
			return it;
		}
		else
		{	
			it.ptr = &(root->keys[0]);
			return begin_wr(it , root->branches[0]);
		}
	
	//undefined return .. cannot come here...
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator __Btree<T,MAX>::last()
{
	__Iterator it;
	return last_wr( it , root);	
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator& __Btree<T,MAX>::last_wr(__Btree<T,MAX>::__Iterator& it , __Node* root)
{
	//count cannot be 0 ..... implemented contr in that way.
	typedef pair<__Node*,int> stack_elem;
	if(root != NULL)
	{
		int count = root->get_count();
		it.stk.push( stack_elem(root,count));
		it.ptr = root->keys+count-1;
		return last_wr(it , root->get_branch(count));
	}
	else
	{
		it.stk.top().second--;
		return it;
	}
	//undefined return.. : cannot come here..
}
//--------------------------------------------------End-__Btree-------------------------------------------------------

//-----------------------------------------------__Btree::__Iterator----------------------------------------------------

template<typename T,int MAX>
__Btree<T,MAX>::__Iterator::__Iterator(T* ptr, stack_t stk )
 : ptr(ptr) , stk(stk)
{
}

template<typename T,int MAX>
bool __Btree<T,MAX>::__Iterator::operator==(const __Iterator& rhs)const
{
	return (ptr == rhs.ptr) && (stk == rhs.stk);
}

template<typename T,int MAX>
bool __Btree<T,MAX>::__Iterator::operator!=(const __Iterator& rhs)const
{
	return !( *this == rhs );
}

template<typename T,int MAX>
T __Btree<T,MAX>::__Iterator::operator*() const
{
	return *ptr;
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Node* __Btree<T,MAX>::__Iterator::get_next_branch()
{
	//__Iterator's member function
	pair<__Node*,int> elem_stack = stk.top();
	return elem_stack.first->get_branch(elem_stack.second);
}

template<typename T,int MAX>
void __Btree<T,MAX>::__Iterator::increment_index_top()
{
	pair<__Node*,int> elem_stack =  stk.top();
	stk.pop();
	elem_stack.second++;
	stk.push( elem_stack );
}

template<typename T,int MAX>
T* __Btree<T,MAX>::__Iterator::get_object_pointer()
{
	__Node* top_ptr = stk.top().first;
	int elem_pos = stk.top().second;
	return top_ptr->get_object_ptr( elem_pos );
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator __Btree<T,MAX>::__Iterator::operator++( int )
{
	__Iterator temp = *this;
	++(*this);
	return temp;
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator& __Btree<T,MAX>::__Iterator::operator++()
{
	//The pre increment.
	T* temp_ptr = ++this->ptr;

	//increment the index in the top of stack.
	stk.top().second++;

	//get the next branch for traversal
	__Node* next = get_next_branch();

	//pushing the branch obtained , 0
	stk.push( pair<__Node*,int>(next,0));

	__Node* top_ptr = stk.top().first;
	while( top_ptr != NULL) //top branch is not NULL
	{
		//first child of the top branch is pushed.
		stk.push( pair<__Node*,int>( top_ptr->get_branch(0), 0) );
		top_ptr = stk.top().first;
	}

	//Now the top of stack is NULL => pop the stack.
	stk.pop();

	//pointer = top's element
	this->ptr = get_object_pointer();

//Algo : (second part)
//	if The pointer is the same object pointer
//		while top's index is equal to count
//			pop
//		if stack is not NULL
//			pointer = top's element (i.e the index is taken from the stack).
//			return __Iterator
//		else state end.(one past the last.)
//	return __Iterator


	if( temp_ptr == ptr)
	{
		while( !stk.empty() && stk.top().second == stk.top().first->get_count())
		{
			stk.pop();
		}
		if(!stk.empty())
		{
			this->ptr = get_object_pointer();
			return *this;
		}
		else
		{
			stk = stack_t();
			ptr = NULL;
		}
	}
	return *this;
	
	
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Node* __Btree<T,MAX>::__Iterator::get_previous_branch()
{
	stack_element elem_stack = stk.top();
	return elem_stack.first->get_branch(elem_stack.second);
}

template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator& __Btree<T,MAX>::__Iterator::operator--()
{
	T* temp_ptr = --ptr;
	//get previous branch to traverse.
	__Node* prev = get_previous_branch();

	if( prev )
		stk.push( stack_element(prev,prev->get_count()) );
	else
		stk.push( stack_element(prev,MAX) );

	__Node* top_ptr = stk.top().first;

	while(  top_ptr != NULL )
	{
		__Node* last_branch = top_ptr->get_branch( top_ptr->get_count() );
		if( last_branch )
		{
			stk.push( stack_element( last_branch , last_branch->get_count() ) );
		}
		else
		{
			stk.push( stack_element(last_branch,MAX) );
		}
		top_ptr = stk.top().first;
	}

	stk.pop();
	stk.top().second--;
	ptr = get_object_pointer();

	if( temp_ptr == ptr && stk.top().second<0)
	{
		while( !stk.empty() && stk.top().second<=0)
			stk.pop();
		if( !stk.empty() )
		{
			stk.top().second--;
			ptr = get_object_pointer();
		}
		else
		{
			ptr = NULL;
			stk = stack_t();
		}
	}
	return *this;
}


template<typename T,int MAX>
typename __Btree<T,MAX>::__Iterator __Btree<T,MAX>::__Iterator::operator--(int)
{
	__Iterator temp = *this;
	--(*this);
	return temp;
}


template<typename T , int MAX>
void __Btree<T,MAX>::__Iterator::display() const
{
	operator<< <T,MAX>( cout , *this);
}

template<typename Y , int MAX_Y>
ostream& operator<<( ostream& o , const typename __Btree<Y,MAX_Y>::__Iterator& it)
{
	typedef stack< pair< typename __Btree<Y,MAX_Y>::__Node*,int> > stack_t;
	stack_t stk_temp(it.stk);
	//copying the stack..

	//Print the key
	if(it.ptr)
		o << *(it.ptr) << " : ";
	else
		o << it.ptr << " : ";
	//print the stack
	while( !stk_temp.empty() )
	{
		pair< typename __Btree<Y,MAX_Y>::__Node*,int> element_of_stack = stk_temp.top();
		o << "(" << element_of_stack.first << "," << element_of_stack.second << ")";
		o  << "  ";
		stk_temp.pop();
	}
	return o;
}

//--------------------------------------------End-__Btree::__Iterator-------------------------------------------------




//The class to support encapsulation of key and offset.
template<typename PK_T , typename RT>
class KeyObj
{
	private:
		PK_T key;
		unsigned long offset;
		string filename;
	public:
		explicit KeyObj( PK_T key = PK_T() )
			: key(key)
		{
			offset = (unsigned long)-1; // A very large offset a dummy keyObj
		}


		KeyObj( const PK_T& key , const RT& record , fstream& file, string name)
			: key(key)
		{
			filename=name;
			//create the record
			file.seekp(0 , ios::end);
			offset = file.tellp();
			file.write( reinterpret_cast<const char*>(&record) , sizeof(record));
		}

		PK_T get_key()
		{
			return key;
		}

		RT get_record()
		{
			RT new_obj;
			fstream file;
			file.open(filename.c_str(),fstream::in | fstream::out);

			file.seekg(offset);
			file.read( reinterpret_cast<char*>(&new_obj), sizeof(new_obj));

			file.close();
			return new_obj;
		}
		
#if 0
//	deprecated
			file << record << endl;
		This makes the client dependent on out implementation
#endif

		//Destructor does not remove the file record as the file is to be deleted after the process gets killed.

		//  Supporting all operators the key supports
		//	Provides flexibilty for the key object to support just the lesser than
        //  operator and other operators can be derived.

		inline bool operator < (const KeyObj& rhs) const
        {
            return key < rhs.key;
        }

		inline bool operator > (const KeyObj& rhs) const
		{
			return rhs.key < key;
		}

		inline bool operator <= (const KeyObj& rhs) const
		{
			return !(rhs.key < key);
		}

		inline bool operator >= (const KeyObj& rhs) const
		{
			return !(key < rhs.key);
		}

		inline bool operator == (const KeyObj& rhs) const
		{
			return !(key < rhs.key) && !(rhs.key < key);
		}

		inline bool operator != (const KeyObj& rhs) const
        {
            return (key < rhs.key) || (rhs.key < key);
        }

		template<typename PK_T_, typename RT_>
		friend ostream& operator<<( ostream& o , const KeyObj<PK_T_ , RT_>& rhs);
};

template<typename PK_T , typename RT>
ostream& operator<<(ostream& o , const KeyObj<PK_T,RT>& rhs)
{
	cout << "(" << rhs.key << "," << rhs.offset << ")";
    
    return o;
}






//The class that actually interfaces with the client
template<typename PK_T , typename RT , int MAX>
class Btree : private __Btree< KeyObj<PK_T,RT> ,MAX>
{
	typedef __Btree< KeyObj<PK_T,RT> ,MAX> _Base;
	typedef Btree<PK_T,RT,MAX> Btree_t;

	private : 
		fstream file_t;

	public:
		class Iterator:public _Base::__Iterator
		{
			typedef typename _Base::__Iterator _Base_Iterator;

			public:
				Iterator( const _Base_Iterator& rhs) : _Base_Iterator( rhs )
				{
					
				}
				pair<PK_T,RT> operator*() const
				{

					//KeyObj<PK_T,RT> temp_obj = *( (_Base_iterator)(*this) );

					KeyObj<PK_T,RT> temp_obj = (* (typename _Base::__Iterator)(*this) );
					
					return pair<PK_T,RT>(temp_obj.get_key(), temp_obj.get_record());
				}
		};
		//constructors
		template<typename keyIterator,typename valueIterator>
		Btree<PK_T,RT,MAX>(keyIterator begin , keyIterator end ,
                           valueIterator begin_RT , valueIterator end_RT) : _Base()
		{
			vector<KeyObj<PK_T,RT> > v;

			//call insert of the base class
			//create a fstream object to write the records into the file.

			file_t.open("data.dat" , fstream::in | fstream::out  | fstream::trunc | fstream::binary);
			while (begin != end)
			{
				KeyObj<PK_T,RT> keyObj_t(*begin,*begin_RT, file_t,"data.dat");		//*begin will give key
				v.push_back(keyObj_t);
				++begin;
				++begin_RT;
			}
			this->_Base::insert_sequence(v.begin(),v.end());
		}
		
		~Btree()
		{
			file_t.close();
		}


		Iterator search(PK_T key);
		void display()
		{
			this->_Base::display();
		}
		Iterator begin()
		{
			return Iterator(this->_Base::begin());
		}
		Iterator end()
		{
			return Iterator(this->_Base::end());
		}
		Iterator last()
		{
			return Iterator(this->_Base::last());
		}
};



template<typename PK_T , typename RT , int MAX>
typename Btree<PK_T,RT,MAX>::Iterator Btree<PK_T,RT,MAX>::search(PK_T key)
{
	KeyObj<PK_T,RT> k(key);
	typename _Base::__Iterator it =_Base::search(k);
//	return (Btree<PK_T,RT,MAX>::Iterator) it;
	return Btree<PK_T,RT,MAX>::Iterator( it );
}



#endif
