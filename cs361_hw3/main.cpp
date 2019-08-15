#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <string>
#include <stdio.h>
#include <string.h>
#include <utility>
using namespace std;

//vertex point representation for BFS
struct VERTEX{
    //used to identify which character the vertex is
    char id;
    //color representation of vertex
    string color;
    //distance from origin value
    int d;
    //pointer to vertex preceding it in graph
    VERTEX* pi;
    //pointer list of vertexes adjacent  to this vertex
    vector<VERTEX*> adj;
};
//vertex point representation for BFS matrix version. Note: adj matrix not here. It is seperate.
struct VERTEX2{
    //used to identify which character the vertex is
    char id;
    //color representation of vertex
    string color;
    //distance from origin value
    int d;
};
//part 1
//prints the Matrix chain order with correct parenthesis placement
void Print_Optimal_parents(vector<vector<int>> s, int i, int j){
    if(i == j)
        cout << "A" << i << " ";
    else{
        cout << "(";
        Print_Optimal_parents(s, i, s[i][j]);
        Print_Optimal_parents(s, s[i][j] + 1, j);
        cout << ")";
    }
}

void Matrix_Chain_Order(vector<int> p){
    //n is the total number of vertexes
    int n = p.size() - 1;
   //m and s are the tables computed by this function
    vector<vector<int>> m;
    m.resize(n+1, vector<int>(n+1, 0)); 
    vector<vector<int>> s;
    s.resize(n, vector<int>(n+1, 0));
    //initiaize all values
    for(int i = 0; i <= n;  i++)
        for(int j = 0; j <= n; j++)
            m[i][j] = -1;
    //initialiize cross section to 0
    for(int i = 1; i <= n; i++)
        m[i][i] = 0;
    for(int l = 2; l <= n; l++){
        for(int i = 1; i <= n-l+1; i++){
            int j = i + l -1;

            for(int k = i ; k <= j-1; k++){
                int q = m[i][k] + m[k+1][j] + p.at(i-1) * p.at(k) * p.at(j);
                //checks for smallest q or if empty(==-1) to be used for m and s
                if(q < m[i][j] || m[i][j] == -1){
                    //places q in m table
                    m[i][j] = q;
                    //places k in s table
                    s[i][j] = k;
                }
            }
        }
    }
    //output number of multiplications
    cout << endl << "Num of multiplications: " <<  m[1][n] << endl;
    //this will end up outputing results with correct parenthesis placement
    Print_Optimal_parents(s,1,n);
}

//part2
int Lookup_Chain(vector<vector<int>> m, vector<int> p, int i, int j, vector<vector<int>> &s){
    //the case where m[i][j] is not empty, return it
     if(m[i][j] != -1)
         return m[i][j];
     //the case where you hit the cross section so it is initalize to 0
     if(i==j)
         m[i][j] = 0;
     else{
         for(int k = i; k <= j-1; k++){   
             //recursive call self
             int q= Lookup_Chain(m,p,i,k,s) + Lookup_Chain(m,p,k+1,j,s) + p.at(i-1) *p.at(k) * p.at(j);
             //checks for smallest q or if empty(==-1) to be used for m and s
             if(q < m[i][j] || m[i][j] == -1){
                 m[i][j] = q;
                 s[i][j] = k;
             }
         }
     }

     return m[i][j];
 }

void Memoized_Matrix_Chain(vector<int> p){
    //n is the total number of vertexes
    int n = p.size() -1;
    //m and s are the tables computed by this function
    vector<vector<int>> m;
    m.resize(n+1, vector<int>(n+1, 0));
    vector<vector<int>> s;
    s.resize(n, vector<int>(n+1, 0));
    //initilize every value in m table to -1
    for(int i = 1; i <= n; i++){
        for(int j= i; j <=n; j++){
            m[i][j] = -1;
        }
    }
     //output number of multiplications
     cout << endl << "Num of multiplications: " << Lookup_Chain(m,p,1, n, s) << endl;
     //this will end up outputing results with correct parenthesis placement
      Print_Optimal_parents(s,1,n);
}

//part 3
void BFS(vector<VERTEX> &G, VERTEX &s){
    //initialize every vertex
    for(int i = 0; i < int(G.size()); i++){
        G.at(i).color = "WHITE";
        G.at(i).d = -1;
        G.at(i).pi = NULL;
    }
    //initialize the start point index
    s.color = "GREY";
    s.d = 0;
    s.pi = NULL;
    //setup a queue
    queue<VERTEX*> Q;
    //enter start vertex into Q
    Q.push(&G.at(0));
    VERTEX* u;
    //Keep looping until Q is empty
    while(!Q.empty()){
        //set u to be the front of the Q and then remove it from Q
        u = Q.front();
        Q.pop();
        //loops though every adjacent vertex too vertex u
        for(int i = 0; i < int(u->adj.size()); i++){
            //setup strings to compare
            string st(u->adj.at(i)->color);
            string st2("WHITE");
            //check if adjacent vertex has not been passed by(is WHITE)
            if(st.compare(st2) == 0){
                //change color of the adjacent vertex to u, signals that it is avaliable
                u->adj.at(i)->color = "GREY";
                //make of the adjacent vertex to u one more than the one before it
                u->adj.at(i)->d = u->d + 1;
                //setup up pi of the adjacent vertex to u with u since it is the preceding vertex
                u->adj.at(i)->pi = u;
                //placed the adjacent vertex at i into the Q
                Q.push(u->adj.at(i));
            }
        }
        //Change the color of vertex u to BLACK, signals that it has been passed by
        u->color = "BLACK";
        //output the id of vertex u(the letter representing it)  in order to output every vertex passed by in order
        cout << u->id;
    }
}
//part 4

void BFSWithMatrix(pair< vector<VERTEX2>, vector<vector<int>>> &M, int s){
    //initialize every vertex in Ms vertex list
    for(int i = 0; i < int(M.first.size()); i++){
        M.first.at(i).color = "WHITE";
        M.first.at(i).d = -1;
    }
    //initialize the start point vertex in Ms vertex list
    M.first.at(s). color = "GREY";
    M.first.at(s).d = 0;
    //setup a queue
    queue<int> Q;
    //enter start vertex index into Q
    Q.push(s);
    //Keep looping until Q is empty
    while(!Q.empty()){
        //set u to be the front of the Q and then remove it from Q
        int u = Q.front();
        Q.pop();
        //loop though every possible vertex int the vertex Matrix for a row u
        for(int i = 0; i < int(M.second[u].size()); i++){
            //if the location in the matrix table has 1 then it is adjacent to vertex u in Ms vertex list
            if(M.second.at(u).at(i) == 1){
                //setup strings to compare
                string st(M.first.at(i).color);
                string st2("WHITE");
                 //check if adjacent vertex has not been passed by(is WHITE)
                if(st.compare(st2) == 0){
                    //change color of the adjacent vertex to u in Ms vertex list, signals that it is avaliable
                    M.first.at(i).color = "GREY";
                    //setup adjacent vertex to u in Ms vertex list to one more than the one before it
                    M.first.at(i).d = M.first.at(u).d;
                    //placed the adjacent vertex index i into the Q
                    Q.push(i);
                }
            }
        }
        //Change the color of vertex at u in Ms vertex list  to BLACK, signals that it has been passed by
        M.first[u].color = "BLACK";
        //output the id of vertex at u in Ms vertex list(the letter representing it)  in order to output every vertex passed by in order
        cout << M.first[u].id;
    }
}

int main()
{


    cout << "------------------(MCM algo.)--------------------";
    vector<int> data;
    data.push_back(30);
    data.push_back(4);
    data.push_back(8);
    data.push_back(5);
    data.push_back(10);
    data.push_back(25);
    data.push_back(15);
    Matrix_Chain_Order(data);
    cout << endl << endl << "------------------(MCM alg. Memoization ver.)--------------------";
    vector<int> data2;
    data2.push_back(30);
    data2.push_back(4);
    data2.push_back(8);
    data2.push_back(5);
    data2.push_back(10);
    data2.push_back(25);
    data2.push_back(15);
     Memoized_Matrix_Chain(data2);
    cout << endl << endl << "------------------(BFS with Adjacency List)--------------------" << endl;
    //vector of vertexes represent the adjacency list. Note: adjacent vertexes built into VERTEX struct
    vector<VERTEX> G(14); 
    //setup vertex vector with A to N
    for(int i = 0; i < 14; i++){
        G.at(i).id = char('A' + i);
    }
    //setup adjacency to each vertex
    //A:0 B:1 C:2 D:3 E:4 F:5 G:6 H:7 I:8 J:9 K:10 L:11 M:12 N:13
    //A
    G.at(0).adj.push_back(&G.at(1));
    G.at(0).adj.push_back(&G.at(3));
    //B
    G.at(1).adj.push_back(&G.at(0));
    G.at(1).adj.push_back(&G.at(2));
    //C
    G.at(2).adj.push_back(&G.at(1));
    G.at(2).adj.push_back(&G.at(12));
    G.at(2).adj.push_back(&G.at(13));
    //D
    G.at(3).adj.push_back(&G.at(0));
    G.at(3).adj.push_back(&G.at(4));
    G.at(3).adj.push_back(&G.at(5));
    G.at(3).adj.push_back(&G.at(6));
    G.at(3).adj.push_back(&G.at(13));
    //E
    G.at(4).adj.push_back(&G.at(3));
    G.at(4).adj.push_back(&G.at(5));
    //F
    G.at(5).adj.push_back(&G.at(3));
    G.at(5).adj.push_back(&G.at(4));
    G.at(5).adj.push_back(&G.at(7));
    //G
    G.at(6).adj.push_back(&G.at(3));
    G.at(6).adj.push_back(&G.at(7));
    G.at(6).adj.push_back(&G.at(9));
    //H
    G.at(7).adj.push_back(&G.at(5));
    G.at(7).adj.push_back(&G.at(6));
    G.at(7).adj.push_back(&G.at(8));
    G.at(7).adj.push_back(&G.at(10));
    //I
    G.at(8).adj.push_back(&G.at(7));
    G.at(8).adj.push_back(&G.at(9));
    //J
    G.at(9).adj.push_back(&G.at(6));
    G.at(9).adj.push_back(&G.at(8));
    G.at(9).adj.push_back(&G.at(10));
    G.at(9).adj.push_back(&G.at(13));
    //K
    G.at(10).adj.push_back(&G.at(7));
    G.at(10).adj.push_back(&G.at(9));
    G.at(10).adj.push_back(&G.at(11));
    //L
    G.at(11).adj.push_back(&G.at(10));
    G.at(11).adj.push_back(&G.at(12));
    //M
    G.at(12).adj.push_back(&G.at(2));
    G.at(12).adj.push_back(&G.at(11));
    G.at(12).adj.push_back(&G.at(13));
    //N
    G.at(13).adj.push_back(&G.at(2));
    G.at(13).adj.push_back(&G.at(3));
    G.at(13).adj.push_back(&G.at(9));
    G.at(13).adj.push_back(&G.at(12));

    cout << "Order passed by: ";
    BFS(G, G.at(0));
    cout << endl << endl << "------------------(BFS with Adjacency Matrix)--------------------" << endl;
    //set up list of Vertexes with specified info for each vertex
    vector<VERTEX2> vertexes(14);
    //setup up vetexes id with A to N
    for(int i = 0; i < 14; i++)
            vertexes.at(i).id = char('A' + i);
    //set up vertex matrix with info on adjacency
    vector<vector<int>> matrix;
    matrix.resize(14, vector<int>(14, 0));
    //initiallize all values in matrix to 0;
    for(int i = 0; i < 14; i++){
        for(int j = 0; j < 14; j++){
            matrix[i][j] = 0;
        }
    }
    //set up adjacency in matrix
    //A
    matrix.at(0).at(1) = 1;
    matrix.at(0).at(3) =1;
    //B
    matrix.at(1).at(0) = 1;
    matrix.at(1).at(2) = 1;
    //C
    matrix.at(2).at(12) = 1;
    matrix.at(2).at(13) =1;
    matrix.at(2).at(1) =1;
    //D
    matrix.at(3).at(4) = 1;
    matrix.at(3).at(5) =1;
    matrix.at(3).at(6) = 1;
    matrix.at(3).at(13) =1;
    matrix.at(3).at(0) =1;
    //E
    matrix.at(4).at(3) = 1;
    matrix.at(4).at(5) =1;
    //F
    matrix.at(5).at(3) =1;
    matrix.at(5).at(4) = 1;
    matrix.at(5).at(7) =1;
    //G
    matrix.at(6).at(3) =1;
    matrix.at(6).at(7) = 1;
    matrix.at(6).at(9) =1;
    //H
    matrix.at(7).at(5) =1;
   matrix.at(7).at(6) =1;
    matrix.at(7).at(8) = 1;
    matrix.at(7).at(10) =1;
    //I
    matrix.at(8).at(7) = 1;
    matrix.at(8).at(9) =1;
    //J
    matrix.at(9).at(6) = 1;
    matrix.at(9).at(8) =1;
    matrix.at(9).at(10) = 1;
    matrix.at(9).at(13) =1;
    //K
    matrix.at(10).at(7) =1;
    matrix.at(10).at(9) = 1;
    matrix.at(10).at(11) =1;
    //L
    matrix.at(11).at(10) = 1;
    matrix.at(11).at(12) =1;
    //M
    matrix.at(12).at(2) =1;
    matrix.at(12).at(11) = 1;
    matrix.at(12).at(13) =1;
    //N
    matrix.at(13).at(2) = 1;
    matrix.at(13).at(3) =1;
    matrix.at(13).at(9) = 1;
    matrix.at(13).at(12) =1;

    //pair up vector of specified vertex info and matrix with adjacenty info
    pair< vector<VERTEX2>, vector<vector<int>>> matrixAndData = make_pair(vertexes, matrix);
    cout << "Order passed by: ";
    BFSWithMatrix(matrixAndData, 0);



//     VERTEX A;
//     A.id = 'A';
//     A.d = 1;
//     VERTEX B;
//     B.id = 'B';
//     B.d = 2;
//     VERTEX C;
//     C.id = 'C';
//     C.d = 3;
//    A.adj.push_back(&B);
//    B.id = 'x';
//    A.adj.at(0)->id = 'z';
//    cout << A.adj.at(0)->id;

    cout << endl;

}
