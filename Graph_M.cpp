#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <sstream>
#include <cmath>

using namespace std;

class Graph_M {
private:
    class Vertex {
    public:
        unordered_map<string, int> nbrs;
    };

    unordered_map<string, Vertex> vtces;

public:
    Graph_M() {}

    int numVertex() {
        return vtces.size();
    }

    bool containsVertex(string vname) {
        return vtces.find(vname) != vtces.end();
    }

    void addVertex(string vname) {
        Vertex vtx;
        vtces[vname] = vtx;
    }

    void removeVertex(string vname) {
        Vertex vtx = vtces[vname];
        vector<string> keys;
        for (auto& pair : vtx.nbrs) {
            keys.push_back(pair.first);
        }

        for (string key : keys) {
            Vertex nbrVtx = vtces[key];
            nbrVtx.nbrs.erase(vname);
        }

        vtces.erase(vname);
    }

    int numEdges() {
        int count = 0;
        for (auto& pair : vtces) {
            count += pair.second.nbrs.size();
        }
        return count / 2;
    }

    bool containsEdge(string vname1, string vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2)) {
            return false;
        }
        return vtces[vname1].nbrs.find(vname2) != vtces[vname1].nbrs.end();
    }

    void addEdge(string vname1, string vname2, int value) {
        if (!containsVertex(vname1) || !containsVertex(vname2) || containsEdge(vname1, vname2)) {
            return;
        }
        vtces[vname1].nbrs[vname2] = value;
        vtces[vname2].nbrs[vname1] = value;
    }

    void removeEdge(string vname1, string vname2) {
        if (!containsVertex(vname1) || !containsVertex(vname2) || !containsEdge(vname1, vname2)) {
            return;
        }
        vtces[vname1].nbrs.erase(vname2);
        vtces[vname2].nbrs.erase(vname1);
    }

    void displayMap() {
        cout << "\t Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------\n" << endl;
        for (auto& pair : vtces) {
            string key = pair.first;
            string str = key + " =>\n";
            Vertex vtx = pair.second;
            for (auto& nbr : vtx.nbrs) {
                str += "\t" + nbr.first + "\t";
                if (nbr.first.length() < 16) str += "\t";
                if (nbr.first.length() < 8) str += "\t";
                str += to_string(nbr.second) + "\n";
            }
            cout << str << endl;
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------\n" << endl;
    }

    void displayStations() {
        cout << "\n***********************************************************************\n" << endl;
        int i = 1;
        for (auto& pair : vtces) {
            cout << i << ". " << pair.first << endl;
            i++;
        }
        cout << "\n***********************************************************************\n" << endl;
    }

    bool hasPath(string vname1, string vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }
        processed[vname1] = true;
        Vertex vtx = vtces[vname1];
        for (auto& nbr : vtx.nbrs) {
            if (!processed[nbr.first]) {
                if (hasPath(nbr.first, vname2, processed)) {
                    return true;
                }
            }
        }
        return false;
    }

    struct DijkstraPair {
        string vname;
        string psf;
        int cost;
        bool operator<(const DijkstraPair& other) const {
            return other.cost < cost;
        }
    };

    int dijkstra(string src, string des, bool nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        priority_queue<DijkstraPair> heap;

        for (auto& pair : vtces) {
            DijkstraPair np;
            np.vname = pair.first;
            np.cost = INT_MAX;

            if (np.vname == src) {
                np.cost = 0;
                np.psf = np.vname;
            }

            heap.push(np);
            map[np.vname] = np;
        }

        while (!heap.empty()) {
            DijkstraPair rp = heap.top();
            heap.pop();

            if (rp.vname == des) {
                val = rp.cost;
                break;
            }

            map.erase(rp.vname);

            ans.push_back(rp.vname);

            Vertex v = vtces[rp.vname];
            for (auto& nbr : v.nbrs) {
                if (map.find(nbr.first) != map.end()) {
                    int oc = map[nbr.first].cost;
                    int nc;
                    if (nan)
                        nc = rp.cost + 120 + 40 * nbr.second;
                    else
                        nc = rp.cost + nbr.second;

                    if (nc < oc) {
                        DijkstraPair& gp = map[nbr.first];
                        gp.psf = rp.psf + nbr.first;
                        gp.cost = nc;

                        heap.push(gp);
                    }
                }
            }
        }
        return val;
    }

    struct Pair {
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };

    string getMinimumDistance(string src, string dst) {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        stack<Pair> st;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;
        
        st.push(sp);

        while (!st.empty()) {
            Pair rp = st.top();
            st.pop();

            if (processed[rp.vname]) {
                continue;
            }

            processed[rp.vname] = true;
            
            if (rp.vname == dst) {
                int temp = rp.min_dis;
                if (temp < min) {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (auto& nbr : rpvtx.nbrs) {
                if (!processed[nbr.first]) {
                    Pair np;
                    np.vname = nbr.first;
                    np.psf = rp.psf + nbr.first + "  ";
                    np.min_dis = rp.min_dis + nbr.second;
                    st.push(np);
                }
            }


        }
        return ans;
    }
};

int main() {
    Graph_M g;
    g.addVertex("Delhi");
    g.addVertex("Noida");
    g.addVertex("Gurgaon");
    g.addVertex("Faridabad");
    g.addVertex("Ghaziabad");
    g.addVertex("Bahadurgarh");
    g.addVertex("Ballabhgarh");
    g.addVertex("Lal Quila");
    g.addVertex("Jama Masjid");
    g.addVertex("Red Fort");
    g.addVertex("Rajiv Chowk");
    g.addVertex("Karol Bagh");
    g.addVertex("Dwarka");
    g.addVertex("Nehru Place");
    g.addVertex("Rithala");

    g.addEdge("Delhi", "Noida", 60);
    g.addEdge("Delhi", "Gurgaon", 50);
    g.addEdge("Delhi", "Faridabad", 60);
    g.addEdge("Delhi", "Ghaziabad", 50);
    g.addEdge("Delhi", "Bahadurgarh", 70);
    g.addEdge("Faridabad", "Ballabhgarh", 30);
    g.addEdge("Delhi", "Lal Quila", 20);
    g.addEdge("Delhi", "Jama Masjid", 20);
    g.addEdge("Lal Quila", "Jama Masjid", 10);
    g.addEdge("Jama Masjid", "Red Fort", 15);
    g.addEdge("Red Fort", "Rajiv Chowk", 20);
    g.addEdge("Rajiv Chowk", "Karol Bagh", 20);
    g.addEdge("Rajiv Chowk", "Dwarka", 30);
    g.addEdge("Rajiv Chowk", "Nehru Place", 40);
    g.addEdge("Nehru Place", "Kalkaji", 20);
    g.addEdge("Dwarka", "Rithala", 40);
    
    g.displayMap();
    g.displayStations();
    
    string source = "Delhi";
    string destination = "Noida";
    if (g.containsVertex(source) && g.containsVertex(destination)) {
        string path = g.getMinimumDistance(source, destination);
        cout << "Shortest path from " << source << " to " << destination << " is: " << path << endl;
    } else {
        cout << "Source or destination not found." << endl;
    }
    
    return 0;
}
