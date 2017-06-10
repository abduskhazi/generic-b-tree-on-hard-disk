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
    
    vector<int> keyList;
	vector<MyByte> valueList;

    keyList.reserve(NUM_OF_ELEMENTS);
    valueList.reserve(NUM_OF_ELEMENTS);
    
	for(int i =0 ; i < NUM_OF_ELEMENTS  ; ++i)
	{
		keyList.emplace_back(rand());
		valueList.emplace_back(MyByte(rand()));
	}	
	keyList[1] = 12;

	Btree<int, MyByte, 3> tree( keyList.cbegin() , keyList.cend() , valueList.cbegin() , valueList.cend());
	
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
