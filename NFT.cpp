#include "Blockchain.h"

int main(){
    //Load blockchain file
    Blockchain chain;
    chain.loadChain("chainHistory.txt");
    //Allow edit to be made via menu
    bool quit = false;
    string input;
    string reciever;
    string nft;
    string sender;
    while (!quit){
        cout << "----------------------BLOCK CHAIN MENU---------------------" << endl;
        cout << "[1] Do a transaction" << endl;
        cout << "[2] Mint a new NFT" << endl;
        cout << "[3] Create a new user" << endl;
        getline(cin, input);
        if(input == "1"){
            cout << "Enter sending user id" << endl;
            getline(cin, sender);
            cout << "Enter reciving user id" << endl;
            getline(cin, reciever);
            cout << "Enter nft id" << endl;
            getline(cin, nft);
            chain.doTranaction(chain.stringToUser(sender), chain.stringToUser(reciever), chain.stringToNFT(nft));
        }
        else if(input == "2"){
            cout << "Enter minting user id" << endl;
            getline(cin, reciever);
            cout << "Enter nft file path" << endl;
            getline(cin, nft);
            chain.mintNFT(chain.stringToUser(reciever), nft);
        }
        else if(input == "3"){
            cout << "Enter user name" << endl;
            getline(cin, sender);
            chain.createUser(sender);
        }
        else{
            quit = true;
            chain.saveChain("chainHistory.txt");
            return 0;
        }
    }
    return 0;
}