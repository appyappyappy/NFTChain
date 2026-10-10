#include "Blockchain.h"
#include <fstream>
using namespace std;

/**
 * @brief Construct a new Blockchain:: Blockchain object
 * 
 */
Blockchain::Blockchain(){
    chainHead = nullptr;
}

/**
 * @brief converts a user id (str) to a user object
 * 
 * @param userID 
 * @return User 
 */
User Blockchain::stringToUser(string userID){
    int userIDInt = stoi(userID);
    if(userIDInt >= users.size()){
        cout << "Error bad User" << endl;
        User errorNFT = {"", -999};
        return errorNFT;
    }
    return users.at(userIDInt);
}

/**
 * @brief converts a nft id (str) to a nft object
 * 
 * @param nftID 
 * @return NFT 
 */
NFT Blockchain::stringToNFT(string nftID){
    int nftIDInt = stoi(nftID);
    if(nftIDInt >= nfts.size()){
        cout << "Error bad NFT" << endl;
        NFT errorNFT = {"", -999};
        return errorNFT;
    }
    return nfts.at(nftIDInt);
}

/**
 * @brief Creates the block id with a very simple hash
 * 
 * @param block - The transaction to be hashed
 * @return int - the hash
 */
int hashBlock(TransactionBlock* block){
    int ans = 0;
    if(block->prevBlock != nullptr){
        ans += block->prevBlock->blockID;
    }
    //Sender
    ans += block->sender.idNumber;
    ans += block->sender.name[0];
    //Reciever
    ans += block->reciever.idNumber;
    ans %= block->reciever.name[0];
    //NFT
    ans += block->item.idNumber;
    ans += block->item.filePath[0];
    
    return ans;
}

/**
 * @brief adds a transaction to the chain
 * 
 * @param sender the User sending the NFT
 * @param reciever the User getting the NFT
 * @param apeImage the NFT being traded
 */
void Blockchain::doTranaction(User sender, User reciever, NFT apeImage){
    //check if sender owns the nft in question
    TransactionBlock* curr = chainHead;
    while(curr != nullptr){
        if(curr->item == apeImage && curr->reciever == sender){
            break;
        }
        else if(curr->item == apeImage){
            cout << "Error, sender doesn't own the NFT" << endl;
            return;
        }
        curr = curr->nextBlock;
    }
    //create new transaction block, and append to chain
    TransactionBlock* newBlock = new TransactionBlock{sender, reciever, apeImage, nullptr, 0};
    if(chainHead != nullptr){
        newBlock->prevBlock = chainHead;
        chainHead->nextBlock = newBlock;
    }
    newBlock->blockID = hashBlock(newBlock);
    chainHead = newBlock;
    cout << "Done with transaction" << endl;
}

/**
 * @brief Adds a transaction to the chain which creates the NFT
 * 
 * @param minter - The User minting the NFT
 * @param apeImage - The file path to the smart contract
 */
void Blockchain::mintNFT(User minter, string filepath){

    NFT newNft = {filepath, int(nfts.size())};
    cout << "NFT created: " << filepath << " with ID: " << int(nfts.size()) << endl;
    nfts.push_back(newNft);

    User emptyUser = {"", -999};
    TransactionBlock* newBlock = new TransactionBlock{emptyUser, minter, newNft, nullptr, 0};
    if(chainHead != nullptr){
        newBlock->prevBlock = chainHead;
        chainHead->nextBlock = newBlock;
    }
    newBlock->blockID = hashBlock(newBlock);
    chainHead = newBlock;
}

/**
 * @brief Creates a new User object
 * 
 * @param name - The user's name
 */
void Blockchain::createUser(string name){
    User newUser = {name, int(users.size())};
    cout << "User created: " << name << " with ID: " << int(users.size()) << endl;
    users.push_back(newUser);
}

/**
 * @brief Loads in a chain object from the file. If the hash is wrong, the program will end.
 * 
 * @param chainFile - The file path to the saved chain
 */
void Blockchain::loadChain(string chainFile){
    ifstream chainFileStream(chainFile);
    string line, idSenderString, idRecieverString, idNFTString, dollarAmountString, hash;

    if (chainFileStream.is_open()) {
        //Toss the first 2 lines (title line, and hash)
        getline(chainFileStream,line);
        getline(chainFileStream,hash);

        while (getline(chainFileStream,line)) {
            // A transaction is saved as 1 \n idSender \n idReciever \n idNFT
           if(line[0] == '1'){
            getline(chainFileStream,idSenderString);
            getline(chainFileStream,idRecieverString);
            getline(chainFileStream,idNFTString);
            doTranaction(stringToUser(idSenderString), stringToUser(idRecieverString), stringToNFT(idNFTString));
           }
           // Minting an NFT is saved as 2 \n minterID \n nftFilePath
           else if(line[0] == '2'){
            getline(chainFileStream, idRecieverString);
            getline(chainFileStream, idNFTString);
            mintNFT(stringToUser(idRecieverString), idNFTString);
           }
           //Creating a user is saved as 3 \n name
           else if(line[0] == '3'){
            getline(chainFileStream, idSenderString);
            createUser(idSenderString);
           }
        }
    chainFileStream.close();
    if(chainHead != nullptr && chainHead->blockID != stoi(hash)){
        cout << "BAD HASH. THE CHAIN HISTORY FILE HAS BEEN TAMPERED WITH" << endl;
        exit(0);
    }
  }
  else{
     cout << "Unable to open file"; 
  }
}

/**
 * @brief Saves the chain
 * 
 * @param chainFile 
 */
void Blockchain::saveChain(string chainFile){
    ofstream chainFileStream(chainFile);
    string line, idSenderString, idRecieverString, idNFTString, dollarAmountString;
    TransactionBlock* curr = chainHead;

    if (chainFileStream.is_open()) {
        chainFileStream << "--- State of the chain ---" << endl;
        chainFileStream << curr->blockID << endl;
        
        //Navigate to end of chain (which is the furthest back in time)
        while(curr->prevBlock != nullptr){
            curr = curr->prevBlock;
        }

        for(int i = 0; i < users.size(); i++){
            //Creating a user is saved as 3 \n name
            chainFileStream << "3\n" << users.at(i).name << endl;
        }

        while (curr->nextBlock != nullptr) {
            // A transaction is saved as 1 \n idSender \n idReciever \n idNFT
            if(curr->sender.idNumber >= 0){
                chainFileStream << "1\n" << curr->sender.idNumber << "\n" << curr->reciever.idNumber << "\n" << curr->item.idNumber << endl;
            }
            // Minting an NFT is saved as 2 \n minterID \n nftFilePath
            else{
                chainFileStream << "2\n" << curr->item.idNumber << "\n" << curr->item.filePath << endl;
            }
           curr = curr->nextBlock;
        }

        // A transaction is saved as 1 \n idSender \n idReciever \n idNFT
        if(curr->sender.idNumber >= 0){
            chainFileStream << "1\n" << curr->sender.idNumber << "\n" << curr->reciever.idNumber << "\n" << curr->item.idNumber << endl;
        }
        // Minting an NFT is saved as 2 \n minterID \n nftFilePath
        else{
            chainFileStream << "2\n" << curr->item.idNumber << "\n" << curr->item.filePath << endl;
        }
        chainFileStream.close();
    }
    else{
        cout << "Unable to open file"; 
    }
}