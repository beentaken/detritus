// $Id: node.cpp 2 2007-07-19 13:00:48Z pdezwart $

#include "node.h"

// Node with blank data
Node::Node() {
	child = NULL;

	initdata();
}

// Create a Node using a STL string as the data
Node::Node(string *s) {
	child = NULL;

	setdata(s);
}

// Create a Node using a c string as the data
Node::Node(char *s) {
	child = NULL;

	setdata(s);
}

Node::~Node() {
	delete data;
}

void Node::setchild(Node *n) {
	child = n;
}

void Node::initdata() {
	try {
		data = new string;
	} catch (exception e) {
		data = NULL;
		cerr << "Memory alloc error in Node::setdata" << endl;
		throw(e);
	}
};

void Node::setdata(string *s) {
	if (!data) {
		initdata();
	}

	*data = *s;
}

void Node::setdata(char *s) {
	if (!data) {
		initdata();
	}

	*data = s;
}

istream & operator >> (istream &input, Node &node) {
	if (!node.data) {
		node.initdata();
	}

	input >> *(node.data);

	return(input);
}

ostream & operator << (ostream &output, const Node &node) {
	output << *(node.data);

	return(output);
}
