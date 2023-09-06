#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cmath>   
#include <iomanip>
#include <queue>
using namespace std;

struct HeapNode{
    int key, id;
    HeapNode():key(0),id(0){};
    HeapNode(int key, int id):key(key),id(id){};
};

class BinaryHeap{
private:
    std::vector<HeapNode> heap;
    void swap(struct HeapNode &p1, struct HeapNode &p2);
    int FindPosition(int node);
    int GetParentNode(int node){return std::floor(node/2);};
public:
    BinaryHeap(){heap.resize(1);};
    BinaryHeap(int n){
        heap.resize(n + 1);          // 主要存放vertex及其distance的vector
    }
    // Min-Priority Queue
    void MinHeapify(int node, int length);
    void BuildMinHeap(vector<int> array);
    void DecreaseKey(int node, int newKey);
    void MinHeapInsert(int node, int key);
    int ExtractMin();                               // 回傳heap[1]並調整Heap
    int Minimum(){return heap[1].id;};         // 回傳heap[1]
    HeapNode get(int id);
    bool IsHeapEmpty(){return (heap.size()<=1);};

};

HeapNode BinaryHeap::get(int id){
    int idx = FindPosition(id);
    return heap[idx];
}

int BinaryHeap::FindPosition(int node){

    int idx = 0;
    for (int i = 1; i < heap.size(); i++) {
        if (heap[i].id == node) {
            idx = i;
        }
    }
    return idx;
}
void BinaryHeap::MinHeapInsert(int node, int key){

    heap.push_back(HeapNode(node,key));
    DecreaseKey(node, key);
}
void BinaryHeap::DecreaseKey(int node, int newKey){

    int index_node = FindPosition(node);      // 找到node所在的位置index

    if (newKey >= heap[index_node].key) {      // 如果不是把node的Key下修, 便終止此函式
        //std::cout << "new key is not smaller than current key\n";
        return;
    }
    heap[index_node].key = newKey;            // 更新node之Key後,   
                                              // 需要檢查是否新的subtree滿足Min Heap
    while (index_node > 1 && heap[GetParentNode(index_node)].key > heap[index_node].key) {
        swap(heap[index_node], heap[GetParentNode(index_node)]);
        index_node = GetParentNode(index_node);
    }
}
void BinaryHeap::swap(struct HeapNode &p1, struct HeapNode &p2){

    struct HeapNode temp = p1;
    p1 = p2;
    p2 = temp;
}
int BinaryHeap::ExtractMin(){

    if (IsHeapEmpty()) {
        std::cout << "error: heap is empty\n";
        exit(-1);
    }
    int min = heap[1].id;    // 此時heap的第一個node具有最小key值
    // 便以min記錄其id, 最後回傳min
    // delete the first id/vertex
    heap[1] = heap[heap.size()-1];            // 把最後一個id放到第一個位置,
    heap.erase(heap.begin()+heap.size()-1);   // 再刪除最後一個id
    MinHeapify(1, (int)heap.size());          // 目前, heap[1]具有最大Key, 需要進行調整

    return min;       // 回傳heap中具有最小key的id
}
void BinaryHeap::BuildMinHeap(vector<int> array){
    int a, b;
    // 將array[]的資料放進 heap之矩陣中, 並預留 heap[0] 不做使用
    for (int i = 0; i < array.size(); i++) {
        heap[i + 1].id = i;                 // 把array[]的idx視為id
        heap[i + 1].key = array[i];              // 把array[]的數值視為key
    }
    for (int i = (int)heap.size()/2; i >= 1 ; i--) {
        MinHeapify(i, (int)heap.size()-1);     // length要減一, 因為heap從從1開始存放資料
    }
}
void BinaryHeap::MinHeapify(int node, int length){

    int left = 2*node,          // 取得left child
        right = 2*node + 1,     // 取得right child
        smallest;               // smallest用來記錄包含root與child, 三者之中Key最小的node

    if (left <= length && heap[left].key < heap[node].key)
        smallest = left;
    else
        smallest = node;

    if (right <= length && heap[right].key < heap[smallest].key)
        smallest = right;

    if (smallest != node) {                 // 如果目前node的Key不是三者中的最小
        swap(heap[smallest], heap[node]);   // 就調換node與三者中Key最小的node之位置
        MinHeapify(smallest, length);       // 調整新的subtree成Min Heap
    }
}

const int Max_Distance = INT32_MAX;
class Graph_SP{             // SP serves as Shortest Path
private:
    int num_vertex;
    std::vector<std::list<std::pair<int,int>>> AdjList;
    std::vector<int> predecessor, distance;
    std::vector<bool> visited;
public:
    Graph_SP():num_vertex(0){};
    Graph_SP(int n):num_vertex(n){
        AdjList.resize(num_vertex);
    }
    void AddEdge(int from, int to, int weight);
    void PrintDataArray(std::vector<int> array);
    void PrintIntArray(int *array);

    void InitializeSingleSource(int Start);     // 以Start作為起點
    void Relax(int X, int Y, int weight);       // edge方向：from X to Y

    vector<int> Dijkstra(int Start, ofstream &ofs);        // 需要Min-Priority Queue
    friend class BinaryHeap;             // 以Binary Heap實現Min-Priority Queue
};
vector<int> Graph_SP::Dijkstra(int Start, ofstream &ofs){

    InitializeSingleSource(Start);

    BinaryHeap minQueue(num_vertex);   // object of min queue
    minQueue.BuildMinHeap(distance);

    visited.resize(num_vertex, false);   // initializa visited[] as {0,0,0,...,0}

    while (!minQueue.IsHeapEmpty()) {
        int u = minQueue.ExtractMin();
        for (std::list<std::pair<int, int>>::iterator itr = AdjList[u].begin();
             itr != AdjList[u].end(); itr++) {

            Relax(u, (*itr).first, (*itr).second);
            minQueue.DecreaseKey((*itr).first, distance[(*itr).first]);
        }
    }
    ofs<<"s: "<<0<<endl;
    ofs<<"t: "<<distance[num_vertex-1]<<endl;
    for(int i=1;i<num_vertex-1;i++){
        if(distance[i]!=INT32_MAX)
            ofs<<"a"<<i<<": "<<distance[i]<<endl;
        else
            ofs<<"a"<<i<<": -1"<<endl;
    }
    return predecessor;
}
void Graph_SP::InitializeSingleSource(int Start){

    distance.resize(num_vertex);
    predecessor.resize(num_vertex);

    for (int i = 0; i < num_vertex; i++) {
        distance[i] = Max_Distance;
        predecessor[i] = -1;
    }
    distance[Start] = 0;
}
void Graph_SP::Relax(int from, int to, int weight){

    if (distance[to] > distance[from] + weight) {
        distance[to] = distance[from] + weight;
        predecessor[to] = from;
    }
}
void Graph_SP::AddEdge(int from, int to, int weight){

    AdjList[from].push_back(std::make_pair(to,weight));
}
void Graph_SP::PrintDataArray(std::vector<int> array){
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << i;
    std::cout << std::endl;
    for (int i = 0; i < num_vertex; i++)
        std::cout << std::setw(4) << array[i];
    std::cout << std::endl;
}

class Graph_FlowNetWorks{
private:
    int num_vertex;
    std::vector<std::vector<int>> AdjMatrix;
public:
    Graph_FlowNetWorks():num_vertex(0){};
    Graph_FlowNetWorks(int n);
    void AddEdge(int from, int to, int capacity);

    void FordFulkerson(int source, int termination, ofstream &ofs);
    bool BFSfindExistingPath(std::vector<std::vector<int>> graphResidual, 
                             int *predecessor, int source, int termination);
    int MinCapacity(std::vector<std::vector<int>> graphResidual, 
                    int *predecessor, int termination);

};
Graph_FlowNetWorks::Graph_FlowNetWorks(int n):num_vertex(n){
    // constructor
    AdjMatrix.resize(num_vertex);
    for (int i = 0; i < num_vertex; i++)
        AdjMatrix[i].resize(num_vertex);
}

bool Graph_FlowNetWorks::BFSfindExistingPath(std::vector<std::vector<int>> graph, 
                                             int *predecessor, int s, int t){
    int *visited = new int[num_vertex];
    for (int i = 0; i < num_vertex; i++){
        visited[i] = 0;     // 0 表示還沒有被找到
        predecessor[i] = -1;
    }

    std::queue<int> queue;
    // BFS 從 s 開始, 也可以規定s一律訂成vertex(0)
    queue.push(s);
    visited[s] = 1;
    while (!queue.empty()) {
        int exploring = queue.front();
        for (int j = 0; j < num_vertex; j++) {
            if (graph[exploring][j]!=0 && visited[j]==0) {
                queue.push(j);
                visited[j] = 1;
                predecessor[j] = exploring;
            }
        }
        queue.pop();
    }
    return (visited[t] == 1);   // 若t有被visited, 表示有path從s到t
                                // 也可以用 if (predecessor[t] != -1) 判斷
}

int Graph_FlowNetWorks::MinCapacity(std::vector<std::vector<int>> graph, 
                                    int *predecessor, int t){
    int min = INT32_MAX;      // 確保min會更新, 假設graph上的capacity都小於100

    // 用predecessor[idx] 和 idx 表示一條edge
    // 找到在從s到t的path上, capacity最小的值, 存入min
    for (int idx = t; predecessor[idx] != -1; idx = predecessor[idx]){
        if (graph[predecessor[idx]][idx]!=0 && graph[predecessor[idx]][idx] < min) {
            min = graph[predecessor[idx]][idx];
        }
    }
    return min;
}

void Graph_FlowNetWorks::FordFulkerson(int source, int termination, ofstream &ofs){

    // residual networks的初始狀態等於AdjMatrix, 見圖五(a)
    std::vector<std::vector<int>> graphResidual(AdjMatrix);    
    int maxflow = 0;                                           
    int *predecessor = new int[num_vertex];

    // BFS finds augmenting path,
    while (BFSfindExistingPath(graphResidual, predecessor, source, termination)) {
        int mincapacity = MinCapacity(graphResidual, predecessor, termination);
        maxflow = maxflow + mincapacity;
        for (int Y = termination; Y != source; Y = predecessor[Y]){
            // 更新 residual graph
            int X = predecessor[Y];
            graphResidual[X][Y] -= mincapacity;
            graphResidual[Y][X] += mincapacity;
        }
    }
    ofs<<maxflow;
    //std::cout << "Possible Maximum Flow: " << maxflow << std::endl;
}
void Graph_FlowNetWorks::AddEdge(int from, int to, int capacity){

    AdjMatrix[from][to] = capacity;
}

int main(int argc, char *argv[]){
    ifstream file (argv[1]);
    ofstream ofs1 (argv[2], std::ofstream::out | std::ofstream::trunc);
    ofstream ofs2 (argv[3], std::ofstream::out | std::ofstream::trunc);
    string tmp1, tmp2;
    vector< vector<int> > matrix;
    vector <int> supermarket, path;
    int type, people, super, num, TYid, SPid, dis, from, to, cap;
    if(!file.is_open()){
		cout<<"Failed to open file"<<endl;	
	} else {
        file>>tmp1>>tmp2;
        file>>tmp1>>tmp2;
        file>>tmp1>>type;
        file>>tmp1>>people;
        matrix.resize(people,vector<int>(type+1));
        for(int i=0;i<people;i++){
            for(int j=0;j<type+1;j++){
                matrix[i][j] = 0;
            }
        }
        for(int i=1;i<people;i++){
            file>>num;
            for(int j=0;j<num;j++){
                file>>TYid;
                matrix[i][TYid] ++;
            }
        }
        file>>tmp1>>super;
        supermarket.resize(super+1);
        for(int i=1;i<super+1;i++){
            file>>tmp1>>SPid;
            supermarket[i] = SPid;
        }
    }

    Graph_SP market(super+2);
    while(file>>tmp1>>tmp2>>dis){
        if(tmp1!="s"&&tmp1!="t"){
            to = stoi(tmp1.substr(1));
        }
        else if(tmp1=="s"){
            to = 0;
        }
        else if(tmp1=="t"){
            to = super+1;
        }
        if(tmp2!="s"&&tmp2!="t"){
            from = stoi(tmp2.substr(1));           
        }
        else if(tmp2=="s"){
            from = 0;
        }
        else if(tmp2=="t"){
            from = super+1;
        }
        market.AddEdge(from, to, dis);
    }
    int index = super+1;
    path = market.Dijkstra(0, ofs1);
    while(path[index]!=-1){
        TYid = supermarket[path[index]];
        matrix[0][TYid]++;
        index = path[index];
    }
    // for(int i=0;i<people;i++){
    //     for(int j=1;j<=type;j++)
    //         cout<<matrix[i][j]<<" ";
    //     cout<<endl;
    // }
        
    Graph_FlowNetWorks exchange(people*type+2);
    int destin = people*type+1;
    for(int i=1;i<=type;i++){
        cap = matrix[0][i];
        exchange.AddEdge(0, i, cap);
        exchange.AddEdge(i, destin, 1);
        for(int j=1;j<=people-1;j++){
            exchange.AddEdge(i, i+type*j, 1);
        }
    }
    vector <int> duplicate;
    vector <int> none;
    for(int i=1;i<=people-1;i++){
        for(int j=1;j<=type;j++){
            if(matrix[i][j]==0){
                none.push_back(j);
            }
            else if(matrix[i][j]>=2){
                duplicate.push_back(j);
            }
        }
        for(int m=0;m<duplicate.size();m++){           
            for(int n=0;n<none.size();n++){
                exchange.AddEdge(none[n]+i*type, duplicate[m], 1);
            }
        }
        none.clear();
        duplicate.clear();
    }
    exchange.FordFulkerson(0, destin, ofs2);
    file.close();
    ofs1.close();
    ofs2.close();
    return 0;
}