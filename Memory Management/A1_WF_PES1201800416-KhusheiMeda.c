#include<stdlib.h>
#include "sanity_client.c"

int size;   //size of the entire array, essentially the parameter of alloc function
char *p;    //entire array from which we are allocating memory

struct block{
    int size_b; //size of block this bookkeeping structure corresponds to
    int status;  //1 for allocated and 0 for free
    struct block *next;//pointer to next block of memory
};

//head is a pointer to head of linked list of blocks  
//last_free_block initially points to the beginning of last free block.
static struct block *head, *last_free_block;  

void allocate(int n)
{
    p=(char*)malloc(sizeof(char));    //allocate char array from which blocks will get space
    size=n;
    head = (struct block*)p;   //initialize head pointer block to point to block
    last_free_block = head+sizeof(struct block);    //as soon as allocate function is called, a book-keeping structure is created, so the free space begins after the book-keeping record
    head->next = last_free_block;   //linking both the blocks of the allocated space
    head->status = 1;   
    head->size_b = sizeof(struct block);
    last_free_block->size_b = n-sizeof(struct block);
    last_free_block->status = 0;
    last_free_block->next = NULL;
}

void print_book()
{
    printf("Size of each book-keeping structure is %d bytes\n", head->size_b);
}

void* mymalloc(int size)    //implementing worst-fit
{
    if (size <= 0)  //cannot have size 0 or negative
        return NULL;
    int *d;
    struct block *temp = (struct block*)p;  //basically points to head
    int *max;
    max=(int*)(p+199);
    *max = 0;   //variable to keep track of size of biggest free block
    struct block *temp_free = (struct block*)p; //to keep track of remaining free block after a block is allocated
    while(temp) //loop to find size of max free block
    {
        if(temp->status == 0 && temp->size_b > *max)
        {
            *max = temp->size_b;
        }
        temp = temp->next;
    }
    for(temp = head; temp != NULL; temp = temp->next)   //loop to allocate size from max free block
    {
        if(temp->status == 0 && temp->size_b == *max)   //if free block exists
        {
            if(*max >= size)    //if free block can accomodate user requested size number of bytes
            {
                if(*max-size >= sizeof(struct block))   //if we can have a new bookkeeping structure along with allocated memory
                {
                    temp->size_b = size + sizeof(struct block);
                    temp_free = (struct block *)(((char*)temp) + size + sizeof(struct block));
                    temp_free->size_b = *max - size - sizeof(struct block);
                    temp_free->status = 0;
                    temp->next = temp_free;
                    temp_free->next = NULL;
                }
                //even if we have just enough bytes to accomodate user requested size and not bookkeping structure
                //we allocate the memory
                //as a previous bookkeping record can keep track of this memory
                temp->status = 1;   
                return temp;
            }
            return NULL;
        }
    }
    return NULL;    //not enough space to allocate size no of bytes
}

void myfree(void *b)
{
    struct block *temp = (struct block *)((char*)head);
    struct block *prev = NULL;
    while(temp && temp != b)    //find memory at pointer  
    {    
        if(temp->status == 0 && (temp->next)->next == b)
            prev = temp;
        temp = temp->next;
    }
    struct block *temp1;
    if(temp == b)   //if found
    {
        temp->size_b -= sizeof(struct block);   //decrease the book space ocuupied by the allocated block
        temp->status = 0;   //allocated block is now free
        temp1 = (struct block *)(((char*)temp) + temp->size_b);   //pointer to book block
        temp1->next = temp->next;   //book made to point to what was previously pointed by the allocated block which we just freed
        temp1->size_b = sizeof(struct block);   //sizeof book
        temp1->status = 1;  //book is allocated memory, so status is 1
        temp->next = temp1; //the freed memory points to book
    }
    while(prev && (prev->next)->next == temp)  //if there exists a previous free block which is left adjacent to newly freed block
    {
        prev->size_b += temp->size_b + sizeof(struct block);    //merge newly freed block with the previous free block
        prev->next = temp1; //previous free block points to book
        if(temp1 == NULL)   //check condition to break from loop after coalescence of blocks
        {
            break;
        }
        if((temp1->next)->status == 0)  //to merge consecutive free blocks, we have to make the loop repeat with updated pointer values of temp and temp1
        {
            temp = temp1->next;
            temp1 = temp->next;
        }
    }
}

void display_mem_map()
{
    //printf("start_addr\tblock_in_bytes\tstatus\n");
    struct block *temp = (struct block *)((char*)head);
    int *s;
    s = (int*)(p+199);
    *s = 0; //counter variable to keep track of start address
    while(temp)
    {
        if(*s == 0) //to print first book-keeping structure which is created when allocate function is called
        {
            printf("0\t%d\tbook\n", temp->size_b);
            (*s)+= temp->size_b;
        }
        else
        {
            if(temp->status == 1)   //if allocated
            {
                if(temp->next && !((temp->next)->size_b == 16))
                {
                    if(temp->size_b -(sizeof(struct block)))    //to avoid printing allocated blocks which have been freed as occupying 0 bytes
                        printf("%d\t%d\tallocated\n", *s, temp->size_b -(sizeof(struct block)));    //print allocated blocks
                    printf("%d\t%d\tbook\n", *s+temp->size_b-(sizeof(struct block)) , sizeof(struct block));    //print book-keeping details of allocated block                                                    
                }
                else    
                //for handling cases like the following
                //in the sample testcase given to us, after a,b,c are allocated, instead of freeing them, if user would have requested 21 or more bytes upto 36 bytes
                    printf("%d\t%d\tallocated\n", *s, temp->size_b);
            }
            else //if(temp->status == 0) 
            {
                if(temp->size_b)    //don't print when 0 bytes of memory is free
                    printf("%d\t%d\tfree\n", *s, temp->size_b);
            }
            
            *s+= temp->size_b;  //increment start address counter
        }
        temp = temp->next;
    }
}