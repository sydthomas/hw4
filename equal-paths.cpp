
#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
#include <iostream>

using namespace std;


// You may add any prototypes of helper functions here

int helperEqualPaths(Node* root);


int helperEqualPaths(Node* root)
{
    int rightlen = 0;
    int leftlen = 0;


    if(root->right != nullptr){
        int temp = helperEqualPaths(root->right) +1;
        if(temp == 0){
            return -1;
        }
        rightlen = temp;
        //return rightlen;  // delete this 
    }
 
    if(root->left != nullptr){
        int temp = helperEqualPaths(root->left) + 1;
        if(temp == 0){
            return -1;
        }
        leftlen = temp;
        //return leftlen; // delete this 

    }

    if(root->left == nullptr && root->right == nullptr){
        return 1;
    }

    // if(leftlen == -1 || rightlen == -1){
    //     return (leftlen == rightlen);
    // }

    if(root->left == nullptr){
        return rightlen;
    }
    // else{
    //     return leftlen;
    // }

    else if(root->right == nullptr){
        return leftlen;
    }
    // else{
    //     return rightlen;
    // }

    if(leftlen != rightlen){
        return -1;
    }
    else{
        return leftlen;
    }

    //return 0;  
}



/*

    - go all the way down
    - add one to the legnth 

    calls resursive call 
    then does hte work 


*/

bool equalPaths(Node * root)
{
    if(root == nullptr){
        return true;
    }

    if(root->left == nullptr && root->right == nullptr){
        return true;
    }

    int equalLens = helperEqualPaths(root);

    if(equalLens <= -1){
        return false;
    }
    return true; 
}