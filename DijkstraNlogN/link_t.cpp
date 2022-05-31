#include "link_t.h"
#include <stdlib.h>

link_t::link_t(node_t* _item)
{
	item = _item; next = 0;
}

link_list::link_list()
{
	header = 0;last = 0;
}

link_list::~link_list()
{
	clear();
}

link_t* link_list::add(node_t* _item)
{
	link_t* newitem = new link_t(_item);
	if (!header) {
		header = newitem;
	}else
		last->next = newitem;
	last = newitem;
	return last;
}

bool link_list::contains(node_t* _item)
{
	link_t* temp = header;
	while (temp) {
		if (temp->item == _item)
			return true;
		temp = temp->next;
	}
	return false;
}

bool link_list::remove(node_t* _item)
{
	link_t* temp = header, *prev = NULL, *next = NULL;
	while (temp) {
		next = temp->next;
		if (temp->item == _item) {
			if (temp == header)	
				header = next;
			else if (temp == last) {
				last = prev;
				prev->next = NULL;
			}
			else
				prev->next = next;
			free(temp);
			return true;
		}
		prev = temp;
		temp = next;
	}
	return false;
}

bool link_list::isempty()
{
	return header == 0;
}

bool link_list::clear()
{
	link_t* temp = header, *next = 0;
	while (temp) {		
		next = temp->next;
		free(temp);
		temp = next;
	}
	header = 0; last = 0;
	return true;
}

int link_list::size()
{
	int cnt = 0;
	link_t* temp = header;
	while (temp) {
		cnt++;
		temp = temp->next;
	}
	return cnt;
}

