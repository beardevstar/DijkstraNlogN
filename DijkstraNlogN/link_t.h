#ifndef LINK_T_HEADER
#define LINK_T_HEADER

#pragma once
class node_t;

class link_t
{
public:
	node_t* item;
	link_t* next;
public:
	link_t(node_t* _item);
};

class link_list
{
public:
	link_list();
	~link_list();

	link_t* header, * last;
	link_t* add(node_t*);
	bool contains(node_t*);
	bool remove(node_t*);
	bool isempty();
	bool clear();
	int size();
private:

};
#endif // !LINK_T_HEADER
