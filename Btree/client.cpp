#include"btree.h"
#include<cstdlib>
#include<ctime>
#include<bitset>

#define NUM_OF_ELEMENTS 100


int main()
{
    using namespace std;
    typedef bitset<8> MyByte;
    
	srand((unsigned)time(0));
    
    vector<int> a(NUM_OF_ELEMENTS);
	vector<MyByte> b;

	for(int i =0 ; i < NUM_OF_ELEMENTS  ; ++i)
	{
		a[i] = rand();
		b.emplace_back(MyByte(rand()));
	}	
	a[1] = 12;

	cout << "creation starts" << endl;
	Btree<int, MyByte, 3> tree( a.cbegin() , a.cend() , b.cbegin() , b.cend());
	cout << "creation ends" << endl;
	
	tree.display();

	auto it = tree.search(12);
    if(it != tree.end())
    {
        int key;
        MyByte value;
        tie(key,value) = *it;
        cout<<"key: " << key << " value: " << value << endl;
    }
    else
    {
        cout << "Key not found" << endl;
    }
	
    return 0;
}
