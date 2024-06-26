#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}
    //new->lower_than = lower_than;



void insertTreeMap(TreeMap * tree, void* key, void * value)
{
    if(tree == NULL || key == NULL || value == NULL) return;
    TreeNode * aux = tree->root;
    if(aux == NULL)
    {
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
        return;
    }
    while(aux != NULL)
        {
            if(is_equal(tree, key, aux->pair->key))
            {
                aux->pair->value = value;
                return;
            }
            if(tree->lower_than(key, aux->pair->key))
            {
                 
                if(aux->left == NULL)
                {
                
                    TreeNode * new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->left = new;
                    tree->current = new;
                    return;
                }
                else
                {
                    aux = aux->left;
                }
                
            }
            else
            {
                if(aux->right == NULL)
                {
                    TreeNode * new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->right = new;
                    tree->current = new;
                    return;
                }
                else
                {
                    aux = aux->right;
                }
                
            }        
        }
}

TreeNode * minimum(TreeNode * x)
{
    if(x == NULL) return NULL;
    while(x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node)
{
    if(tree == NULL || node == NULL) return;
    if(node->left == NULL && node->right == NULL)
    {
        if(node->parent == NULL)
        {
            tree->root = NULL;
            tree->current = NULL;
            free(node->pair->key);
            free(node->pair->value);
            free(node->pair);
            free(node);
            return;
        }
        if(node->parent->left == node)
        {
            node->parent->left = NULL;        
        }
        else
        {
            node->parent->right = NULL;         
        }       
    }
    else
    {
        if(node->left != NULL && node->right != NULL)
        {
            TreeNode * min = minimum(node->right);
            node->pair->key = min->pair->key;
            node->pair->value = min->pair->value;
            removeNode(tree, min);
            return;
        }
        else
        {
            if(node->left != NULL)
            {
                if(node->parent->left == node)
                {
                    node->parent->left = node->left;
                    node->left->parent = node->parent;                    
                }
                else
                {
                    node->parent->right = node->left;
                    node->left->parent = node->parent;                    
                }              
            }
            else
            {
                if(node->parent->left == node)
                {
                    node->parent->left = node->right;
                    node->right->parent = node->parent;                  
                }
                else
                {
                    node->parent->right = node->right;
                    node->right->parent = node->parent;                  
                }               
            }      
        }    
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}




Pair * searchTreeMap(TreeMap * tree, void* key)
{
    if(tree == NULL || tree->root == NULL || key == NULL) return NULL;   
    TreeNode * aux = tree->root;
    while(aux != NULL)
        {
            if(is_equal(tree, key, aux->pair->key))
            {
                tree->current = aux;
                return aux->pair;               
            }
            if(tree->lower_than(key, aux->pair->key))
            {
                aux = aux->left;        
            }
            else
            {
                aux = aux->right;
            }          
        }  
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
    if(tree == NULL || tree->root == NULL || key == NULL) return NULL;
    TreeNode * aux = tree->root;
    TreeNode * ub = NULL;
    while(aux != NULL)
        {
            if(is_equal(tree, key, aux->pair->key))
            {
                tree->current = aux;
                return aux->pair;
                
            }
            else if(tree->lower_than(key, aux->pair->key))
            {
                ub = aux;
                aux = aux->left;
            }
            else
            {
                aux = aux->right;   
            }
            
            
        }
    
    if(ub == NULL)
    {
        return NULL;
    }
    else
    {
        return ub->pair;
    }
    
    
    
    
    
    
        
    
    
    
    
  
}


Pair * firstTreeMap(TreeMap * tree)
{
    if(tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while(aux->left != NULL)
        {
            aux = aux->left;
            
        }
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    if(tree == NULL || tree->current == NULL) return NULL;
    TreeNode * aux = tree->current;
    if(aux->right != NULL)
    {
        aux = aux->right;
        while(aux->left != NULL)
            {
                aux = aux->left;
            }
        tree->current = aux;
        return aux->pair;     
    }
    else
    {
        TreeNode * aux2 = aux->parent;
        while(aux2 != NULL && aux == aux2->right)
            {
                aux = aux2;
                aux2 = aux->parent;             
            }
        tree->current = aux2;
        if(aux2 != NULL)
        {
            return aux2->pair;   
        }
        else
        {
            return NULL;         
        }
    }
}
