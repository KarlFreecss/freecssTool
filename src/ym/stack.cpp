#include<iostream>

using namespace std;

class STACK{
private:
	int len;
	int data[100];
	int index;
public:
	int push(const int a);
	int pop(int & d);
	STACK();
};

STACK::STACK() : len(100), index(0){}

int STACK::push(const int a){
	if (index < len) {
		data[index++] = a;
		return 0;
	}
	return -1;
}

int STACK::pop(int &d){
	if (index > 0) {
		d = data[--index];
		return 0;
	}
	return -1;
}


int main(){
	STACK test;
	for (int i = 0; i < 101; ++i) {
		cout << "push : " << i;
		if (test.push(i) == 0) cout << " succeed" << endl;
		else cout << " failed" << endl;
	}
	for (int i = 0; i < 101; ++i) {
		int d;
		int flag = test.pop(d);
		if (flag == 0) cout << "pop value : " << d << endl;
		else cout << "pop failed" << endl;
	}
	return 0;
}
