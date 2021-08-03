#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

void print_vect_vect( vector<vector<double>> v){
    int len = v.size();
    for (int i=0; i<len; i++){
        int len_sub = v[i].size();
        for (int j=0; j<len_sub; j++){
            cout<<v[i][j]<<',';
        }
        cout<<endl;
    }
}

void print_vect( vector<double> v){
    int len = v.size();
    for (int i=0; i<len; i++){
        cout<<v[i]<<endl;
    }
}

bool comp_sort_edge(const vector<double> &a, const vector<double> &b){
    return(a[2]<b[2]);
}


class Clustering{
    public:

    string file_address;
    vector<vector<double>> graph_edges;
    vector<vector<double>> graph_adj;
    vector<double> node_to_cluster;
    map<double,vector<double>> cluster_to_nodes;
    double num_of_nodes;
    double num_of_edges;
    

    void read_graph(){
        ifstream infile;
        string row;
        double node1;
        double node2;
        double weight;

        infile.open(file_address);
        infile>>num_of_nodes;
        vector<double> init;
        graph_adj.resize(num_of_nodes,init);

        getline(infile,row);

        int edge_number = 0;

        while(infile.good()){
            getline(infile,row);
            stringstream s(row);
            s>>node1;
            s>>node2;
            s>>weight;

            vector<double> edge = {node1-1, node2-1, weight};
            graph_edges.push_back(edge);
            graph_adj[node1-1].push_back(edge_number);
            graph_adj[node2-1].push_back(edge_number);
            edge_number = edge_number+1;
        }
        num_of_edges = edge_number;

    }

    void set_node_to_cluster(){
        for (int i=0; i<num_of_nodes; i++){
            node_to_cluster.push_back(i);
        }
    }

    void set_cluster_to_nodes(){
        for (int i=0; i<num_of_nodes; i++){
            vector<double> v = {double(i)};
            cluster_to_nodes.insert(make_pair(i,v));
        }
    }

    void merge_clusters(int c1, int c2){
        auto it1 = cluster_to_nodes.find(c1);
        auto it2 = cluster_to_nodes.find(c2);

        vector<double> &nodes1 = it1->second;
        vector<double> &nodes2 = it2->second;

        if (nodes1.size() > nodes2.size()){
            int len = nodes2.size();

            for (int i=0; i<len; i++){
                double node = nodes2[i];
                nodes1.push_back(node);
                node_to_cluster[node] = c1;
            }
            cluster_to_nodes.erase(it2);

        }else{
            int len = nodes1.size();

            for (int i=0; i<len; i++){
                double node = nodes1[i];
                nodes2.push_back(node);
                node_to_cluster[node] = c2;
            }
            cluster_to_nodes.erase(it1);
        }
        
    }

    void find_clusters(){
        int edge_counter = 0; 
        while(cluster_to_nodes.size() > 4){
            vector<double> & edge = graph_edges[edge_counter];
            double node1 = edge[0];
            double node2 = edge[1];
            double cluster_node1 =  node_to_cluster[node1];
            double cluster_node2 =  node_to_cluster[node2];
            if(cluster_node1 != cluster_node2){
                merge_clusters(cluster_node1,cluster_node2);
            }
            edge_counter=edge_counter+1;

        }
        find_spacing(edge_counter);

    }

    void find_spacing(int starting_edge){
        int edge_counter = 0;
        while (true){
            vector<double> & edge = graph_edges[edge_counter];
            double node1 = edge[0];
            double node2 = edge[1];
            double cluster_node1 =  node_to_cluster[node1];
            double cluster_node2 =  node_to_cluster[node2];
            if(cluster_node1 != cluster_node2){
                cout<<edge[2];
                return;
            }
            edge_counter=edge_counter+1;
        }
    }

    Clustering(string file_add){
        file_address = file_add;
        read_graph();
        sort(graph_edges.begin(),graph_edges.end(),comp_sort_edge);
        set_node_to_cluster();
        set_cluster_to_nodes();
        find_clusters();
    }


};


int main(){

    string file_address = "G:\\My Drive\\course\\Algorithm\\Clustering\\clustering.txt";
    Clustering cls(file_address);

    return 0;
}