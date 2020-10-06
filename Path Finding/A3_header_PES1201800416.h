#ifndef A3_header_PES1201800416_H
#define A3_header_PES1201800416_H

//state structure and functions associated to a state 
//interface of state functions
struct state
{
    int A_;
    int B_;
    int C_;
	int fn_index_; // indicates next function to apply
};

typedef struct state state_t;
void set_state(state_t *ptr_state, int A, int B, int C);
void disp_state(const state_t *ptr_state);

//transition functions
int AtoB(state_t *dst, int A, int B, int C);
int AtoC(state_t *dst, int A, int B, int C);
int BtoA(state_t *dst, int A, int B, int C);
int BtoC(state_t *dst, int A, int B, int C);
int CtoA(state_t *dst, int A, int B, int C);
int CtoB(state_t *dst, int A, int B, int C);
int is_valid(const state_t *, int, int, int);
int are_same(const state_t *lhs, const state_t *rhs);


//node and list structures and functions associated to list
//interface of list functions
struct node
{
	state_t st_;
	struct node* prev;
	struct node* next;
};
typedef struct node node_t;
struct list
{
	node_t* head_;
	node_t* tail_;
};
typedef struct list list_t;
void init_list(list_t *ptr_list);
void add_at_end(list_t *ptr_list, const state_t* ptr_state);
void remove_at_end(list_t *ptr_list);
int is_repeated(list_t *ptr_list, const state_t* ptr_state);
void disp_list(const list_t *ptr_list);
#endif

