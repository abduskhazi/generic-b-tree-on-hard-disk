#include"btree.h"
#include<cstdlib>
#include<ctime>
#include<bitset>

#define NUM_OF_ELEMENTS 100

using namespace std;

int main()
{
	srand(time(0));
	int *a = new int[NUM_OF_ELEMENTS];		
	bitset<8> *b = new bitset<8>[NUM_OF_ELEMENTS];

	for(int i =0 ; i < NUM_OF_ELEMENTS  ; ++i)
	{
		a[i] = rand();
		b[i] = bitset<8>( (unsigned long)rand() );
	}	
	a[1] = 12;

	cout << "creation starts" << endl;
	Btree<int, bitset<8>, 3> tree(a,a+NUM_OF_ELEMENTS ,b,b+NUM_OF_ELEMENTS );
	cout << "creation ends";

//	Btree<int, bitset<75>, 3> tree(a,a+NUM_OF_ELEMENTS ,b,b+NUM_OF_ELEMENTS );
//__Btree< int , 3> tree( a , a+ NUM_OF_ELEMENTS);

	
	tree.display();
	cout << "The iterator display:";
	cout<<endl;
	
	Btree<int,bitset<8>,3>::Iterator it = tree.begin();
	while(it!=tree.end())
	{
		pair< int , bitset<8> > p = *it;
		cout<<"key: "<<p.first<<"  value: "<< p.second << " ";

		cout << "RAM stored : ";
		it.display();
		cout<<endl;
		++it;
	}
	it = tree.search(12);
	pair< int , bitset<8> > p = *it;
	cout<<"key: "<<p.first<<"value: "<<p.second<<endl;
	
    return 0;
}
