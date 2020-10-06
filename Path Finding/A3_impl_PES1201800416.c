#include <stdio.h>
#include <stdlib.h>
#include "A3_header_PES1201800416.h"


//implementation of state functions
void set_state(state_t *ptr_state, int A, int B, int C)	//initialise state values with A, B, C values.
{
    ptr_state->A_ = A;
    ptr_state->B_ = B;
    ptr_state->C_ = C;
    ptr_state->fn_index_ = 0;	//Initially, fn_index is 0 because every state has to try all possible tansitions beginning with the first one
}

void disp_state(const state_t * ptr_state)
{
    printf("%d %d %d\n", ptr_state->A_, ptr_state->B_, ptr_state->C_);
}

int AtoB(state_t *dst, int A, int B, int C) 
{
    while(dst->A_ > 0 && dst->B_ < B )	//till either A is empty or B has reached its maximum capacity 
    {
        dst->A_ -=1;
        dst->B_ += 1;
    }
    return is_valid(dst, A, B, C);
}

int AtoC(state_t *dst, int A, int B, int C)
{
    while(dst->A_ > 0 && dst->C_ < C ) 
    {
        dst->A_ -=1;
        dst->C_ += 1;
    }
    return is_valid(dst, A, B, C);
}

int BtoA(state_t *dst,  int A, int B, int C)
{
    while(dst->B_ > 0 && dst->A_ < A ) 
    {
        dst->B_ -=1;
        dst->A_ += 1;
    }
    return is_valid(dst, A, B, C);
}

int BtoC(state_t *dst, int A, int B, int C)
{
    while(dst->B_ > 0 && dst->C_ < C ) 
    {
        dst->B_ -=1;
        dst->C_ += 1;
    }
    return is_valid(dst, A, B, C);
}

int CtoA(state_t *dst, int A, int B, int C)
{
    while(dst->C_ > 0 && dst->A_ < A ) 
    {
        dst->C_ -=1;
        dst->A_ += 1;
    }
    return is_valid(dst, A, B, C);
}

int CtoB(state_t *dst, int A, int B, int C)
{
    while(dst->C_ > 0 && dst->B_ < B ) 
    {
        dst->C_ -=1;
        dst->B_ += 1;
    } 
    return is_valid(dst, A, B, C); 
}


int are_same(const state_t *lhs, const state_t *rhs)	//checks if litres in the jugs are same
{
    return lhs->A_ == rhs->A_ && lhs->B_ == rhs->B_ && lhs->C_ == rhs->C_;
}

int is_valid(const state_t *ptr_state, int A, int B, int C)	//state is valid if litre constrainsts of every jug are satisfied.
{
    return ptr_state->A_ <= A && ptr_state->B_ <= B && ptr_state->C_ <= C &&
            ptr_state->A_ >= 0 && ptr_state->B_ >= 0 && ptr_state->C_ >= 0;
}



//implementation of list functions
void init_list(list_t *ptr_list)
{
	ptr_list->head_ = NULL;
	ptr_list->tail_ = NULL;
}
void add_at_end(list_t *ptr_list, const state_t* ptr_state)
{
	node_t* temp = (node_t*)malloc(sizeof(node_t));
	temp->st_ = *ptr_state;
	// empty
	if(ptr_list->head_ == NULL)
	{
		temp->prev = ptr_list->tail_;
		ptr_list->head_ = temp;
		ptr_list->tail_ = temp;
		temp->next = NULL;
	}
	// non-empty
	else
	{
		temp->prev = ptr_list->tail_;
		ptr_list->tail_->next = temp;
		ptr_list->tail_ = temp;
		temp->next = NULL;
	}

}
void remove_at_end(list_t *ptr_list)
{
	//empty
	if(ptr_list->head_ == NULL)
	{
		printf("empty\n");
	}
	// one node list
	else if(ptr_list->head_ == ptr_list->tail_)
	{
		free(ptr_list->head_);
		ptr_list->head_ = ptr_list->tail_ = NULL;
	}
	// other
	else
	{
		node_t *temp = ptr_list->tail_;
		ptr_list->tail_ = temp->prev;
		ptr_list->tail_->next = NULL;
		free(temp);
	}
	
}

int is_repeated(list_t *ptr_list, const state_t* ptr_state)
{
	node_t* temp = ptr_list->head_;
	while(temp)
	{
		if(are_same(&temp->st_, ptr_state))
		{
			//printf("same in is_repeated\n");	
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

void disp_list(const list_t *ptr_list)
{
	node_t* temp = ptr_list->head_;
	while(temp)
	{
		disp_state(&temp->st_);
		temp = temp->next;
	}
}


