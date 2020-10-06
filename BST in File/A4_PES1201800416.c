#include <stdlib.h>
#include "assignment_4.h"

FILE* init_tree(const char* filename)
{
    tree_t h;
    h.free_head = h.root = -1;
    FILE* fp = fopen(filename, "r+");
	if(fp == NULL)
	{
		//printf("empty file\n");
		fp = fopen(filename, "w");
		h.free_head = -1;
		h.root = -1;
		fwrite(&h, sizeof(tree_t), 1, fp);
		fclose(fp);
		fp = fopen(filename, "r+");
	}
	fread(&h, sizeof(tree_t), 1, fp);
	return fp;
}

void close_tree(FILE *fp)
{
    fclose(fp);
}

void insert_key(int key, FILE *fp)
{
    int off;
	
	node_t temp_node;
	int temp;
    tree_t h;
    fseek(fp, 0, SEEK_SET);
    fread(&h, sizeof(tree_t), 1, fp);

    //finding position to insert node
	if(h.free_head == -1)   //insert in end  
	{
		fseek(fp, 0, SEEK_END); // seek to the end
		temp = ftell(fp);
		
	}
    else    //when some element has been deleted
    {
        temp = h.free_head;
		fseek(fp, temp, SEEK_SET);
		fread(&temp_node, sizeof(node_t), 1, fp);
		h.free_head = temp_node.left_offset;
	}
    temp_node.key = key;
	temp_node.left_offset = temp_node.right_offset = -1;

    
    if(h.root == -1) //if no elements
	{
		h.root = temp;
		temp_node.left_offset = -1;
		temp_node.right_offset = -1;
		fseek(fp, 0, SEEK_SET); fwrite(&h, sizeof(tree_t), 1, fp);
		fseek(fp, temp, SEEK_SET); fwrite(&temp_node, sizeof(node_t), 1, fp);
	}
	else
	{
        int prev = -1; int pres = h.root;
        // read the pres node-root
        node_t pres_node; node_t prev_node;
        fseek(fp, pres, SEEK_SET);
        fread(&pres_node, sizeof(node_t), 1, fp);

        while(pres != -1)
        {
            if(key < pres_node.key)
            {
                //pres_node = pres_node->left
                prev = pres;
                prev_node = pres_node;
                pres = pres_node.left_offset;
                if(pres != -1)
                {	
                    fseek(fp, pres, SEEK_SET);
                    fread(&pres_node, sizeof(node_t), 1, fp);
                }
                
            }
            else if(key == pres_node.key)
                return;
            else
            {
                //pres_node = pres_node->right
                prev = pres;
                prev_node = pres_node;
                pres = pres_node.right_offset;
                if(pres != -1)
                {
                    fseek(fp, pres, SEEK_SET);
                    fread(&pres_node, sizeof(node_t), 1, fp);
                }
            }
        }
        if(key < prev_node.key)
        {
            prev_node.left_offset = temp;
            fseek(fp, prev, SEEK_SET);
            fwrite(&prev_node, sizeof(node_t), 1, fp);
        
        }
        else
        {
            prev_node.right_offset = temp;
            fseek(fp, prev, SEEK_SET);
            fwrite(&prev_node, sizeof(node_t), 1, fp);
        }
        fseek(fp, temp, SEEK_SET);
        fwrite(&temp_node, sizeof(node_t), 1, fp);
		fseek(fp, 0, SEEK_SET); fwrite(&h, sizeof(tree_t), 1, fp);
    }
}

void delete_key(int key, FILE *fp)	
{
	tree_t h;
	fseek(fp, 0, SEEK_SET);
	fread(&h, sizeof(tree_t), 1, fp);
	if(h.root == -1)
	{
		//printf("Tree empty. Cannot delete\n");
		return;
	}
	else	//find key
	{
		int prev = -1; int pres = h.root;
        // read the pres node-root
        node_t pres_node; node_t prev_node;
        fseek(fp, pres, SEEK_SET);
        fread(&pres_node, sizeof(node_t), 1, fp);

        while(pres != -1)
        {
            if(key < pres_node.key)
            {
                //pres_node = pres_node->left
                prev = pres;
                prev_node = pres_node;
                pres = pres_node.left_offset;
                if(pres != -1)
                {	
                    fseek(fp, pres, SEEK_SET);
                    fread(&pres_node, sizeof(node_t), 1, fp);
                }
                
            }
            else if(key == pres_node.key)
                break;
            else
            {
                //pres_node = pres_node->right
                prev = pres;
                prev_node = pres_node;
                pres = pres_node.right_offset;
                if(pres != -1)
                {
                    fseek(fp, pres, SEEK_SET);
                    fread(&pres_node, sizeof(node_t), 1, fp);
                }
            }
        }
		node_t q1_node, p_node;	//q1_node is the inorder successor, p_node is its parent node
		int q1 = -1, p = -1;	//offset of inorder successor and parent, respectively
		if(key == pres_node.key && pres_node.left_offset == -1 && pres_node.right_offset == -1)	//node is a leaf
		{
			if(prev == -1)	//root
			{
				h.root = -1;
				pres_node.left_offset = h.free_head;
				h.free_head = pres;
				pres_node.key = -1;
				pres_node.right_offset = -1;
				fseek(fp, pres, SEEK_SET);
				fwrite(&pres_node, sizeof(node_t), 1, fp);
				fseek(fp, 0, SEEK_SET);
				fwrite(&h, sizeof(tree_t), 1 , fp);
			}
			else
			{
				if(pres == prev_node.left_offset)	//left child
				{
					prev_node.left_offset = pres_node.left_offset;
				}
				else	//right child
				{
					prev_node.right_offset = pres_node.left_offset;
				}
				fseek(fp, prev, SEEK_SET);
				fwrite(&prev_node, sizeof(node_t), 1, fp);
				pres_node.left_offset = h.free_head;
				h.free_head = pres;
				pres_node.key = -1;
				pres_node.right_offset = -1;
				fseek(fp, pres, SEEK_SET);
				fwrite(&pres_node, sizeof(node_t), 1, fp);
				fseek(fp, 0, SEEK_SET);
				fwrite(&h, sizeof(tree_t), 1 , fp);
			}
		}
		else if(key == pres_node.key && pres_node.left_offset != -1 && pres_node.right_offset == -1)	//no right subtree, only left
		{
			if(prev == -1)	//root
			{
				h.root = pres_node.left_offset;
			}
			else
			{
				//check if pres_node is left child or right child of prev_node
				if(pres == prev_node.left_offset)	//left child
				{
					prev_node.left_offset = pres_node.left_offset;
				}
				else	//right child
				{
					prev_node.right_offset = pres_node.left_offset;
				}
				fseek(fp, prev, SEEK_SET);
				fwrite(&prev_node, sizeof(node_t), 1, fp);
			}
			pres_node.key = -1;
			pres_node.right_offset = -1;
			pres_node.left_offset = h.free_head;
			h.free_head = pres;
			fseek(fp, pres, SEEK_SET);
			fwrite(&pres_node, sizeof(node_t), 1, fp);
			fseek(fp, 0, SEEK_SET);
			fwrite(&h, sizeof(tree_t), 1 , fp);
		}
		else if(key == pres_node.key && pres_node.left_offset == -1 && pres_node.right_offset != -1)	//no left subtree, only right subtree
		{
			if(prev == -1)	//root
			{
				h.root = pres_node.right_offset;
			}
			else
			{
				//check if pres_node is left child or right child of prev_node
				if(pres == prev_node.left_offset)	//left child
				{
					prev_node.left_offset = pres_node.right_offset;
				}
				else	//right child
				{
					prev_node.right_offset = pres_node.right_offset;
				}
				fseek(fp, prev, SEEK_SET);
				fwrite(&prev_node, sizeof(node_t), 1, fp);
			}
			pres_node.key = -1;
			pres_node.right_offset = -1;
			pres_node.left_offset = h.free_head;
			h.free_head = pres;
			fseek(fp, pres, SEEK_SET);
			fwrite(&pres_node, sizeof(node_t), 1, fp);
			fseek(fp, 0, SEEK_SET);
			fwrite(&h, sizeof(tree_t), 1 , fp);
			
		}
		else if(key == pres_node.key )	//both subtrees present
		{
			p_node = pres_node;
			p = pres;
			q1 = pres_node.right_offset;	
			fseek(fp, q1, SEEK_SET);
			fread(&q1_node, sizeof(node_t), 1, fp);
			if(q1_node.left_offset != -1)
			{
				p = q1;
				p_node = q1_node;
				q1 = q1_node.left_offset;
				while(q1 != -1)
				{
					fseek(fp, q1, SEEK_SET);
					fread(&q1_node, sizeof(node_t), 1, fp);
					if(q1_node.left_offset != -1)
					{
						p = q1;
						p_node = q1_node;
						q1 = q1_node.left_offset;
						fseek(fp, q1, SEEK_SET);
						fread(&q1_node, sizeof(node_t), 1, fp);
					}	
					else
					{
						break;
					}
				}
			}
			if(prev == -1)	//deleting root
			{
				h.root = q1;
				q1_node.left_offset = pres_node.left_offset;
			}		
			else	//deleting non-root
			{
				if(pres == prev_node.left_offset)	//left child
				{
					prev_node.left_offset = q1;
				}
				else	//right child
				{
					prev_node.right_offset = q1;
				}
				fseek(fp, prev, SEEK_SET);
				fwrite(&prev_node, sizeof(node_t), 1, fp);
				q1_node.left_offset = pres_node.left_offset;
				
			}
			p_node.left_offset = q1_node.right_offset;
			fseek(fp, p, SEEK_SET);
			fwrite(&p_node, sizeof(node_t), 1, fp);
			if(p_node.right_offset != q1)
				q1_node.right_offset = pres_node.right_offset;
			pres_node.key = -1;
			pres_node.right_offset = -1;
			pres_node.left_offset = h.free_head;
			h.free_head = pres;
			fseek(fp, q1, SEEK_SET);
			fwrite(&q1_node, sizeof(node_t), 1, fp);
			fseek(fp, pres, SEEK_SET);
			fwrite(&pres_node, sizeof(node_t), 1, fp);
			fseek(fp, 0, SEEK_SET);
			fwrite(&h, sizeof(tree_t), 1 , fp);
		}
		
	}
}

void preorder(FILE *fp, int pos, node_t *temp)
{
	int l = temp->left_offset;
	int r = temp->right_offset;
	printf("%d ", temp->key);
	if(l != -1)
	{
		pos = l;
		fseek(fp, l, SEEK_SET);
		fread(temp, sizeof(node_t), 1, fp);
		preorder(fp, pos, temp);	
	}
	if(r != -1)
	{
		pos = r;
		fseek(fp, r, SEEK_SET);
		fread(temp, sizeof(node_t), 1, fp);
		preorder(fp, pos, temp);	
	}
}

void display_preorder(FILE *fp)
{
	tree_t h;
	fseek(fp, 0, SEEK_SET);
	fread(&h, sizeof(node_t), 1, fp);
	if(h.root == -1)
	{
		printf("\n");
		return;
	}
		
	else
	{
		int pos = h.root;
		fseek(fp, h.root, SEEK_SET);
		node_t temp;
		//callback to recursivepreorder traversal function
		fread(&temp, sizeof(node_t), 1, fp);
		preorder(fp, pos, &temp);
		printf("\n");
	}
	
}

void inorder(FILE *fp, int pos, node_t *temp)
{
	int l = temp->left_offset;
	int r = temp->right_offset;
	int key = temp->key;
	if(l != -1)
	{
		fseek(fp, l, SEEK_SET);
		fread(temp, sizeof(node_t), 1, fp);
		inorder(fp, l, temp);	
		
	}
	printf("%d ", key);
	
	if(r != -1)
	{
		fseek(fp, r, SEEK_SET);
		fread(temp, sizeof(node_t), 1, fp);
		inorder(fp, r, temp);	
	}
	
}

void display_inorder(FILE *fp)
{
	tree_t h;
	fseek(fp, 0, SEEK_SET);
	fread(&h, sizeof(node_t), 1, fp);
	if(h.root == -1)
	{
		printf("\n");
		return;
	}
		
	else
	{
		node_t temp;
		fseek(fp, h.root, SEEK_SET);
		fread(&temp, sizeof(node_t), 1, fp);
		int pos = temp.left_offset;
		if(pos != -1)	//left subtree exists
		{
			inorder(fp, h.root, &temp);
		}
		else
		{
			pos = temp.right_offset;
			if(pos != -1)	//no left subtree, only right subtree 
			{
				inorder(fp, h.root, &temp);
			}
			else	//root is leaf
			{
				printf("%d", temp.key);
			}
			
		}
		printf("\n");
	}
}