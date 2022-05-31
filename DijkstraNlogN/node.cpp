#include "node.h"
#include "lsdb.h"
#include "nodeset.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern NodeSet ndset;
extern LSDB lsdb;

void node_t::buildNeighbours(NodeSet* _ndset, LSDB* _lsdb)
{
	ncnt = 0;
	memset(neighbours, 0, sizeof(node_t*) * MAX_NODE_CNT);
	for (int i=0;i<_ndset->cnt;i++)
	{
		node_t* to = _ndset->node_root[i];
		if (_lsdb->isneighbour(this, to)) {
			neighbours[ncnt++] = to;
		}
	}
}

node_t::node_t()
{
	memset(routelist, 0, MAX_NODE_CNT * sizeof(route_t));
}

node_t::~node_t()
{
	
}

int node_t::size()
{
	return 0;
}

void node_t::initroute(NodeSet* _ndset)
{
	memset(routelist, 0, MAX_NODE_CNT * sizeof(route_t));
	for (int i=0;i<_ndset->cnt;i++ )
	{
		route_t* temp = routelist + i;
		temp->src = this;
		temp->dst = _ndset->node_root[i];
		temp->min_len = MAX_DIST;
		if (this == temp->dst) temp->min_len = 0;
	}
}

/*
This case happens when:

The given graph G=(V, E) is represented as an adjacency matrix. Here w[u, v] stores the weight of edge (u, v).
The priority queue Q is represented as an unordered list.
Let |E| and |V| be the number of edges and vertices in the graph, respectively. Then the time complexity is calculated:

Adding all |V| vertices to Q takes O(|V|) time.
Removing the node with minimal dist takes O(|V|) time, and we only need O(1) to recalculate dist[u] and update Q. 
Since we use an adjacency matrix here, we’ll need to loop for |V| vertices to update the dist array.
The time taken for each iteration of the loop is O(|V|), as one vertex is deleted from Q per loop.
Thus, total time complexity becomes O(|V|) + O(|V|) \times O(|V|) = O(|V|^2).
*/
/*
* |E| lsdb
* |V| ndset
* |Q| uncalc
* |S| calc
*/
void node_t::updateNlogN(NodeSet* _ndset, LSDB* _lsdb)
{
	initroute(_ndset);

	link_list slist, qlist;
	for (int i = 0 ; i < _ndset->cnt ; i++)
		qlist.add(_ndset->node_root[i]);
	qlist.size();
	slist.clear();

	while (!qlist.isempty()) {
		node_t* minnode = findMinNode(&qlist);
		if(!minnode) break;

		qlist.remove(minnode);
		slist.add(minnode);
		route_t* rtemp = findRecord(minnode);

		for (int i = 0; i < minnode->ncnt; i++) {
			node_t* ntemp = minnode->neighbours[i];
			route_t* nrtemp = findRecord(ntemp);

			if(slist.contains(ntemp)) 
				continue;
			int newlen = _lsdb->directDist(minnode, ntemp) + rtemp->min_len;
			if (newlen < nrtemp->min_len) {
				nrtemp->min_len = newlen;
				if (minnode == this) 
					nrtemp->prev = ntemp;
				else 
					nrtemp->prev = minnode;
			}
		}
	}
	qlist.clear();
	slist.clear();
}

route_t* node_t::findRecord(node_t* _dst)
{
	for (int i=0;i<MAX_DIST;i++)
	{
		route_t* temp = routelist + i;
		if (temp->dst == _dst) 
			return temp;
	}
	return 0;
}

unsigned int node_t::getDist(node_t* _dest)
{
	route_t* rec = findRecord(_dest);
	return rec->min_len;
}

void node_t::showRouteTable()
{
	route_t* temp = NULL;
	for (int i = 0; i < MAX_NODE_CNT; i++)
	{
		temp = routelist+ i;
		if (temp->min_len < MAX_DIST && this != temp->dst && temp->min_len != 0)
			printf("%s, %s, %u\n", temp->dst->name, temp->prev->name, temp->min_len);
	}
}

void node_t::show(){
	printf("\n%s Neighbour Table:\n",name);
	node_t* nigh = NULL;
	for (int i=0;i<ncnt;i++)
	{
		nigh = neighbours[i];
		unsigned int len = lsdb.directDist(this, nigh);
		if(len > 0 && len < MAX_DIST) printf("%s %u\n", nigh->name, len);
	}
	printf("\n%s LSDB:\n",name);
	lsdb.show();

	printf("\n%s Routing Table:\n", name);
	showRouteTable();
}

node_t* node_t::findMinNode(link_list* _qlist)
{
	link_t* temp = _qlist->header;
	node_t* ret = NULL;
	int len = MAX_DIST;

	while (temp) {
		node_t* ntemp = (node_t*)temp->item;
		route_t* rttemp = findRecord(ntemp);
		if (rttemp->min_len < len) {
			len = rttemp->min_len;
			ret = ntemp;
		}
		temp = temp->next;
	}

	if (len == MAX_DIST) return 0;
	return ret;
}
