#include<bits/stdc++.h>
using namespace std;

struct block {
    bool valid = false;
    long long tag = 0;
    long long lru = 0; 
    bool dirty=false;
};


//All data here is in Bytes
const int L1_sizes = 32*1024;   //will update like in JS if i make some website here 
const int L1_block = 64;    
const int L1_assoc = 4;           


const int L2_sizes = 512*1024; 
const int L2_block = 64;    
const int L2_assoc = 8;           


const int L3_sizes = 2*1024*1024; 
const int L3_block = 64;    
const int L3_assoc = 16;           


long long timer=0;

class cache{
public:
    long long sizes,blocksize,assoc,lines,sets;
    vector<vector<block>> data;
    cache *parent=nullptr;
    long long hits=0,misses=0,reads=0,writes=0;
    cache(long long sizes,long long blocksize,long long assoc){
this->sizes=sizes;
this->blocksize=blocksize;
this->assoc=assoc;
this->lines=sizes/blocksize;
 this->sets  =this->lines/assoc;
 if(this->sets<=0) this->sets=1;    //safeguard for 0   . can be removed  !!! 

 data.resize(sets, vector<block>(assoc));

 
    }


    long long search(long long address, bool write ){
         long long index = (address / blocksize) % sets;
        long long tag = address / (blocksize * sets);
for(int i =0 ; i<assoc;i++){

    if(data[index][i].valid && data[index][i].tag==tag ){
       
       
if(write) data[index][i].dirty=true;
    data[index][i].lru = timer;
       return i;

    }
}

return -1;
    }

    void insert(long long address, bool write  ){
        long long index = (address / blocksize) % sets;
        long long tag = address / (blocksize * sets);

int lruindex = 0;
long long mini = LLONG_MAX;
int empindex = -1;  //if empty cache then it will be inserted there only as there is no hit 


for(long long i =0;i<assoc;i++){
if(!data[index][i].valid && empindex == -1 ){
    empindex=i;
    
}
if(data[index][i].valid && mini>data[index][i].lru){
    mini= data[index][i].lru;
    lruindex=i;
}
}
int replace = lruindex;
if(empindex!=-1) replace= empindex;

else{
    //code for evicting in case of dirty bit validity .
if(data[index][lruindex].valid && data[index][lruindex].dirty){
    long long finaladdress = 1LL*(data[index][lruindex].tag*sets+index) *blocksize;
    if (parent != nullptr)
    parent->insert(finaladdress, true);
}
}
data[index][replace].lru=timer;
data[index][replace].tag=tag;
data[index][replace].valid=true;
data[index][replace].dirty = write;

    }
 


};


void access(cache &L1, cache &L2, cache &L3 ,long long address, bool write){
if(L1.search(address,write )!=-1 ){
    L1.hits++;
return;
}
else if(L2.search(address,write)!=-1){
    L2.hits++;
    L1.misses++;
    L1.insert(address,write);

}
else if(L3.search(address,write)!=-1){
    L1.misses++;
    L2.misses++;
    L3.hits++;
    L1.insert(address,write);
    L2.insert(address,write);

}
else{
    L1.insert(address,write);
    L2.insert(address,write);
    L3.insert(address,write);
    L1.misses++;
    L2.misses++;
    L3.misses++;
    
}

}



int main(int argc, char* argv[]){
    // Default values if no arguments provided
    long long b_size = 64;
    long long l1_s = 128, l1_a = 4;
    long long l2_s = 1024, l2_a = 8;
    long long l3_s = 2048, l3_a = 16;

    if(argc == 8) {
        b_size = stoll(argv[1]);
        l1_s = stoll(argv[2]); l1_a = stoll(argv[3]);
        l2_s = stoll(argv[4]); l2_a = stoll(argv[5]);
        l3_s = stoll(argv[6]); l3_a = stoll(argv[7]);
    }

    // Calculating total sizes to pass to your constructor
    cache L1(l1_s * b_size * l1_a, b_size, l1_a);
    cache L2(l2_s * b_size * l2_a, b_size, l2_a);
    cache L3(l3_s * b_size * l3_a, b_size, l3_a);
 
    L1.parent = &L2;
    L2.parent = &L3;
    L3.parent = nullptr;

    ifstream trace_file("trace.out");
    if (!trace_file.is_open()) {
        cerr << "error: couldn't open trace.out" << endl;
        return -1;
    }

    string mode, hexa;
    while (trace_file >> mode >> hexa) {
        timer++;
        bool writesfinal = (mode == "W");
        try {
            long long address = stoull(hexa, nullptr, 16);
            access(L1, L2, L3, address, writesfinal);
        } catch (...) { continue; }
    }

    double percL1 = ((double)L1.hits * 100.0) / (L1.hits + L1.misses);
    double percL2 = ((double)L2.hits * 100.0) / (L2.hits + L2.misses);
    double percL3 = ((double)L3.hits * 100.0) / (L3.hits + L3.misses);

    cout << "L1 hits: " << L1.hits << " misses: " << L1.misses << "\n";
    cout << "L2 hits: " << L2.hits << " misses: " << L2.misses << "\n";
    cout << "L3 hits: " << L3.hits << " misses: " << L3.misses << "\n";
    cout << fixed << setprecision(2) << percL1 << "% " << percL2 << "% " << percL3 << "%" << endl;

    return 0;
}

