#ifndef NODE_HEADER
#define NODE_HEADER

#include "define.h"
#include "nodeset.h"
#include "record.h"
#include "link_t.h"

class route_t;
class LSDB;
class NodeSet;

#define MAX_NODE_CNT 16

class node_t
{
	
public:
	char name[NAME_LEN];
	route_t routelist[MAX_NODE_CNT];
	node_t* neighbours[MAX_NODE_CNT];
	int ncnt;

	node_t();
	~node_t();
	
	int size();
	void initroute(NodeSet* _ndset);

	void updateNlogN(NodeSet*, LSDB*);
	void buildNeighbours(NodeSet*, LSDB*);

	route_t* findRecord(node_t*);	
	unsigned int getDist(node_t* _dest);
	void showRouteTable();
	void show();

	node_t* findMinNode(link_list* _qlist);
};

#endif // NODE_HEADER
