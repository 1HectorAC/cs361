#include <iostream>
#include <vector>
#include <map>
using namespace std;


struct STATE{
    //holds different states a single state points to
    map<char, STATE*> dir;
    //determine if state is an accept state
    bool accepted = false;
};

struct VERTEX{
    char id;
    int d;
    VERTEX* pi;

};

bool stringCheck(vector<STATE> states, string word){
    //current is start state that we will move from
    STATE* current = &states[0];
    //loop though every every char in the word string
    for(int i = 0; i < int(word.length()); i++){
        //set current to next state determined by current char from word
        current = current->dir.find(word[i])->second;
    }
    //check if current state (or state we ended in) is accepted and return
    if(current->accepted)
        return true;
    else
        return false;
}

void Initialize_Single_Source(pair<vector<VERTEX>, vector<pair<int, int>>> &G, VERTEX& s){
   //initialize every vertex with a distance and pi(predecessor)
    for(int i = 0; i < int(G.first.size()); i++){
        //note: all distances initialized to 1000. This will work for this example but wont for dealing with high distance values
        G.first.at(i).d = 1000;
        //initialize pi(or predecessor) to null
        G.first.at(i).pi = NULL;
    }
    //set distance of start vertex to 0
    s.d = 0;
}

void Relax(VERTEX& u, VERTEX& v, map<pair<char,char>, int> w){
    //checks if distance of v > distance of u + edge from u to v
    //note: distance of u cant be equal to 1000 since it is a substitute for infinity in this case
    if(v.d > u.d + w.find(make_pair(u.id, v.id))->second && u.d != 1000){
        //sets v distance to distance of u + edge from u to v
        v.d = u.d +  w.find(make_pair(u.id, v.id))->second;
        //sets v predecessor to u
        v.pi = &u;
    }
}

bool Bellman_Ford(pair<vector<VERTEX>, vector<pair<int, int>>> &G, map<pair<char,char>, int> w, VERTEX& s){
    //setup the distance and predecessor for each vertex
    Initialize_Single_Source(G, s);
    //loop for every vertex contained - 2 amount of times
    for(int i = 1; i <= int(G.first.size()) - 1; i++){
        //loop for every edge containted amount of times
        for(int j = 0; j < int(G.second.size()); j++){
            Relax(G.first.at(G.second.at(j).first), G.first.at(G.second.at(j).second), w);
        }
    }
    //output final distances and predecessors for each vertex
    cout << "ID: DISTANCE PREDECESSOR" << endl;
    for(int i = 0; i < int(G.first.size()); i++){
        cout << G.first.at(i).id << " : " << G.first.at(i).d << " ";
        if(G.first.at(i).pi != NULL)
            cout << G.first.at(i).pi->id << endl;
        else
            cout << "NULL" << endl;
    }
    //loop for every edge containted amount of times
    for(int j = 0; j < int(G.second.size()); j++){
        //last check to see if there is negative weight cycle
        if(G.first.at(G.second.at(j).second).d >
                G.first.at(G.second.at(j).first).d +
                w.find(make_pair(G.first.at(G.second.at(j).first).id, G.first.at(G.second.at(j).second).id))->second &&
                G.first.at(G.second.at(j).first).d != 1000){
            return false;
        }
    }
    return true;
}

int main()
{
    cout << "*************************************finite automaton part******************************" << endl;
    //vector of all the states
    //note: s = 0 q1 = 1, q2 = 2, r1 = 3 r2 = 4
    vector<STATE> states(5);
    //settting certain states to be accepted states
    states[1].accepted = true;
    states[3].accepted = true;
    //setting up arrows or pointers directions
    states[0].dir['a'] = &states[1];
    states[0].dir['b'] = &states[3];
    states[1].dir['a'] = &states[1];
    states[1].dir['b'] = &states[2];
    states[2].dir['a'] = &states[1];
    states[2].dir['b'] = &states[2];
    states[3].dir['a'] = &states[4];
    states[3].dir['b'] = &states[3];
    states[4].dir['a'] = &states[4];
    states[4].dir['b'] = &states[3];
    //checking strings with machine
    string test1 = "ababa";
    if(stringCheck(states, test1))
        cout << test1 << " accepted" << endl;
    else
        cout << test1 << " denied" << endl;

    string test2 = "baba";
    if(stringCheck(states, test2))
        cout << test2 << " accepted" << endl;
    else
        cout << test2 << " denied" << endl;

    string test3 = "aababaab";
    if(stringCheck(states, test3))
        cout << test3 << " accepted" << endl;
    else
        cout << test3 << " denied" << endl;

    string test4 = "babaabaaabb";
    if(stringCheck(states, test4))
        cout << test4 << " accepted" << endl;
    else
        cout << test4 << " denied" << endl;

    string test5 = "";
    if(stringCheck(states, test5))
        cout << "empty" << " accepted" << endl;
    else
        cout << "empty" << " denied" << endl;
    cout << "****************************************bellman ford**********************************" << endl;
   //vector holding list of vertices
    vector<VERTEX> vertices(14);
   //initialize vertex values;
    //setup up vertices id with A to N
    for(int i = 0; i < 14; i++)
            vertices.at(i).id = char('A' + i);
    //setup edges of vertices
    vector<pair<int, int>> edges(21);
    //A:0 B:1 C:2 D:3 E:4 F:5 G:6 H:7 I:8 J:9 K:10 L:11 M:12 N:13
    edges.at(0) = make_pair(0,3);
    edges.at(1) = make_pair(1,0);
    edges.at(2) = make_pair(2,1);
    edges.at(3) = make_pair(2,13);
    edges.at(4) = make_pair(2,12);
    edges.at(5) = make_pair(3,4);
    edges.at(6) = make_pair(3,5);
    edges.at(7) = make_pair(3,6);
    edges.at(8) = make_pair(3,13);
    edges.at(9) = make_pair(4,5);
    edges.at(10) = make_pair(5,7);
    edges.at(11) = make_pair(6,7);
    edges.at(12) = make_pair(6,9);
    edges.at(13) = make_pair(7,10);
    edges.at(14) = make_pair(8,7);
    edges.at(15) = make_pair(9,8);
    edges.at(16) = make_pair(9,10);
    edges.at(17) = make_pair(11,10);
    edges.at(18) = make_pair(12,11);
    edges.at(19) = make_pair(13,13);
    edges.at(20) = make_pair(13,2);
    //G holds vertices and edges list
    pair<vector<VERTEX>, vector<pair<int, int>>> G = make_pair(vertices, edges);
    //holds edge weights
    map<pair<char,char>, int> w;
    //setup edge weights
    //A:0 B:1 C:2 D:3 E:4 F:5 G:6 H:7 I:8 J:9 K:10 L:11 M:12 N:13
    w[make_pair(G.first.at(0).id, G.first.at(3).id)] = 3;
    w[make_pair(G.first.at(1).id, G.first.at(0).id)] = -2;
    w[make_pair(G.first.at(2).id, G.first.at(1).id)] = 1;
    w[make_pair(G.first.at(2).id, G.first.at(13).id)] = -3;
    w[make_pair(G.first.at(2).id, G.first.at(12).id)] = 3;
    w[make_pair(G.first.at(3).id, G.first.at(4).id)] = 2;
    w[make_pair(G.first.at(3).id, G.first.at(5).id)] = 6;
    w[make_pair(G.first.at(3).id, G.first.at(6).id)] = -1;
    w[make_pair(G.first.at(3).id, G.first.at(13).id)] = -1;
    w[make_pair(G.first.at(4).id, G.first.at(5).id)] = 3;
    w[make_pair(G.first.at(5).id, G.first.at(7).id)] = -2;
    w[make_pair(G.first.at(6).id, G.first.at(7).id)] = 1;
    w[make_pair(G.first.at(6).id, G.first.at(9).id)] = 3;
    w[make_pair(G.first.at(7).id, G.first.at(10).id)] = -1;
    w[make_pair(G.first.at(8).id, G.first.at(7).id)] = -4;
    w[make_pair(G.first.at(9).id, G.first.at(8).id)] = 2;
    w[make_pair(G.first.at(9).id, G.first.at(10).id)] = 3;
    w[make_pair(G.first.at(11).id, G.first.at(10).id)] = 2;
    w[make_pair(G.first.at(12).id, G.first.at(11).id)] = -4;
    w[make_pair(G.first.at(13).id, G.first.at(13).id)] = 8;
    w[make_pair(G.first.at(13).id, G.first.at(2).id)] = -3;

    if(Bellman_Ford(G, w, G.first.at(0)))
        cout << "no negative weight cycle detected" << endl;
    else
        cout << "negative weight cycle detected" << endl;

}
