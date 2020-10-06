#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intal.h"

static void reverse(char *arr)
{
    int n=strlen(arr);
    for(int i=0;i<n/2;++i)
    {
        char t= arr[i];
        arr[i]= arr[n-i-1];
        arr[n-i-1] = t;
    }
}

static void strip_trailing_zeros(char *arr)
{
	if(strcmp(arr, "0")==0)
		return ;
	int end;
	end = strlen(arr) - 1;
	while(end>0 && arr[end]=='0')
	{
		arr[end] = '\0';
		--end;
	}
}

char* intal_add(const char* intal1, const char* intal2)
{
    int l1= strlen(intal1);
    int l2= strlen(intal2);

    if(l1 >= l2)
    {
        int i=0;
        char *sum1;
        sum1 = (char*)malloc(sizeof(char)*(l1+2)); 
        int carry = 0;
        while(i<l2)
        {
            int s= (intal1[l1-1-i]-'0') + (intal2[l2-1-i]-'0') + carry;
            sum1[i]=(s%10) + '0';
            carry = s/10; 
            ++i;
        }
        while(i<l1)
        {
            int s = (intal1[l1-1-i]-'0') + carry;
            sum1[i] = (s%10) + '0';
            carry = s/10;
            ++i;
        }
        if(carry)
        {
           sum1[i]=carry + '0';
		    ++i; 
        }   
		sum1[i]='\0';

		reverse(sum1);
        return sum1;
    }
    else
    {
        int i=0;
        char *sum1;
		sum1= (char*)malloc(sizeof(char)*(l2+2));    
        int carry = 0;
        while(i<l1)
        {
            int s= (intal1[l1-1-i]-'0') + (intal2[l2-1-i]-'0') + carry;
            sum1[i]=(s%10) + '0';
            carry = s/10; 
            ++i;
        }
        while(i<l2)
        {
            int s = (intal2[l2-1-i]-'0') + carry;
            sum1[i] = (s%10) + '0';
            carry = s/10;
            ++i;
        }
        if(carry)
        {
           sum1[i]=carry + '0';
		    ++i; 
        }   
		sum1[i]='\0';
		
		reverse(sum1);
        return sum1;
    }
}

int intal_compare(const char* intal1, const char* intal2)
{
	int l1 = strlen(intal1);
	int l2 = strlen(intal2);

	if(l1 > l2)
    	return 1;
	else if(l2 > l1)
    	return -1;

	for(int i = 0; i < l1; ++i) 
	{
		if(intal1[i] > intal2[i])
			return 1;
		else if(intal2[i] > intal1[i])
			return -1;
  	}
  	return 0;
}

char* intal_diff(const char* intal1, const char* intal2)	
{
	int l1= strlen(intal1);
    int l2= strlen(intal2);

	if(intal_compare(intal1, intal2) == 0)
    {
		char *diff1;
		diff1 = (char*)malloc(2*sizeof(char));
		diff1[0] = '0';
		diff1[1] = '\0';
		return diff1;
    }
    else if(intal_compare(intal1, intal2) == 1)
    {
        int i=0;
        char *diff1;
		diff1 = (char*)malloc((l1+1) *sizeof(char));
        int borrow = 0;
		int size = 0;
        while(i<l2)
        {
            int d= (intal1[l1-1-i]-'0') - (intal2[l2-1-i]-'0') - borrow;
			if(d<0)
			{
				d= d + 10;
				borrow = 1;
			}
			else
			{
				borrow = 0;
			}
			
            diff1[i]=d + '0';
            ++i;
			++size;
        }
        while(i<l1)
        {
			if(intal1[l1-i-1]=='0' && borrow)
			{
				diff1[i] = '9';
				++i;
				++size;
				continue;
			}
            int d = (intal1[l1-1-i]-'0') - borrow;
			if(l1-i-1>0 || d>0)
			{
				diff1[i] = d + '0';
				++size;
			}
            borrow = 0;
            ++i;
        }
		diff1[size]='\0';
		strip_trailing_zeros(diff1);
		reverse(diff1);
        return diff1;
    }
    else
    {
        int i=0;
        char *diff1;
        diff1 = (char*)malloc((l2+1) *sizeof(char));
        int borrow = 0, size = 0;
        while(i<l1)
        {
            int d= (intal2[l2-1-i]-'0') - (intal1[l1-1-i]-'0') - borrow;
			if(d<0)
			{
				d= d + 10;
				borrow = 1;
			}
			else
			{
				borrow = 0;
			}
			
            diff1[i]=d + '0';
            ++i;
			++size;
        }
        while(i<l2)
        {
			if(intal2[l2-i-1]=='0' && borrow)
			{
				diff1[i] = '9';
				++i;
				++size;
				continue;
			}
            int d = (intal2[l2-1-i]-'0') - borrow;
			if(l2-i-1>0 || d>0)
			{
				diff1[i] = d + '0';
				++size;
			}
			++i;
            borrow = 0;
            
        }
		diff1[size] = '\0';
		strip_trailing_zeros(diff1);
		reverse(diff1);
        return diff1;
    }
}

char* intal_multiply(const char* intal1, const char* intal2)
{
	if(strcmp(intal1, "0") == 0 || strcmp(intal2, "0") == 0)
	{
		char *product;
		product = malloc(sizeof(char)*2);
		product[0] = '0';
		product[1] = '\0';
		return product;
	}
	
	int l1= strlen(intal1);
	int l2 = strlen(intal2);
	char *product1;
	product1 = (char*)(calloc(l1+l2+2, sizeof(char)));
    int left_shift = 0;

    for(int i = l2-1; i >= 0; i--)
    {
        int carry = 0, k = left_shift;
        for(int j = l1-1; j >= 0; j--)
        {
            int partial_product = (intal1[j] - '0')*(intal2[i] - '0') + carry;
			if(product1[k])
			{
				partial_product += (product1[k] - '0');
			}
                
            product1[k] = partial_product%10 + '0';
			++k;
            carry = partial_product/10;  
        }
        if(carry > 0)
            product1[k] = carry +'0'; 
        left_shift++;
    }
	reverse(product1);
	return product1;
}

char* intal_mod(const char* intal1, const char* intal2)
{	
	if(intal_compare(intal1, intal2)==0)	//if intal1==intal2
	{
		char *mod;
		mod = (char*)malloc(sizeof(char)*2);
		mod[0]='0';
		mod[1]='\0';
		return mod;	
	}
	if(intal_compare(intal1, intal2)== -1)	//intal1<intal2
	{
		char *mod;
		mod = (char*)malloc(sizeof(char)*strlen(intal1) + 1);
		strcpy(mod, intal1);
		return mod;	
	}

	int l1= strlen(intal1);
	int l2= strlen(intal2);
	char *intal2_extended;
	intal2_extended = (char*)malloc(sizeof(char)*(l1+2));
	strcpy(intal2_extended, intal2);
	int l2_ext=l2;
	char *intal1_copy;
	intal1_copy = (char*)malloc(sizeof(char)*(l1+1));
	strcpy(intal1_copy, intal1);	
	char *diff1;
	diff1 = (char*)malloc(sizeof(char)*(l1+1));

	while(intal_compare(intal1_copy, intal2) == 1|| intal_compare(intal1_copy, intal2) == 0)	//as long as intal1_copy>=intal2
	{
		while(intal_compare(intal1_copy, intal2_extended) == 1)	//intal1_copy is greater	//append trailing 0s to intal2_extended
		{
			intal2_extended[l2_ext]='0';
			intal2_extended[l2_ext+1]='\0';
			++l2_ext;
		}
		while(intal_compare(intal1_copy, intal2_extended) == -1)	//intal_copy is lesser	//remove trailing 0s from intal2_extended
		{
			--l2_ext;
			intal2_extended[l2_ext]='\0';
		}
		
		while(intal_compare(intal1_copy, intal2_extended)==1 || intal_compare(intal1_copy, intal2_extended)==0)
		{
			int i=0;
			l1 = strlen(intal1_copy);
			int borrow = 0, size = 0;
			while(i<l2_ext)
			{
				int d= (intal1_copy[l1-1-i]-'0') - (intal2_extended[l2_ext-1-i]-'0') - borrow;
				if(d<0)
				{
					d= d + 10;
					borrow = 1;
				}
				else
				{
					borrow = 0;
				}
				
				diff1[i]=d + '0';
				++i;
				++size;
			}
			while(i<l1)
			{
				if(intal1_copy[l1-i-1]=='0' && borrow)
				{
					diff1[i] = '9';
					++i;
					++size;
					continue;
				}
				int d = (intal1_copy[l1-1-i]-'0') - borrow;
				if(l1-i-1>0 || d>0)
				{
					diff1[i] = d + '0';
					++size;
				}
				borrow = 0;
				++i;
			}
			diff1[size] = '\0';
			strip_trailing_zeros(diff1);	//check if need to use a temporary pointer
			reverse(diff1);
			free(intal1_copy);
			intal1_copy = (char*)malloc(sizeof(char)*(strlen(diff1)+1));
			strcpy(intal1_copy, diff1);
		}	
	}
	free(intal2_extended);
	intal2_extended = NULL;
	free(diff1);
	diff1 = NULL;
	return intal1_copy;
}

char* intal_pow(const char* intal1, unsigned int n)
{
	char *intal1_copy;
	intal1_copy = (char*)malloc(sizeof(char)*(strlen(intal1)+1));
	strcpy(intal1_copy, intal1);
	char *pow1;
	pow1 = (char*)malloc(sizeof(char)*1001);
	pow1[0] = '1';
	pow1[1] = '\0';
	while(n>0)
	{
		if(n&1)
		{
			char *temp1 = intal_multiply(intal1_copy, pow1);
			free(pow1);
			pow1 = temp1;
		}
		n = n>>1;
		char *temp = intal_multiply(intal1_copy, intal1_copy);
		free(intal1_copy);
		intal1_copy = temp;
	}
	free(intal1_copy);
	intal1_copy = NULL;
	return pow1;
}

char* intal_gcd(const char* intal1, const char* intal2) 
{
	char* intal1_copy = (char*)malloc(sizeof(char)*(strlen(intal1) + 1));
  	strcpy(intal1_copy, intal1);	
  	char* intal2_copy = (char*)malloc(sizeof(char)*(strlen(intal2) + 1));
	strcpy(intal2_copy, intal2);	

	while(strcmp(intal2_copy, "0"))
	{
		char *r = intal_mod(intal1_copy, intal2_copy);	
		free(intal1_copy);
		intal1_copy = intal2_copy;	
		intal2_copy = r;	
	}
	free(intal2_copy);
	intal2_copy = NULL;
	return intal1_copy;
}

char* intal_fibonacci(unsigned int n)
{
	char* a;
	a = (char*)malloc(sizeof(char)*2);
	a[0]='0';
	a[1]='\0';

	if(n == 0)
		return a;

	char* b;
	b = (char*)malloc(sizeof(char)*2);
	b[0]='1';
	b[1]='\0';

	if(n == 1)
	{
		free(a);
		a = NULL;
		return b;
	}
	for(int i=1; i<n; ++i) 
	{
		char* c = intal_add(a, b);
		free(a);
		a = b;
		b = c;
	}
	free(a);
	a = NULL;
	return b;
}

char* intal_factorial(unsigned int n)
{
	char *one;
	one = malloc(sizeof(char)*2);
	one[0]='1';
	one[1]='\0';

	if(n<=1)
	{
		return one;
	}

	char* factorial;
	factorial = (char*)malloc(sizeof(char)*2);
	factorial[0] = '1';
	factorial[1] = '\0';
	char* next;
	next = (char*)malloc(sizeof(char)*2);
  	next[0] = '1';
  	next[1] = '\0';
	
	for(int i=1; i<n; ++i)	
  	{
		char* prev = next;	
		next = intal_add(one, next);	
		free(prev);
		prev = factorial;	
		factorial = intal_multiply(next, factorial);	
		free(prev);
		prev = NULL;
  	}
	free(next);
	next = NULL;
  	free(one);
	one = NULL;
	return factorial;
}

char* intal_bincoeff(unsigned int n, unsigned int k)	//exceeding time limit
{
	if(k>n/2)
		k=n-k;	

	char** c= (char**)malloc((k+1)*sizeof(char*));
	for(int i=0; i<k+1; ++i) 
	{
    	c[i]= (char*)malloc(sizeof(char)*2);
    	c[i][0] = '0';
    	c[i][1] = '\0';
  	}

	c[0][0] = '1';
  	for(int i=1; i<=n; ++i) 
  	{
    	for(int j= (i<k)?i:k; j>=0; --j) 
		{
			if((j!=i) && (j!=0))
			{
				char* temp = c[j];	
				c[j] = intal_add(c[j], c[j-1]);
				free(temp);
				temp = NULL;
			}
      		else
			{
				free(c[j]);
				c[j] = malloc(sizeof(char)*2);
				c[j][0] = '1';
				c[j][1] = '\0';
			}
			  
    	}
  	}
  	char* bincoeff= (char*)malloc(sizeof(char)*(strlen(c[k]) + 1));
  	strcpy(bincoeff, c[k]);
  	for(int i=0; i<k+1; ++i) 
  	{
    	free(c[i]);
		c[i] = NULL;
  	}
  	free(c);
	c = NULL;
	return bincoeff;
}

int intal_max(char **arr, int n)
{
	if(n==1)
		return 0;
	int max1_offset = 0;
  	for(int i=1; i<n; ++i) 
	{
    	if(intal_compare(arr[max1_offset], arr[i]) == -1)
      		max1_offset = i;
  	}
  	return max1_offset;
}

int intal_min(char **arr, int n)
{
	if(n==1)
		return 0;
	int min1_offset = 0;
  	for(int i=1; i<n; ++i) 
	{
    	if(intal_compare(arr[min1_offset], arr[i]) == 1)
      		min1_offset = i;
  	}
  	return min1_offset;
}

int intal_search(char **arr, int n, const char* key)
{
	if(n==1)
	{
		if(intal_compare(arr[0], key) == 0)
			return 0;
	}
  	for(int i=1; i<n; ++i) 
	{
    	if(intal_compare(arr[i], key) == 0)
		{	
			return i;
		}
  	}
  	return -1;
}

int intal_binsearch(char **arr, int n, const char* key)
{
	int l = 0;
  	int u = n-1;
	int result = -1;
 	while(l <= u) 
	{
    	int mid = (l + u)/2;
    	int cmp = intal_compare(arr[mid], key);
    	if(cmp == 0) 
		{
			result = mid;
			u = mid-1;
		}
      		
    	else if(cmp > 0) 
      		u = mid-1;
    	else 
      		l = mid+1;
    }
	return result;
}

static void heapify(char **arr, int n)//n is the index of the last element
{
    int i,j;
	char *key;
    j = 0;
    key = arr[j];
   
    i = 2*j + 1;	//find the index of the left child
    
    while(i <= n)//if the left child exists
	{
    	if((i+1) <= n)//check if the right child exists
      	{
        	if(intal_compare(arr[i+1], arr[i]) == 1)
           		i++; //get the index of the largest child
      	}
      	if(intal_compare(key, arr[i]) == -1)
      	{
        	arr[j] = arr[i];	//move child up
        	j = i;
        	i = 2*j + 1;	//find the left child
        }
      	else
        	break;
    }
   	arr[j] = key;//insert the key in its proper place
}

static void bot_heap(char **arr, int n)
{
	int i, j, k;
	char *key;
	for(k=(n-1)/2; k>=0; --k)	//start from the last parental node
  	{
    	j = k;
		key = arr[k];

		i = 2*j + 1;
		while(i <= n)	//if the left child exists
    	{
      		if((i+1) <= n)//check if the right child exists
     		{
        		if(intal_compare(arr[i+1], arr[i]) == 1)
					i++; //get the index of the largest child
      		}
			if(intal_compare(key, arr[i]) == -1)
			{
				arr[j] = arr[i];	//move child up
				j = i;
				i = 2*j + 1;//find the left child
			}
			else
				break;
		}
		arr[j] = key;	//insert the key in its proper place
    }
}

void intal_sort(char **arr, int n)
{
	bot_heap(arr, n-1);
	for(int i=n-1; i>0; --i)
	{
		char *t = arr[0];//swap first and the last element
		arr[0] = arr[i];
    	arr[i] = t;
    	heapify(arr, i-1);
	}
}

char* coin_row_problem(char **arr, int n)
{
	if(n == 0)
	{
		char *result;
		result = (char*)malloc(sizeof(char)*2);
		result[0] = '0';
		result[1] = '\0';
		return result;
	}
	
	char *prev;
	prev = (char*)malloc(sizeof(char)*1001);
	prev[0] = '0';
	prev[1] = '\0';

	char *curr;
	curr = (char*)malloc(sizeof(char)*1001);
	strcpy(curr, arr[0]);
	char *next;
	next = (char*)malloc(sizeof(char)*1001);
  	for(int i=2; i<=n; ++i)	
	{
    	char* amt = intal_add(prev, arr[i-1]);
    	if(intal_compare(amt, curr) == 1 || intal_compare(amt, curr) == 0)
		{
			strcpy(next, amt);
		}
      			
		else 
		{
			strcpy(next, curr);
			
		}
		if(intal_compare(prev, curr))
		strcpy(prev, curr);
		if(intal_compare(curr, next))
		strcpy(curr, next);
		free(amt);
			amt = NULL;
	}
  	free(prev);
  	prev = NULL;
  	free(next);
  	next = NULL;
	return curr;
}

