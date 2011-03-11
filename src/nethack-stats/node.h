// $Id: node.h 2 2007-07-19 13:00:48Z pdezwart $

#ifndef __NODE_H
#define __NODE_H

#include <string>

using namespace std;

class Node {
	friend istream & operator >> (istream &input, Node &node);
	friend ostream & operator << (ostream &output, const Node &node);

	public:
		Node();
		Node(string *s);
		Node(char *s);
		~Node();

		void setchild(Node *node);
		void setdata(string *s);
		void setdata(char *s);

		// inline data retrieval
		string * getdata() {
			return(data);
		}

		// inline child retrieval
		Node * getchild() {
			return(child);
		}

	private:
		string	*data;
		Node	*child;

		void initdata();
};

#endif
