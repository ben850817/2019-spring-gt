#ifndef HEADER
#define HEADER

#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<map>
#include<string>
#include<limits.h>
using namespace std;

class Net;
class Node
{
public:
	Node(){};
	Node(string s) {
		name = s;
	}
    string name;
	vector<Net *> connected_Nets;
	int ov_index;
};
class Net
{
public:
	Net(){};
	Net(Node *a, Node *b, bool f) {
		v1 = a;
		v2 = b;
		fake = f;
		flag = false;
	};
	Node *v1, *v2;
	bool flag, fake;
};

#endif