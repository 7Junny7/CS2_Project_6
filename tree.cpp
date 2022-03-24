#include <iostream>
#include <string>
#include <queue>
#include <vector>
using namespace std;

typedef string ItemType;

struct WordNode {
    ItemType m_data;
    WordNode *m_left;
    WordNode *m_right;
    // You may add additional data members and member functions in WordNode
    int count = 0;
};

class WordTree {
private:
    WordNode *root;
    bool Insert(WordNode*& nodeptr, ItemType item);
    void Destroy(WordNode*& nodeptr);
    void PrintTree(WordNode* nodeptr);
    int getLevel(WordNode* nodeptr) const;
    bool del(WordNode* nodeptr, ItemType v);
public:
    // default constructor
    WordTree() : root(nullptr) {
//        root = new WordNode{NULL, nullptr, nullptr};
    };
    // copy constructor
    WordTree(const WordTree& rhs);
    // assignment operator
    const WordTree& operator=(const WordTree& rhs);
    // Inserts v into the WordTree
    void add(ItemType v);
    // Returns the number of distince words / nodes
    int distinctWords() const;
    // Returns the total number of words inserted, including duplicate
    // values
    int totalWords() const;
    // Prints the LinkedList
    friend ostream& operator<<(ostream &out, const WordTree& rhs);
    // Removes the word from the tree. Returns true if the word is
    // in the tree, false if it is not.
    bool remove(ItemType v);
    // Destroys all the dynamically allocated memory
    // in the tree.
    ~WordTree();
    void Print();
};

WordTree::WordTree(const WordTree &rhs) {
    // copy constructor
    queue<WordNode*> q;
    WordNode* guess;
    q.push(rhs.root);
    while (!q.empty()) {
        guess = q.front();
        q.pop();
        if (guess == nullptr) {
            continue;
        }

        root->m_data = guess->m_data;
        root->m_left = guess->m_left;
        root->m_right = guess->m_right;
        root->count = guess->count;

        q.push(guess->m_left);
        q.push(guess->m_right);
    }
}

const WordTree &WordTree::operator=(const WordTree &rhs) {
    // assignment operator
    // Need deleting

    Destroy(root);
    queue<WordNode*> q;
    WordNode* guess;
    q.push(rhs.root);
    while (!q.empty()){
        guess = q.front();
        q.pop();
        if (guess == nullptr){
            continue;
        }

        root->m_data = guess->m_data;
        root->m_left = guess->m_left;
        root->m_right = guess->m_right;
        root->count = guess->count;

        q.push(guess->m_left);
        q.push(guess->m_right);
    }
}

void WordTree::add(ItemType v) {
    // Inserts v into the WordTree

    bool check = Insert(root,v);
    if (!check){
        queue<WordNode*> q;
        WordNode* guess;
        q.push(root);
        while (!q.empty()){
            guess = q.front();
            q.pop();
            if (guess == nullptr){
                continue;
            }
            if (guess->m_data == v){
                guess->count++;
            }
            q.push(guess->m_left);
            q.push(guess->m_right);
        }
    }
}
bool WordTree::Insert(WordNode*& nodeptr, ItemType item) {

    if (nodeptr == nullptr){
        nodeptr = new WordNode{item, nullptr, nullptr,1};
        return true;
    }
    if (item < nodeptr->m_data){
        return Insert(nodeptr->m_left,item);
    }else if (item > nodeptr->m_data){
        return Insert(nodeptr->m_right,item);
    }
    return false;
}

int WordTree::getLevel(WordNode *nodeptr) const {
    if (nodeptr == nullptr){
        return 0;
    }
    return getLevel(nodeptr->m_left) + getLevel(nodeptr->m_right) + 1;
}
int WordTree::distinctWords() const {
    // Returns the number of distince words / nodes
    int count = getLevel(root);
    return count;

    //by queue
//    queue<WordNode*> q;
//    WordNode* guess;
//    int count = 0;
//    q.push(root);
//    while (!q.empty()) {
//        guess = q.front();
//        q.pop();
//        if (guess == nullptr) {
//            continue;
//        }
//        count++;
//        q.push(guess->m_left);
//        q.push(guess->m_right);
//    }
//    return count;

}

int WordTree::totalWords() const {
    // Returns the total number of words inserted, including duplicate
    // values
    queue<WordNode*> q;
    WordNode* guess;
    int count = 0;
    q.push(root);
    while (!q.empty()) {
        guess = q.front();
        q.pop();
        if (guess == nullptr) {
            continue;
        }
        count += guess->count;
        q.push(guess->m_left);
        q.push(guess->m_right);
    }
    return count;
}

bool compare_item(WordNode *a, WordNode *b){
    return a->m_data < b->m_data;
}
ostream& operator<<(ostream &out, const WordTree& rhs){
    // Prints the LinkedList
    if (rhs.root != nullptr){

        queue<WordNode*> q;
        WordNode* guess;
        vector<WordNode*> results;
        q.push(rhs.root);
        while (!q.empty()) {
            guess = q.front();
            q.pop();
            if (guess == nullptr) {
                continue;
            }
            results.push_back(guess);
            q.push(guess->m_left);
            q.push(guess->m_right);
        }
        sort(results.begin(), results.end(), compare_item);
        for (int i=0; i<results.size(); i++){
            out<<results.at(i)->m_data<<" "<<results.at(i)->count<<endl;
        }
        return out;
    }
}

void WordTree::Destroy(WordNode*& nodeptr) {
    if (nodeptr != nullptr){
        Destroy(nodeptr->m_right);
        Destroy(nodeptr->m_left);
        delete nodeptr;
    }
}
WordTree::~WordTree() {
    // Destroys all the dynamically allocated memory
    // in the tree.
    Destroy(root);
}

//For extra credit
bool WordTree::del(WordNode *nodeptr, ItemType v) {
    if (nodeptr == nullptr){
        Destroy(nodeptr);
        return true;
    }
    if (nodeptr->m_data == v){
        if (nodeptr->count > 1){
            nodeptr->count--;
            return true;
        }else{
            if (nodeptr->m_left == nullptr && nodeptr->m_right == nullptr){
                delete nodeptr;
                return true;
            }else if (nodeptr->m_left != nullptr && nodeptr->m_right == nullptr){
                WordNode* temp_del = nodeptr;
                nodeptr = nodeptr->m_left;
                delete temp_del;
                return true;
            }else if (nodeptr->m_left == nullptr && nodeptr->m_right != nullptr){
                WordNode* temp_del = nodeptr;
                nodeptr = nodeptr->m_right;
                delete temp_del;
                return true;
            }else if (nodeptr->m_left != nullptr && nodeptr->m_right != nullptr){
                queue<WordNode*> q_sub;
                WordNode* temp_pa = nodeptr;
                WordNode* temp_sub_result = nodeptr;
                q_sub.push(nodeptr);
                while (!q_sub.empty()){
                    WordNode* temp_sub;
                    temp_sub = q_sub.front();
                    q_sub.pop();
                    if (temp_sub == nullptr){
                        continue;
                    }
                    if (temp_sub_result->m_data < temp_sub->m_data){
                        temp_pa = temp_sub_result;
                        temp_sub_result = temp_sub;
                    }
                    q_sub.push(temp_sub->m_left);
                    q_sub.push(temp_sub->m_right);
                }
                nodeptr->m_data = temp_sub_result->m_data;
                nodeptr->count = temp_sub_result->count;
                if (temp_pa->m_right->m_data == temp_sub_result->m_data){
                    temp_pa->m_right = nullptr;
                }
                if (temp_pa->m_left->m_data == temp_sub_result->m_data){
                    temp_pa->m_left = nullptr;
                }
                delete temp_sub_result;
                return true;
            }
        }
    }
    del(nodeptr->m_left, v);
    del(nodeptr->m_right, v);

    return false;
}
bool WordTree::remove(ItemType v) {
    if (root != nullptr){
        return del(root, v);
    }
}

void WordTree::PrintTree(WordNode *nodeptr) {
    if (nodeptr == nullptr){
        return;
    }
    PrintTree(nodeptr->m_left);
    cout<<nodeptr->m_data<<" ";
    PrintTree(nodeptr->m_right);
}

void WordTree::Print() {
    PrintTree(root);
    cout<<endl;
}

int main() {
    WordTree t;

    //1
    t.add("Skyler");
    t.add("Walter");
    t.add("Walter");
    t.add("Walter");
    assert(t.distinctWords() == 2);
    assert(t.totalWords() == 4);

    //2
//    t.add("Skyler");
//    t.add("Walter");
//    t.add("Walter");
//    t.add("Hank");
//    t.add("Gus");
//    t.add("Walter");
//    t.add("Gus");
//    cout << t;

    //3
//    t.add("Skyler");
//    t.add("Walter");
//    t.add("Walter");
//    t.add("Hank");
//    t.add("Hank");
//    t.add("Jessie");
//    t.add("Gus");
//
//    cout << t;
//    t.remove("Hank");
//    cout << "Removing Hank..." << endl;
//    cout << t;
//    t.remove("Hank");
//    cout << "Removing Hank..." << endl;
//    cout << t;

    return 0;
}
