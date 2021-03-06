#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "assignment_2.h"

//data structure used to represent each value of numbers array
struct node 
{
    int data;
    struct node *next;
};

//for constructing the initial array of nodes pointing to the losing element nodes
//adjacency matrix 
void matrix_construction(struct node *arr, int length)  
{
    for(int i = 0; i< (int)log2(length) ; i++)  //looping through the columns, needed for shifting levels of nodes
    {
        if(arr[i].data != -1 && arr[i+1].data != -1)  //if level i and i+1 are filled
        {
            //c1 and c2 are pointers to the pair of elements being compared
            struct node *c1 = &arr[i];  
            struct node *c2 = &arr[i+1];    
            
            while(c2)   //printing the earlier winner of the two winners and the list of elements it has won over
            {
                printf("%d ", c2->data);
                c2 = c2->next;
            }
            printf("x ");
            while(c1)   //printing the later winner and the list of elements it has won over
            {
                printf("%d ", c1->data);
                c1 = c1->next;
            }
            printf("= ");

            if(arr[i].data > arr[i+1].data) 
            {
                //create a node for losing data
                //and inserting this node right after the winner node
                //and making it point to the previous element node that had lost to the winner
                struct node *loser1 = malloc(sizeof(struct node));  
                loser1->data = arr[i+1].data;       
                loser1->next = arr[i].next; 
                arr[i+1].data = arr[i].data; 
                arr[i+1].next = loser1;
            }
            else    
            {
                struct node *loser1 = malloc(sizeof(struct node));
                loser1->data = arr[i].data; 
                loser1->next = arr[i+1].next;   
                arr[i+1].next = loser1; 
            }

            struct node *temp2 = &arr[i+1]; //points to winner
            while(temp2)    //printing winner followed by all elements that have lost to it
            {
                printf("%d ", temp2->data);
                temp2 = temp2->next;
            }
            printf("\n");

            arr[i].data = -1;   //empty the ith node
            arr[i].next = NULL; 
        }
        
        //to copy data of arr[i] to arr[i+1] and make arr[i+1] point to what arr[i] was pointing to 
        else if(arr[i].data != -1 && arr[i+1].data == -1)   
        {
            arr[i+1].data = arr[i].data;    
            arr[i+1].next = arr[i].next;
            arr[i].data = -1;  //empty previous level
            arr[i].next = NULL;  
            break;
        }
    }
}

void find_second_greatest(int *numbers, int length)
{
    //a level indicates the number of matches played by a node
    struct node *arr= malloc(sizeof(struct node) * ((int)log2(length)+1));  //allocating space for log2(length) + 1 levels of nodes
    for(int i=0; i<(int)log2(length) +1; i++)   //initializing every node with a negative value before processing any number, indicating it is empty
    {
        arr[i].data = -1;
        arr[i].next = NULL;
    }
    for(int i = 0; i<length ; i++)  //for looping through the numbers
    {
        if(arr[0].data == -1)   //if node is empty=> no collision at level 0=> no match needs to be played
        {
            arr[0].data = numbers[i];
        }
        else    //collision occuring at level 0
        {
            printf("%d x %d = ", arr[0].data, numbers[i]);
            if(numbers[i] > arr[0].data) 
            {
                struct node *loser = malloc(sizeof(struct node));   //create a node for loser number
                loser->data = arr[0].data;     
                loser->next = NULL; 
                arr[0].data = numbers[i];   //replace data in 0th level node with winner data
                arr[0].next = loser; //points to loser node
                struct node *temp = &arr[0];    //for printing single winner element followed by single loser element, i.e, results of comparing two level 0 elements
                while(temp)
                {
                    printf("%d ", temp->data);
                    temp = temp->next;
                }
                printf("\n");
                matrix_construction(arr, length);
            }
            else
            {
                struct node *temp = malloc(sizeof(struct node));
                temp->data = numbers[i];
                temp->next = NULL;
                arr[0].next = temp;
                struct node *test = &arr[0];
                while(test)
                {
                    printf("%d ", test->data);
                    test = test->next;
                }
                printf("\n");
                matrix_construction(arr, length);
            }
        }
    }    
    
    if(log2(length) - (int)log2(length) > 0)    //if not a power of two
    {
        int extra = length - pow(2, (int)log2(length)); //the extra number of calls that need to be made
        while(extra > 0)
        {
            matrix_construction(arr, length);
            --extra;
        }
    }

    struct node * win = arr[(int)log2(length)].next;    //points to first loser of final winning list
    int ans = win->data;
    while(win)  //n-1 iterations through list of all elements that have lost to max element, in order to get second max element
    {
        if(win->data > ans)
            ans = win->data;
        win = win->next;
    }
    printf("\n%d", ans);
     
}