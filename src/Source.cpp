#include"Header.h"
#include <map>
#include "string.h"
#include <stdlib.h>
#include"network_manager.h"

void readFile(const char* filename);
int Min_W_Matching(int, int);
vector<Node *> Hierholzer();
void tracePath();
void FindShortPath();
vector<string> ShowShortPath(string m, string n, Vertex *current);
//
vector<Node *> vlist, ovlist, euler_cycle;
vector<Net *> elist;
int **length_matrix;
Node **optimal_list;
int min_weight = INT_MAX;
vector<vector<Edge *>> avail_paths;
vector<vector<Edge *>> shortpath;
vector<vector<int>> val_of_shortpath;
Vertex *nm2_head;
NetworkManager *nm, *nm2;


int main(int argc, char *argv[])
{
	nm = new NetworkManager();
	nm->interpret(argv[1]);
	Path *path;
	path = new Path();
	path->append(nm->elist);

	readFile(argv[1]);
	FindShortPath();

	int ovindex = 0;
	for (int i = 0; i < vlist.size(); i++)
	{
		int degree = 0;
		for (int j = 0; j < vlist[i]->connected_Nets.size(); j++)
		{
			if (vlist[i]->connected_Nets[j]->v1->name == vlist[i]->connected_Nets[j]->v2->name)
				degree++;
		}
		if ((vlist[i]->connected_Nets.size() + degree) % 2 != 0)
		{
			vlist[i]->ov_index = ovindex;
			ovindex++;
			ovlist.push_back(vlist[i]);
		}
	}
	if (ovindex != 0);
	{
		optimal_list = new Node*[ovlist.size()];
		length_matrix = new int*[ovlist.size()];
		for (int i = 0; i < ovlist.size(); i++)
		{
			length_matrix[i] = new int[ovlist.size()];
			for (int j = 0; j < ovlist.size(); j++)length_matrix[i][j] = 0;
		}
		for (int i = 0; i < ovlist.size() - 1; i++)
		{
			for (int j = i + 1; j < ovlist.size(); j++)
			{
				vector<string> vertex_path = ShowShortPath(ovlist[i]->name, ovlist[j]->name, nm2_head);
				int shortest_path_length = vertex_path.size() - 1;
				length_matrix[i][j] = shortest_path_length, length_matrix[j][i] = shortest_path_length;
			}
		}
		int a = Min_W_Matching(0, 0);
		for (int i = 0; i < elist.size(); i++)elist[i]->fake = false;
		Net *extra_edge;
		for (int i = 0; i < ovlist.size(); i += 2)
		{
			extra_edge = new Net();
			extra_edge->v1 = ovlist[i], extra_edge->v2 = ovlist[i + 1], extra_edge->fake = true;
			ovlist[i]->connected_Nets.push_back(extra_edge);
			ovlist[i + 1]->connected_Nets.push_back(extra_edge);
			elist.push_back(extra_edge);
		}
	}

	euler_cycle = Hierholzer();
	tracePath();


	system("pause");
	return 0;
}
void readFile(const char *filename)
{
	ifstream in_file(filename, ios::in);

	if (!in_file) {
		cout << "The file cannot be opened." << endl;
		exit(1);
	}

	Node *v1, *v2;
	Net *edge;
	string line, s1, s2;
	const char *delimiters = " \t";
	map< string, Node * > hash_tb;
	map< string, Node * >::iterator iter_v1, iter_v2;

	while (!in_file.eof()) {
		getline(in_file, line);
		if (line.length() == 0) continue;

		s1 = strtok(strdup(line.c_str()), delimiters);
		s2 = strtok(NULL, delimiters);

		iter_v1 = hash_tb.find(s1);
		if (iter_v1 == hash_tb.end()) {
			v1 = new Node(s1);
			hash_tb[s1] = v1;
			vlist.push_back(v1);
		}
		else
			v1 = iter_v1->second;

		iter_v2 = hash_tb.find(s2);
		if (iter_v2 == hash_tb.end()) {
			v2 = new Node(s2);
			hash_tb[s2] = v2;
			vlist.push_back(v2);
		}
		else
			v2 = iter_v2->second;

		edge = new Net(v1, v2, false);
		v1->connected_Nets.push_back(edge);
		if (v1->name != v2->name)
			v2->connected_Nets.push_back(edge);
		elist.push_back(edge);
	}

	in_file.close();
}

int	Min_W_Matching(int w, int start)
{
	int min_w = INT_MAX;

	if (start == ovlist.size() - 2)
	{
		int weight = length_matrix[ovlist[start]->ov_index][ovlist[start + 1]->ov_index];
		if (w + weight < min_weight)
		{
			min_weight = w + weight;
			for (int i = 0; i < ovlist.size(); i++)optimal_list[i] = ovlist[i];
		}
		return weight;
	}

	for (int i = start + 1; i < ovlist.size(); i++)
	{
		int weight = length_matrix[ovlist[start]->ov_index][ovlist[i]->ov_index];
		Node *tmp = ovlist[start + 1];
		ovlist[start + 1] = ovlist[i];
		ovlist[i] = tmp;
		weight += Min_W_Matching(w + weight, start + 2);
		tmp = ovlist[start + 1];
		ovlist[start + 1] = ovlist[i];
		ovlist[i] = tmp;
		if (weight < min_w)min_w = weight;
	}
	return min_w;
}
vector<Node *> Hierholzer()
{
	Node *top;
	stack<Node *> tmp_path;
	vector<Node *> final_path;
	for (int i = 0; i < elist.size(); i++)elist[i]->flag = false;
	if (!vlist.empty())tmp_path.push(vlist.front());
	while (!tmp_path.empty()) 
	{
		top = tmp_path.top();
		int i;
		for (i = 0; i < top->connected_Nets.size(); i++)
		{
			if (!top->connected_Nets[i]->flag) 
			{
				top->connected_Nets[i]->flag = true;
				if (top->connected_Nets[i]->v1->name != top->name)tmp_path.push(top->connected_Nets[i]->v1);
				else tmp_path.push(top->connected_Nets[i]->v2);
				break;
			}
		}
		if (i == top->connected_Nets.size()) 
		{
			tmp_path.pop();
			final_path.push_back(top);
		}
	}
	return final_path;
}
void tracePath()
{
	ofstream out("output.txt", ios::app);
	vector<string> vertex_path;
	for (int i = 0; i < euler_cycle.size() - 1; i++) 
	{
		for (int j = 0; j < euler_cycle[i]->connected_Nets.size(); j++) 
		{
			if (euler_cycle[i]->connected_Nets[j]->v1->name == euler_cycle[i + 1]->name ||
				euler_cycle[i]->connected_Nets[j]->v2->name == euler_cycle[i + 1]->name) 
			{
				if (!euler_cycle[i]->connected_Nets[j]->fake)
				{
					cout << euler_cycle[i]->name << "->";
					out << euler_cycle[i]->name << "->";
				}
				else 
				{
					vertex_path = ShowShortPath(euler_cycle[i]->name, euler_cycle[i + 1]->name, nm2_head);
					for (unsigned int k = 0; k < vertex_path.size() - 1; ++k)
					{
						cout << vertex_path[k] << "->";
						out << vertex_path[k] << "->";
					}
				}
				break;
			}
		}
	}
	cout << euler_cycle.back()->name << endl;
	out << euler_cycle.back()->name << endl;
}
void FindShortPath()
{
	nm2 = new NetworkManager();
	int num_v = 0;
	Vertex *ver0 = nm->get_all_nodes();
	Vertex *ver_head = ver0;
	while (ver0 != NULL) 
	{
		nm2->add_switch(ver0->name);
		num_v++;
		ver0 = ver0->next;
	}
	Vertex *ver1 = ver_head;
	Vertex *ver2 = ver1;
	Edge *edge1 = nm->elist;
	while (edge1 != NULL)
	{
		if (nm2->connected(edge1->head->name, edge1->tail->name) == 1)
			nm2->connect_r(edge1->head->name, edge1->tail->name);
		edge1 = edge1->next;
	}
	Path *path;
	path = new Path();
	path->append(nm2->elist);
	Vertex *v = nm2->get_all_nodes();
	Vertex *v2 = v;
	nm2_head = v;
	Vertex *v_tail;
	while (v2 != NULL) 
	{
		if (v2->next == NULL)v_tail = v2;
		v2 = v2->next;
	}
	v2 = v;
	while (v != NULL) 
	{
		while (v2 != NULL) 
		{
			if (v2 == v)v2 = v2->next;
			if (v2 != NULL) 
			{
				avail_paths = path->find_paths(v->name, v2->name);
				v2 = v2->next;
				int shorter = 0;
				for (int i = 0; i < avail_paths.size(); i++)
				{
					if (avail_paths[i].size() < avail_paths[shorter].size())shorter = i;
				}
				shortpath.push_back(avail_paths[shorter]);
			}
		}
		v = v->next;
		v2 = nm2_head;
	}
	vector<int> val_of_line;
	int count = 0;
	for (int i = 0; i<shortpath.size(); i++)
	{
		for (int j = 0; j<shortpath[i].size(); j++) 
		{
		}
		count++;
		val_of_line.push_back(shortpath[i].size());
		if (count == num_v - 1)
		{
			int pos = (i + 1) / count - 1;
			val_of_line.insert(val_of_line.begin() + pos, 0);
			val_of_shortpath.push_back(val_of_line);
			val_of_line.clear();
			count = 0;
		}
	}
}
vector<string> ShowShortPath(string m, string n, Vertex *current)
{
	vector<string> vertex_path;
	int order_m = 0;
	int order_n = 0;
	int count = 0;
	int num = 0;
	while (current != NULL) 
	{
		num++;
		if (n == current->name)order_n = count;
		if (m == current->name)order_m = count;
		current = current->next;
		count++;
	}
	int order_in_path = 0;
	if (order_m > order_n)order_in_path = order_m*(num - 1) + order_n;
	if (order_m < order_n)order_in_path = order_m*(num - 1) + order_n - 1;
	vertex_path.push_back(m);
	for (int i = 0; i < shortpath[order_in_path].size(); i++)
		vertex_path.push_back(shortpath[order_in_path][i]->tail->name);
	return vertex_path;
}
