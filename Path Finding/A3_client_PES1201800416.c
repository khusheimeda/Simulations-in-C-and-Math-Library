#include <stdio.h>
#include "A3_header_PES1201800416.h"

int main()
{
	int A, B, C;
	scanf("%d%d%d", &A, &B, &C);
	int X, Y, Z;
	scanf("%d%d%d", &X, &Y, &Z);
	int P, Q, R;
	scanf("%d%d%d", &P, &Q, &R);

	state_t start;
	set_state(&start, X, Y, Z);
    state_t goal;
    set_state(&goal, P, Q, R);

	
	int (*move[])(state_t *dst, int A, int B, int C) = {
        AtoB,
        AtoC,
        BtoA,
        BtoC,
        CtoA,
        CtoB
    };
    list_t soln_list;	//create doubly linked list of valid unique stated 
	init_list(&soln_list);	
	add_at_end(&soln_list, &start);	//add start state to list
	int fn_index;
	state_t temp = start;
	int count = 0;	//number of solutions
	printf("\n");
	while( soln_list.head_->st_.fn_index_ < 6 )	//as long as start state doesn't try out all possible transitions
	{
		fn_index = soln_list.tail_->st_.fn_index_;
		
		state_t temp1 = temp;
		
		//if transition results in valid state which is not already in the list
		if( fn_index < 6 && move[fn_index](&temp, A, B, C) &&  ! is_repeated(&soln_list, &temp) )	
		{
			temp.fn_index_ = 0;	//every state of the list should begin trying with first possible transition
			
			add_at_end(&soln_list, &temp);
			
			//if goal state is reached
			//increment count of solutions
			//display the list
			//increment the function count of the state prev to the final state so that the next transition of this state does not repeat the same path
			//backtrack
			if(are_same(&temp, &goal))	
			{
				++count;
				disp_list(&soln_list);
				printf("\n");
				soln_list.tail_->prev->st_.fn_index_ += 1;
				remove_at_end(&soln_list);
				temp = soln_list.tail_->st_;
			}
			
		}	

		//increment state's fn_index so that next transition is tried in the next iteration
		else if(are_same(&temp, &temp1) && fn_index < 6)
		{
			++soln_list.tail_->st_.fn_index_;	
		}

		//when temp is changed because the state is a valid state, but it is not added to the list because it is repeated
		//in  such a case, we have to reverse the effect of the transition function on temp
		//hence temp = temp1
		else if(!are_same(&temp, &temp1) && fn_index < 6)
		{
			temp = temp1;
			++soln_list.tail_->st_.fn_index_;
		}

		//when a deadend is reached- no possible transitions in a non-goal state
		//backtrack
		//increment the function count of the prev state so that the next transition of this state does not repeat the same path
		else
		{
			soln_list.tail_->prev->st_.fn_index_ += 1;
			remove_at_end(&soln_list);
			temp = soln_list.tail_->st_;
		}
	}
	printf("%d", count);
}