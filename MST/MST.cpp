#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>   
#include <iomanip>
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
        std::cout << "new key is not smaller than current key\n";
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

void PrintArray(std::vector<int> array){
    for (int i = 0; i < array.size(); i++){
        cout << std::setw(4) << i;
    }
    std::cout << endl;
    for (int i = 0; i < array.size(); i++){
        cout << std::setw(4) << array[i];
    }
    cout << endl << endl;
}

int main(int argc, char *argv[]) {
    int num, x, y, dis;
    int cost = 0;
    vector<int> predecessor, distance;
    vector<bool> visited;
    vector< vector<int> > matrix;
    vector< pair<int, int> > coords;
	ifstream file (argv[1]);
    ofstream ofs (argv[2], std::ofstream::out | std::ofstream::trunc);

	if(!file.is_open()){
		cout<<"Failed to open file"<<endl;	
	} else {
        file>>num;
        predecessor.resize(num);
        distance.resize(num);
        matrix.resize(num,vector<int>(num));
        visited.resize(num, false);
        for(int i=0;i<num;i++){
            file>>x>>y;
            coords.push_back(make_pair(x,y));
        }
        
        for(int m=0;m<num;m++){
            for(int n=0;n<num;n++){
                if(m!=n){
                    dis = abs(coords[m].first - coords[n].first) + abs(coords[m].second - coords[n].second);
                    matrix[m][n] = dis;
                }
                else matrix[m][n] = 0;
            }
        }

        
        while(file>>x>>y){
            matrix[x][y] = 0;
            matrix[y][x] = 0;
        }

        for (int i = 0; i < num; i++) {
            distance[i] = INT32_MAX;
            predecessor[i] = -1;
        }
        distance[0] = 0;
        BinaryHeap minQueue(num);
        minQueue.BuildMinHeap(distance); 

        while(!minQueue.IsHeapEmpty()){
            int u = minQueue.ExtractMin();
            visited[u] = true;
            for(int v=0;v<num;v++){
                if(visited[v]==false && matrix[u][v]<distance[v]){
                    distance[v]=matrix[u][v];
                    predecessor[v]=u;
                    minQueue.DecreaseKey(v, distance[v]);
                    
                }
            }
        }
        // cout << "print predecessor[]:\n";
        // PrintArray(predecessor);
        // cout << "print distance[]:\n";
        // PrintArray(distance);
        for(int i=0;i<num;i++) {
            cost+=distance[i];         
        }     
        ofs << cost << endl;
        for(int i=0;i<num;i++) {
            if(distance[i]!=0){
                ofs << predecessor[i] << " " << i << endl;
            }           
        }     
        file.close();
        ofs.close();
        return 0;
    }
}