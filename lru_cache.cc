#include <iostream>          
#include <algorithm>         
#include <unordered_map> 
#include <exception>
#include <stdexcept>     
using namespace std;

struct InvalidKeyException : public exception
{
  const char * what () const throw ()
  {
    return "cache miss: key does not exist in cache";
  }
};

struct Node {
    string key;
    int val;

    Node *prev;
    Node *next;
};

class LRUCache {

    Node *head;
    Node *tail;

    unordered_map<string, Node*> lru_map;
    int cap;

    void addToHead(Node *n) {
        n->prev = head;
        n->next = head->next;

        n->prev->next = n;
        n->next->prev = n;
    }

    void removeNode(Node *n) {
        Node *nprev = n->prev;
        Node *nnext = n->next;

        nprev->next = nnext;
        nnext->prev = nprev;
    }

    void moveToHead(Node *n) {
        removeNode(n);
        addToHead(n);
    }

    void removeFromBack() {
        // node to remove is before tail
        Node *n = tail->prev;
        removeNode(n);
    }

    public:
    LRUCache(int _cap) {
        cap = _cap;
        head = new Node();
        tail = new Node();

        head->next = tail;
        tail->prev = head;
    }

    int get(string key) {
        auto it = lru_map.find(key);
        // if key is in the map
        if(it != lru_map.end()) {
            Node *n = it->second;
            //move it to the front denoting the access
            moveToHead(n);
            return it->second->val;
        } else {
            throw InvalidKeyException();
        }
    }

    void insert(string key, int val) {
        auto it = lru_map.find(key);
        // if key is in the map
        if(it != lru_map.end()) {
            // if this key exists then update the val for it
            it->second->val = val;
            // make it most recently accessed
            Node *n = it->second;
            moveToHead(n);
        } else {
            // key not in map
            Node *n = new Node();
            n->key = key;
            n->val = val;

            //insert into map
            lru_map[key] = n;
            //insert in list
            addToHead(n);
            if(lru_map.size() > cap) {
                // remove from map as well
                lru_map.erase(tail->prev->key);
                removeFromBack();
            }
        }
    }
};

bool lru_test() {
   {
       cout << "Running test 1... " << endl;
       bool except = false;
       int got;
       try {
            LRUCache lru(2);
            lru.insert("1", 1);
            lru.insert("4", 4);
            got = lru.get("3");
        } catch(class exception& e) {
            except = true;
            cout << e.what() << endl;
        }
        if(except == false) return false;
   };

   {
       cout << "Running test 2... " << endl;
       bool except = false;
       int got;
       try {
            LRUCache lru(2);
            lru.insert("1", 1);
            lru.insert("4", 4);
            got = lru.get("1");
        } catch(class exception& e) {
            except = true;
            cout << e.what() << endl;
        }
        if(except == true || got != 1) return false;
   };
   return true;
}

int main() {
    // int got;
    // try {
    //     LRUCache lru(2);
    //     lru.insert("1", 1);
    //     lru.insert("4", 4);
    //     got = lru.get("3");
    //     cout << got << endl;
    // } catch(exception& e) {
    //     cout << e.what() << endl;
    // }

    if(lru_test() == false) return -1;
    cout << "Tests Passed" << endl;

    return 0; 
}