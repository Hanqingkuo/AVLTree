#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


template <typename KeyType>
class AVLtree
{

    protected:
    struct AVLNode
    {
        struct AVLNode *parent;
        struct AVLNode *leftChild;
        struct AVLNode *rightChild;
        int  balance;
        KeyType  key;

    };
    static AVLNode *buyNode()
    {
        AVLNode *p = (AVLNode*)malloc(sizeof(AVLNode));
        assert(p != NULL);
        memset(p,0,sizeof(AVLNode));
        return p;
    }
    static void freeNode(AVLNode* p)
    {
        if(p != NULL)
        {
            free(p);
        }
    }
    
    private:
    struct AVLNode *root;
    
    static bool insert(AVLNode *&ptr,KeyType x,AVLNode *pa)
    {
    bool result = false;

    if(ptr == NULL)
    {
        ptr = buyNode();
        ptr->key = x;
        ptr->parent = pa;
        result = true;
        return result;
    }
    else if(x > ptr->key)
    {
        result = insert(ptr->rightChild,x,ptr);
        switch(ptr->balance)
        {
            case 0 : 
                ptr->balance = 1;
                break;
            case 1:
                rightBalance(ptr);
                break;
            case -1:
                ptr->balance = 0;
                break;
        }
    }
    else if(x < ptr->key)
    {
        result = insert(ptr->leftChild,x,ptr);
        switch(ptr->balance)
        {
            case 0:
                ptr->balance = -1;
                break;
            case 1:
                ptr->balance = 0;
            case -1:
                leftBalance(ptr);
                break;
            default:
                break;
        }
    }
    return result;


}

    static void rotateLeft(AVLNode *&ptr)
    {
        AVLNode *rightChild = ptr->rightChild;
        rightChild->parent = ptr->parent;
        ptr->rightChild = rightChild->leftChild;
        if(rightChild->leftChild != NULL)
        {
            rightChild->leftChild->parent = ptr;
        }
        rightChild->leftChild = ptr;
        ptr->parent = rightChild;
        ptr = rightChild;
    }
    static void rotateRight(AVLNode *&ptr)
    {
        AVLNode *newRoot = ptr->leftChild;
        newRoot->parent = ptr->parent;
        ptr->leftChild = newRoot->rightChild;
        if(newRoot->rightChild != NULL)
        {
            newRoot->rightChild->parent = ptr;
        }
        newRoot->rightChild = ptr;
        ptr->parent = newRoot;
        ptr = newRoot;
    }

    static void rightBalance(AVLNode *&ptr)
    {
        AVLNode *&rightSub = ptr->rightChild;
        AVLNode* leftSub = NULL;
        switch(rightSub->balance)
        {
            case 0:
                std::cout<<"the right tree alread balance\n";
                break;
            case 1:
                rightSub->balance = 0;
                ptr->balance = 0;
                rotateLeft(ptr);
                 break;
            case -1:
                leftSub = rightSub->leftChild;
                switch(leftSub->balance)
                {
                    case 0:
                        rightSub->balance = 0;
                        ptr->balance = 0;
                        break;
                    case 1:
                        ptr->balance = -1;
                        rightSub->balance = 0;
                        break;
                    case -1:
                        ptr->balance = 0;
                        rightSub->balance = 1;
                        break;
                }
                leftSub->balance = 0;
                rotateRight(rightSub);
                rotateLeft(ptr);
                break;
        }
    }
    static void leftBalance(AVLNode *&ptr)
    {
        AVLNode *&leftSub = ptr->leftChild;
        AVLNode *rightSub = NULL;
        switch(leftSub->balance)
        {
            case 0:
                std::cout<<"left alread balance\n";
                break;
            case -1:
                ptr->balance = 0;
                leftSub->balance = 0;
                rotateRight(ptr);
                break;
            case 1:
                rightSub = leftSub->rightChild;
                switch(rightSub->balance)
                {
                    case 0:
                        ptr->balance = 0;
                        leftSub->balance = 0;
                        break;
                    case 1:
                        ptr->balance = 0;
                        leftSub->balance = -1;
                        break;
                    case -1:
                        ptr->balance = 1;
                        leftSub->balance = 0;
                        break;
                }
                rightSub->balance = 0;
                rotateLeft(leftSub);
                rotateRight(ptr);
                break;
                    
        }

    }
    static bool deleteNode(AVLNode *&t, KeyType x, bool &res)
    {
        if(t == NULL)
        {
            return false;
        }
        else if(t->key == x)
        {
            AVLNode *p = NULL;
            if(t->rightChild == NULL)
            {
                p = t;
                t = t->leftChild;
                if(t != NULL)
                t->parent = p->parent;
                delete p;
                res = true;
            }
            else if(t->leftChild == NULL)
            {
                p = t;
                t = t->rightChild;
                if(t != NULL)
                t->parent = p->parent;
                delete p;
                res = true;
            }
            else
            {
                p = t->leftChild;
                while(p->rightChild)
                {
                    p = p->rightChild;
                }
                t->key = p->key;
                deleteNode(t->leftChild,p->key,res);
            }
        }
        else if(t->key > x)
        {
            if(!deleteNode(t->leftChild,x,res))
            {
                return false;
            }
            if(res == true)
            {
                switch(t->balance)
                {
                    case 0:
                        t->balance = 1;
                        res = false;
                        break;
                    case -1:
                        t->balance = 0;
                        res = true;
                        break;
                    case 1:
                        rightBalance(t);
                        if(t->rightChild->balance == 0)
                        {
                            res = false;
                        }
                        else
                        {
                            res = true;
                        }
                        break;
                }
            }
        }
        else
        {
            if(!deleteNode(t->rightChild,x,res))
            {
                return false;
            }
            if(res)
            {
                switch(t->balance)
                {
                    case 0:
                        t->balance = -1;
                        res = false;
                        break;
                    case 1:
                        t->balance = 0;
                        res = true;
                        break;
                    case -1:
                        leftBalance(t);
                        if(t->leftChild->balance == 0)
                        {
                            res = false;
                        }
                        else
                        { 
                            res = true;
                        }
                        break;
                }

            }
        }
        return true;
    }
    static AVLNode* fast(AVLNode *p)
    {
        while(p != NULL && p->leftChild != NULL)
        {
            p = p->leftChild;
        }
        return p;
    }
    static AVLNode* next(AVLNode *p)
    {
        if(p == NULL)
        {
            return NULL;
        }
        if(p->rightChild != NULL)
            return fast(p->rightChild);
        else
        {
            AVLNode *pa = p->parent;
            while(pa != NULL && pa->leftChild != p)
            {
                p = pa;
                pa = pa->parent;
            }
            return pa;
        }
    }
    public:
    AVLtree():root(NULL){}
    ~AVLtree(){ root = NULL; }
    bool recur_insert(KeyType x);
    void *findValue(KeyType data);
    void show();
    bool erase(KeyType data);

};

template <typename KeyType>
 bool AVLtree<KeyType>::recur_insert(KeyType x)
{
    return insert(root,x,NULL);   
}

template <typename KeyType>
void AVLtree<KeyType>::show()
{
    if(root == NULL)
    {
        return;
    }
    for(AVLNode*p = fast(root); p != NULL; p = next(p))
    {
        std::cout<<p->key<<"   ";
    }
    std::cout<<"\n";
}

template <typename KeyType>
void* AVLtree<KeyType>::findValue(KeyType x)
{
    if(root == NULL)
    {
        return NULL;
    }
    AVLNode *p = root;

    while(p != NULL)
    {
        if(p->key > x)
        {
            p = p->rightChild;
        }
        else if(p->key < x)
        {
            p = p->leftChild;
        }
        else
        {
            return p;
        }
    }
    return NULL;
}
template <typename KeyType>
bool AVLtree<KeyType>::erase(KeyType x)
{
    bool res;
    return deleteNode(root,x,res);   
}

int main()
{
    AVLtree<int> tree;
    for(int i = 3; i < 20; ++i)
    {
        tree.recur_insert(i);
    }
    tree.show();
    int x = 0;
    while(std::cin>>x && x != -1)
    {
        tree.erase(x);
        tree.show();
    }
}






