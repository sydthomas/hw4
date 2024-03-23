#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here

    // check balance ( checks to see non of the balances exceeed x  > |2| )
    bool checkBalance(AVLNode<Key,Value>* root) const;  /* previous parameter:  'const AVLNode<Key,Value>* root'   */

    //find node with balance not bewteen -1 to 1 
    AVLNode<Key,Value>* nodeBalFind(AVLNode<Key,Value>* node_) const;  //don't need parameters

    // left rotation
    void leftRotation(AVLNode<Key,Value>* n1);

    // right rotation
    void rightRotation(AVLNode<Key,Value>* n1);

    //
    void removeFix(AVLNode<Key,Value>* n, int8_t diff);
    void insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);


    //performs balancing nodes 
    //void balanceNodes(AVLNode<Key,Value>* newNode);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    //check to see if empty 
    
    if(this->root_ == NULL){
        this->root_  = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        static_cast<AVLNode<Key, Value>*>(this->root_ )->setBalance(0);
        return; //maybe 
    }
    else{  //find spot for inserted node  
        AVLNode<Key,Value>* currNode = static_cast<AVLNode<Key, Value>*>(this->root_ );
        bool condition = false;
        //std::cerr << "beginning of insert function" << std::endl;
        //this->print();
        while(condition == false){
            if(new_item.first < currNode->getKey()){
                if(currNode->getLeft() == NULL){
                    currNode->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, currNode));
                    currNode->getLeft()->setBalance(0);
                    //std::cout << currNode->getLeft()->getKey() << std::endl; 
                    if(currNode->getBalance() == 1 || currNode->getBalance() == -1){
                        currNode->setBalance(0);
                        return;
                    }
                    else if(currNode->getBalance() == 0){
                        currNode->setBalance(-1);
                        //insert_fix(currNode, currNode->getLeft()); 
                    }
                    //else if(currNode->getBalance() )
                    // std:: cout << "parent:" << currNode->getKey() << std::endl;
                    // std:: cout << "newly inserted" << currNode->getLeft()->getKey() << std::endl;
                    // std::cerr << "Before insert_fix --> left child" << std::endl;
                    insert_fix(currNode, currNode->getLeft());
                    //this->print();
                    condition = true;
                    //break;
                }
                else{
                    currNode = currNode->getLeft();
                }
            }
            else if(currNode->getKey() < new_item.first){
                if(currNode->getRight() == NULL){
                    currNode->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, currNode));
                    currNode->getRight()->setBalance(0);
                    //std::cout << currNode->getRight()->getKey() << std::endl; 
                    if(currNode->getBalance() == -1 || currNode->getBalance() == 1){
                        currNode->setBalance(0);
                        return;
                    }
                    else if(currNode->getBalance() == 0){
                        currNode->setBalance(1);
                        //insert_fix(currNode, currNode->getRight());
                    }
                    // std:: cout << "parent:" << currNode->getKey() << std::endl;
                    // std:: cout<< "newly inserted:" << currNode->getRight()->getKey() << std::endl;
                    // std::cerr << "Before insert_fix --> right child" << std::endl;
                    insert_fix(currNode, currNode->getRight()); 
                    //this->print();
                    condition = true;
                    //break;
                }
                else{
                    currNode = currNode->getRight();
                }
            }
            else{
                currNode->setValue(new_item.second);
                condition = true;
                //break;
            }
        }
        // std::cerr << "endiing of insert function" << std::endl;
        // this->print();
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    if(static_cast<AVLNode<Key, Value>*>(this->internalFind(key)) == NULL){
        return;
    }

    AVLNode<Key,Value>* removeNode = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    
    //AVLNode<Key,Value>* current;

    //AVLNode<Key,Value>* childNode;
    int8_t diff = 0;

    if (removeNode->getLeft() != NULL && removeNode->getRight() != NULL){
        AVLNode<Key, Value>* predec = static_cast<AVLNode<Key, Value>*>(this->predecessor(removeNode));
        nodeSwap(removeNode, predec);
	}


    AVLNode<Key,Value>* parent_ = removeNode->getParent();
    if(parent_ != NULL){
        if(parent_->getLeft() == removeNode){
            diff = 1;
        }
        else{
            diff = -1;
        }
    }

    //int8_t diff = 0;
    
    if(removeNode->getLeft() == NULL && removeNode->getRight() == NULL){
        //AVLNode<Key,Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(removeNode));
        if(removeNode == this->root_){
            this->root_ = NULL;
        }
        else if(removeNode->getParent()->getLeft() == removeNode){
            removeNode->getParent()->setLeft(NULL);
        }
        else{
            removeNode->getParent()->setRight(NULL);
        }
    }
    else if(removeNode->getRight() == NULL){
        if(removeNode == this->root_){
            this->root_ = removeNode->getLeft();
            this->root_->setParent(NULL);
        }
        else if(removeNode->getParent()->getLeft() == removeNode){
            removeNode->getParent()->setLeft(removeNode->getLeft());
			removeNode->getLeft()->setParent(removeNode->getParent());
        }
        else{
            removeNode->getParent()->setRight(removeNode->getLeft());
			removeNode->getLeft()->setParent(removeNode->getParent());
        }
    }
    else if (removeNode->getLeft() == NULL) {
			if (removeNode == this->root_) {
				this->root_ = removeNode->getRight();
				this->root_->setParent(NULL);
			}
			else if (removeNode->getParent()->getLeft() == removeNode) {
				removeNode->getParent()->setLeft(removeNode->getRight());
				removeNode->getRight()->setParent(removeNode->getParent());
			}
			else {
				removeNode->getParent()->setRight(removeNode->getRight());
				removeNode->getRight()->setParent(removeNode->getParent());
			}
		}
		delete removeNode;
		removeFix(parent_, diff);

    


    //     if(removeNode->getLeft() != NULL){
    //         childNode = removeNode->getLeft();
    //     }
    //     else{
    //         childNode = removeNode->getRight();
    //     }
    //     if(parent_ != NULL){
    //         if(parent_->getLeft() == removeNode){
    //             parent_->setLeft(childNode);
    //             childNode->setParent(parent_);
    //             diff = 1;
    //         }
    //         else{
    //             parent_->setRight(childNode);
    //             childNode->setParent(parent_);
    //             diff = -1;
    //         }
    //         delete removeNode;
    //     }
    //     else{
    //         this->root_ = childNode;
    //         childNode->setParent(this->root_);
    //         delete removeNode;
    //     }
    // }
    // else{
    //     current = removeNode->getLeft();  // usually getRight()
    //     while(current->getRight() != NULL){
    //         current = current->getRight();
    //     }
    //     // if(current->getLeft() == NULL && current->getRight() == NULL){
    //     //     nodeSwap(removeNode, current);
    //     // }
    //     // else if(current->getLeft() == NULL || current->getRight() == NULL){
    //     //     nodeSwap(removeNode, current);
    //     // }
    //     // else{
    //     //     current = 
    //     // }
    //     nodeSwap(removeNode, current);

    //     if(removeNode->getLeft() != NULL){
    //         childNode = removeNode->getLeft();
    //     }
    //     else{
    //         childNode = removeNode->getRight();
    //     }

    //     if(parent_ != NULL){
    //         if(parent_->getLeft() == removeNode){
    //             parent_->setLeft(childNode);
    //             if(childNode != NULL){
    //                 childNode->setParent(parent_);
    //                 diff = 1;
    //             }
    //         }
    //         else{
    //             parent_->setRight(childNode);
    //             if(childNode != NULL){
    //                 childNode->setParent(parent_);
    //                 diff = -1;
    //             }
    //         }
    //         delete removeNode;
    //     }
    // }
    // removeFix(parent_, diff);


 

    //find Node that has correct key

    // while(current){
    //     if(current->getKey() == key){
    //         removeNode = current;
    //         break;
    //     }
    //     else{
    //         if(current->getKey() > key){
    //             current = current->getLeft();
    //         }
    //         else{
    //             current = current->getRight();
    //         }
    //     }
    //     removeNode = current;
    // }

// get parent node of removeNode
// s removeNode left or right child of parent
    // AVLNode<Key,Value>* parent_ = removeNode->getParent();
    // int8_t diff;
    // if(parent_ != NULL){
    //     if(parent_->getLeft() == removeNode){
    //         diff = 1;  
    //     }
    //     else{
    //         diff = -1;
    //     }
    // } 

    // if(removeNode != NULL){
    //     if(current->getLeft() != NULL){
    //         current = current->getLeft();
    //         while(current->getRight() != NULL){
    //             current = current->getRight();
    //         }
    //         nodeSwap(removeNode, current);
    //         delete removeNode; //ask abt this and ^^ line 
    //         removeNode = NULL;
    //     }
    //     else{
    //         nodeSwap(removeNode, current);
    //         delete removeNode; // I think thats it  
    //         removeNode = NULL;
    //     }
    // }
    
    // removeFix(parent_, diff);


    //     STEPS:
    //go to left subtree
    //find right most node (if there is one)
    // replace deleting node with that one 

    // check if tree is balanced 
    // if not perform balancing act 
    // thats it 

}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

/*       HELPER FUNCTIONS      */
template<class Key, class Value>
bool AVLTree<Key, Value>::checkBalance(AVLNode<Key,Value>* root) const
{
    AVLNode<Key,Value>* current = root;
    //bool is_itBalanced = false;
    while(current){
        if((current->getBalance() < -1) || (current->getBalance() > 1)){
            return false;
        }
        current = current->getParent();
    }
    return true;
}

template<class Key, class Value>   
AVLNode<Key,Value>* AVLTree<Key, Value>::nodeBalFind( AVLNode<Key,Value>* node_) const
{
    AVLNode<Key,Value>* tempNode = node_;
    while(tempNode->getBalance() > -2 && tempNode->getBalance() < 2){
        tempNode = tempNode->getParent();
    }
    return tempNode;
}

    // left rotation
template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation(AVLNode<Key,Value>* n1) 
{ // just imputing these nodes as pointers is enough to change them right

    AVLNode<Key,Value>* n2 = n1->getRight();

    AVLNode<Key,Value>* tempNode = n1->getParent();

    if(n2 != NULL){
        if(tempNode != NULL){
            if(tempNode->getLeft() == n1){
                tempNode->setLeft(n2);
            }
            else{
                tempNode->setRight(n2);
            }
            //tempNode->setLeft(n2);
            n2->setParent(tempNode);
            n1->setRight(n2->getLeft());
            if(n2->getLeft()){
                n2->getLeft()->setParent(n1);
            }
            n2->setLeft(n1);
            n1->setParent(n2);
            //n1->setRight(n2->setLeft());
        }
        else{
            this->root_ = n2;
            n2->setParent(NULL);
            n1->setRight(n2->getLeft());
            if(n2->getLeft()){
                n2->getLeft()->setParent(n1);
            }
            n2->setLeft(n1);
            n1->setParent(n2);
            //n1->setRight(n2->getLeft());
        }
    }

    // only look at above portion 

    // if(n2->getLeft() != NULL){
    //     AVLNode<Key,Value>* n3 = n2->getLeft();
    //     nodeSwap(n1, n3);
    //     n2->setLeft(NULL);
    //     n3->setLeft(n1);
    //     n1->setParent(n3);
    // }
    // else{
    //     tempNode = n1->getParent();
    //     if(tempNode != NULL){
    //         tempNode->setLeft(n2);
    //         n2->setParent(tempNode);
    //         n2->setLeft(n1);
    //         n1->setParent(n2);
    //     }
    //     else{
    //         n2->setParent(NULL);
    //         n2->setLeft(n1);
    //         n1->setParent(n2);
    //     }
    // }
    
}

    // right rotation
template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation(AVLNode<Key,Value>* n1)
{
    AVLNode<Key,Value>* n2 = n1->getLeft();

    AVLNode<Key,Value>* tempNode = n1->getParent();

     

    if(n2 != NULL){
        if(tempNode != NULL){
            if(tempNode->getLeft() == n1){
                tempNode->setLeft(n2);
            }
            else{
                tempNode->setRight(n2);
            }
            //tempNode->setLeft(n2);
            n2->setParent(tempNode);
            n1->setLeft(n2->getRight());
            if(n2->getRight()){
                n2->getRight()->setParent(n1);
            }
            n2->setRight(n1);
            n1->setParent(n2);
            
            //n1->setRight(n2->setLeft());
        }
        else{
            this->root_ = n2;
            n2->setParent(NULL);
            n1->setLeft(n2->getRight());
            if(n2->getRight()){
                n2->getRight()->setParent(n1);
            }
            n2->setRight(n1);
            n1->setParent(n2);
            //n1->setRight(n2->getLeft());
        }
    }


        // if(n2 != NULL){    
    //     if(tempNode != NULL){
    //         if(tempNode->getLeft() == n1){
    //             tempNode->setLeft(n2);
    //         }
    //         else{
    //             tempNode->setRight(n2);
    //         }
    //         tempNode->setRight(n2);
    //         n2->setParent(tempNode);
    //         n2->setRight(n1);
    //         n1->setParent(n2);
    //         n1->setLeft(NULL);
    //     }
    //     else{
    //         this->root_ = n2;
    //         n2->setParent(NULL);
    //         n2->setRight(n1);
    //         n1->setParent(n2);
    //         n1->setLeft(NULL);
    //     }
    // }

    

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int8_t diff)
{
    // if(n == NULL){
    //     return;
    // }

    // AVLNode<Key,Value>* parent_ = n->getParent();

    // int nodeDiff;

    // if(parent_ != NULL && parent_->getLeft() == n){
    //     nodeDiff = 1;
    // }
    // else{
    //     nodeDiff = -1;
    // }

    // int newBalNode = n->getBalance() + nodeDiff;
    

    // if(diff == -1){
    //     if(newBalNode == -2){
    //         AVLNode<Key,Value>* child_ = n->getLeft(); //might need to do a little more 

    //         if(child_->getBalance() == -1){
    //             rightRotation(n);
    //             n->setBalance(0);
    //             child_->setBalance(0);
    //             // if(parent_ != NULL){
    //             //     if(parent_->getLeft() == n){
    //             //         diff = 1;  
    //             //     }
    //             //     else{
    //             //         diff = -1;
    //             //     }
    //             // }
    //             removeFix(parent_, diff);
    //         }
    //         else if(child_->getBalance() == 0){
    //             rightRotation(n);
    //             n->setBalance(-1);
    //             child_->setBalance(1);
    //             return;
    //             //removeFix()
    //         }
    //         else if(child_->getBalance() == 1){

    //             AVLNode<Key,Value>* g  = child_->getRight();
    //             leftRotation(child_);
    //             rightRotation(n);

    //             if(g->getBalance() == 1){
    //                 n->setBalance(0);
    //                 child_->setBalance(-1);
    //                 //g->setBalance(0);
    //             }
    //             else if(g->getBalance() == 0){
    //                 n->setBalance(0);
    //                 child_->setBalance(0);
    //                 //g->setBalance(0);
    //             }
    //             else if(g->getBalance() == -1){
    //                 n->setBalance(1);
    //                 child_->setBalance(0);
    //                 //g->setBalance(0);
    //             }
    //             g->setBalance(0);
    //             // if(parent_ != NULL){
    //             //     if(parent_->getLeft() == n){
    //             //         diff = 1;  
    //             //     }
    //             //     else{
    //             //         diff = -1;
    //             //     }
    //             // }
    //             removeFix(parent_, diff);
    //         }
    //     }
    //     else if(newBalNode == -1){
    //         n->setBalance(-1);
    //         return;
    //         //removeFix(n->getParent(), -1);
    //     }
    //     else if(newBalNode == 0){
    //         n->setBalance(0);
    //         // if(parent_ != NULL){
    //         //     if(parent_->getLeft() == n){
    //         //         diff = 1;  
    //         //     }
    //         //     else{
    //         //         diff = -1;
    //         //     }
    //         // }
    //         removeFix(parent_, diff);
    //     } 
    // }
    // if(diff == 1){
    //     if(newBalNode == 2){
    //         AVLNode<Key,Value>* child_ = n->getRight(); //might need to do a little more 

    //         if(child_->getBalance() == 1){
    //             leftRotation(n);
    //             n->setBalance(0);
    //             child_->setBalance(0);
    //             // if(parent_ != NULL){
    //             //     if(parent_->getRight() == n){
    //             //         diff = -1;  
    //             //     }
    //             //     else{
    //             //         diff = 1;
    //             //     }
    //             // }
    //             removeFix(parent_, diff);
    //         }
    //         else if(child_->getBalance() == 0){
    //             leftRotation(n);
    //             n->setBalance(1);
    //             child_->setBalance(-1);
    //             return;
    //             //removeFix()
    //         }
    //         else if(child_->getBalance() == -1){

    //             AVLNode<Key,Value>* g  = child_->getLeft();
    //             rightRotation(child_);
    //             leftRotation(n);

    //             if(g->getBalance() == -1){
    //                 n->setBalance(0);
    //                 child_->setBalance(1);
    //                 //g->setBalance(0);
    //             }
    //             else if(g->getBalance() == 0){
    //                 n->setBalance(0);
    //                 child_->setBalance(0);
    //                 //g->setBalance(0);
    //             }
    //             else if(g->getBalance() == 1){
    //                 n->setBalance(-1);
    //                 child_->setBalance(0);
    //                 //g->setBalance(0);
    //             }
    //             g->setBalance(0);
    //             // if(parent_ != NULL){
    //             //     if(parent_->getRight() == n){
    //             //         diff = -1;  
    //             //     }
    //             //     else{
    //             //         diff = 1;
    //             //     }
    //             // }
    //             removeFix(parent_, diff);
    //         }
    //     }
    //     else if(newBalNode == 1){
    //         n->setBalance(1);
    //         return;
    //         //removeFix(n->getParent(), -1);
    //     }
    //     else if(newBalNode == 0){
    //         n->setBalance(0);
    //         // if(parent_ != NULL){
    //         //     if(parent_->getRight() == n){
    //         //         diff = -1;  
    //         //     }
    //         //     else{
    //         //         diff = 1;
    //         //     }
    //         // }
    //         removeFix(parent_, diff);
    //     } 
    // }
////////// ^^ uncomment above part 
//     if(diff == 1){
//         if((n->getBalance() + diff) == 2){
//             AVLNode<Key,Value>* child_ = n->getRight();

//             if(child_->getBalance() == 1){
//                 rightRotation(n);
//                 n->setBalance(0);
//                 child_->setBalance(0);
//                 if(parent_ != NULL){
//                     if(parent_->getRight() == n){
//                         diff = -1;  
//                     }
//                     else{
//                         diff = 1;
//                     }
//                 }
//                 removeFix(parent_, diff);
//             }
//             else if(child_->getBalance() == 0){
//                 rightRotation(n);
//                 n->setBalance(1);
//                 child_->setBalance(-1);
//                 //removeFix()
//             }
//             else if(child_->getBalance() == -1){
//                 AVLNode<Key,Value>* g  = child_->getLeft();
//                 leftRotation(child_);
//                 rightRotation(n);

//                 if(g->getBalance() == -1){
//                     n->setBalance(0);
//                     child_->setBalance(1);
//                     g->setBalance(0);
//                 }
//                 else if(g->getBalance() == 0){
//                     n->setBalance(0);
//                     child_->setBalance(0);
//                     g->setBalance(0);
//                 }
//                 else if(g->getBalance() == 1){
//                     n->setBalance(-1);
//                     child_->setBalance(0);
//                     g->setBalance(0);
//                 }
//             }
//         }
//         else if((n->getBalance() + diff) == 1){
//             n->setBalance(1);
//         }
//         else if((n->getBalance() + diff) == 0){
//             n->setBalance(0);
//                 if(parent_ != NULL){
//                     if(parent_->getRight() == n){
//                         diff = -1;  
//                     }
//                     else{
//                         diff = 1;
//                     }
//                 }
//                 removeFix(parent_, diff);
//         }
        
//     }
    if (n == NULL){
        return;
    }
	int nextDiff;
	AVLNode<Key, Value>* parent_ = n->getParent();
	if (parent_ != NULL) {
		if (parent_->getLeft() == n) {
			nextDiff = 1;
		}
		else{
            nextDiff = -1;
        } 
	}

	if (n->getBalance() + diff == diff * 2) {
		AVLNode<Key, Value>* child_;
		if (diff == -1){
            child_ = n->getLeft();  
            if (child_ != NULL) {
                if (child_->getBalance() == diff) {
                    rightRotation(n);
                    n->setBalance(0);
                    child_->setBalance(0);
                    removeFix(parent_, nextDiff);
                }
                else if (child_->getBalance() == 0) {
                    rightRotation(n);
                    n->setBalance(diff);
                    child_->setBalance(-1*diff);
                }
                else if (child_->getBalance() == -1*diff) {
                    AVLNode<Key, Value>* g = child_->getRight();;

                    leftRotation(child_);
                    rightRotation(n);

                    if (g->getBalance() == -1*diff) {
                        n->setBalance(0);
                        child_->setBalance(diff);
                        g->setBalance(0);
                    }

                    else if (g->getBalance() == 0) {
                        n->setBalance(0);
                        child_->setBalance(0);
                        g->setBalance(0);
                    }

                    else {
                        n->setBalance(-1*diff);
                        child_->setBalance(0);
                        g->setBalance(0);
                    }

                    removeFix(parent_, nextDiff);
                }
            }
        }
        else{
            child_ = n->getRight();

            if (child_ != NULL) {
                if (child_->getBalance() == diff) {
                    leftRotation(n);
                    n->setBalance(0);
                    child_->setBalance(0);
                    removeFix(parent_, nextDiff);
                }
                else if (child_->getBalance() == 0) {
                    leftRotation(n);
                    n->setBalance(diff);
                    child_->setBalance(-1*diff);
                }
                else if (child_->getBalance() == -1*diff) {
                    AVLNode<Key, Value>* g = child_->getLeft();
                    rightRotation(child_);
                    leftRotation(n);

                    if (g->getBalance() == -1*diff) {
                        n->setBalance(0);
                        child_->setBalance(diff);
                        g->setBalance(0);
                    }
                    else if (g->getBalance() == 0) {
                        n->setBalance(0);
                        child_->setBalance(0);
                        g->setBalance(0);
                    }

                    else {
                        n->setBalance(-1*diff);
                        child_->setBalance(0);
                        g->setBalance(0);
                    }

                    removeFix(parent_, nextDiff);
                }
            }
        
        }
	}
	else if (n->getBalance() + diff == diff){
		n->setBalance(diff);
	}
	else {
		n->setBalance(0);
		if (parent_ != NULL){
            removeFix(parent_, nextDiff);
        }
	}


}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
    //std::cout << "In insert_fix" << std::endl;

    if(p == NULL){
        return;
    }

    if(p->getParent() == NULL){
        return;
    }

    AVLNode<Key,Value>* g = p->getParent();

    //g's left child
    if(g->getLeft() == p){
        
        // if(p->getBalance() != 0){
        //     g->updateBalance(-1); 
        // }
        g->setBalance(g->getBalance() + -1); 

        if(g->getBalance() == 0){
            //g->setBalance(-1); //SYDNEY - FRI
            return;
        }
        else if(g->getBalance() == -1){
            insert_fix(g, p); //SYDNEY - FRI
            //return; //SYDNEY - FRI 
        }
        else if(g->getBalance() == -2){
            if (p->getLeft() == n) { //p->getBalance() == -1
                rightRotation(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else{
                //std::cout << "zig zag left child" << std::endl;
                leftRotation(p);
                //std::cout << "left rotation" << std::endl;
                rightRotation(g);
                //std::cout << "right rotation" << std::endl;
                if(n->getBalance() == -1){
                    p->setBalance(0);
                    g->setBalance(1);
                    //n->setBalance(0);
                }
                else if(n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    //n->setBalance(0);
                }
                else if(n->getBalance() == 1){
                    p->setBalance(-1);
                    g->setBalance(0);
                    //n->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }
    else if(g->getRight() == p){
        //g's right child
        // if(p->getBalance() != 0){
        //     g->updateBalance(1); //CHECK THIS
        // }
        g->setBalance(g->getBalance() + 1);


        if(g->getBalance() == 0){
            //g->setBalance(1); //SYDNEY - FRI
            return;
        }
        else if(g->getBalance() == 1){
            insert_fix(g, p); //SYDNEY - FRI
            //return; //SYDNEY - FRI
        }
        else if(g->getBalance() == 2){
            if(p->getRight() == n) { //before condition: p->getBalance() == 1 || p->getRight() == n
                leftRotation(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else{
                //std::cout << "zig-zag right child" << std::endl;
                rightRotation(p); 
                //std::cout << "right rotation" << std::endl;
                leftRotation(g);
                //std::cout << "left rotation" << std::endl;
                if(n->getBalance() == 1){
                    p->setBalance(0);
                    g->setBalance(-1);
                    //n->setBalance(0);
                }
                else if(n->getBalance() == 0){
                    p->setBalance(0);
                    g->setBalance(0);
                    //n->setBalance(0);
                }
                else if(n->getBalance() == -1){
                    p->setBalance(1);
                    g->setBalance(0);
                    //n->setBalance(0);
                }
                n->setBalance(0);
            }
        }
    }
    //insert_fix(g, p);

}

#endif
