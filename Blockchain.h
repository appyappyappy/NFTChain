#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

struct NFT{ 
    string filePath;
    int idNumber; //zero indexed
    bool operator == (const NFT &n){
        if (filePath == n.filePath && idNumber == n.idNumber) {return true;}
        return false;
    }
};

struct User{
    string name;
    int idNumber; //zero indexed
    bool operator == (const User &u){
        if (name == u.name && idNumber == u.idNumber) {return true;}
        return false;
    }
};

struct TransactionBlock{
    User sender, reciever;
    NFT item;
    TransactionBlock* nextBlock;
    TransactionBlock* prevBlock;
    int blockID;
};

class Blockchain{
    private:
        TransactionBlock* chainHead;
        vector<User> users;
        vector<NFT> nfts;
    public:
        Blockchain();
        User stringToUser(string);
        NFT stringToNFT(string);

        void loadChain(string fileName);
        void saveChain(string fileName);

        void doTranaction(User, User, NFT);
        void mintNFT(User, string);
        void createUser(string);
};