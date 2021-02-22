#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

/*--------------------------------------------------------------------------- FILE NAME is RecomendationDataSet.csv at LINE 197 ------------------------------------------------------------------*/

struct node
{
	char *books[100]; /* ARRAY of STRINGS to STORE BOOK NAMES */
	int points[100];	/* POINTS for BOOKS */
	char name[6];		/* USER IDs LIKE U19 or NU1 etc. */
};

float similarities(char x[5], char y[5], struct node *user_list); /* FINDING SIMILARITY RATIOS OF 2 USERS BETWEEN THEM */
float *most_similars(int k, struct node *user_list, char user[]); /* RETURNING A FLOAT ARRAY WHICH STORES SIMILARITY RATIOS */
struct node *bring_list(struct node* user_list);				 /* READING DATA FROM CSV FILE */
void recommend_book(struct node* user_list, float iterator_values[], char user[],int k); /* RECOMMEND BOOK/BOOKS ACCORDING TO SIMILARITY RATIOS & POINTS */

int main()
{	
	while(1)
	{
		printf("TYPE EXIT WHILE USERNAME ENTRANCE or -1 WHILE k ENTRANCE TO TERMINATE.\n\n");
		int i;
		char user[10];
		int k;
		float *arr;
		struct node *user_list;
		user_list=bring_list(user_list); /* READING FROM CSV FILE & STORING DATA in ARRAY OF STRUCTS */
		int flag=0;
		while(flag==0)
		{
			printf("USERNAME (EX NU1):");
			scanf("%s",user);			
			flag=is_in_list(user_list,user);
		}
		flag=0;
		while(flag==0)
		{
			printf("k:");
			scanf("%d",&k);
			flag=is_k_enough(user_list,k);
		}						
		arr=most_similars(k,user_list,user);	/* MOST SIMILAR k PERSON */
		recommend_book(user_list,arr,user,k);	/* RECOMMEND BOOKS */
		free(arr);
		free(user_list);
	}
	return 0;		
}

float *most_similars(int k, struct node *user_list, char user[])
{	
	/* FIRST ELEMENT OF ARRAY OF STRUCT IS FOR STORING NUMBER OF TOTAL USERS, NUMBER OF BOOKS, and NUMBER OF USERS WITH ID STARTS WITH U*/													
	int total_users=user_list[0].points[0];	
	int total_books=user_list[0].points[1];
	int u_users=user_list[0].points[2];
	float *similarity_values=(float*)calloc(u_users+1,sizeof(float)); /* STORING SIMILARITY RATIOS BETWEEN PARAMETER USER AND OTHER USERS WHOSE ID STARTS WITH 'U' */
	float *iterator_values=(float*)calloc((2*k+1),sizeof(float));	  /* STORING INDEXES OF MOST SIMILAR PERSON and SIMILARITY RATIOS */
	int iterator;
	float temp;
	int i,j;
	float max;
	for(i=1;i<=u_users;i++)
		similarity_values[i]=similarities(user,user_list[i].name,user_list);
	printf("\nTHE FIRST %d PEOPLE MOST LIKE USER %s and SIMILARITY RATIOS:\n",k,user);
	for(i=1;i<2*k+1;i=i+2)
	{	
		max=similarity_values[1];
		iterator=1;
		for(j=2;j<=u_users;j++)
		{												/* WARNING: EXPLANATION OF THE PART OF QUESTION 2 IN LINE 82 */
			temp=similarity_values[j];	
			if(temp>max)
			{											
				iterator=j;
				max=temp;
			}								/* DECIDING MAXIMUM SIMILARITY RATIO */ 
		}
		iterator_values[i]=iterator;
		iterator_values[i+1]=max;
		printf("%s : %f\n",user_list[iterator].name, similarity_values[iterator]);
		similarity_values[iterator]=-41;	/* INSTEAD OF SORTING, AFTER ADDING THE MAX SIMILARITY RATIO TO FLOAT ARRAY in k TIMES, I CHANGED THE MAXIMUM SIMILARITY RATIO IN EVERY LOOP BY MAKING IT -41 */
	}								
	printf("\n");
	return iterator_values;		/* RETURNING FLOAT ARRAY WHICH IS STORING LOCATION AND SIMILARITY RATIOS OF MOST SIMILAR USERS IN ARRAY OF STRUCT */
}

void recommend_book(struct node* user_list, float iterator_values[], char user[],int k)
{
	int total_users=user_list[0].points[0];
	int book_number=user_list[0].points[1]; /* FIRST ELEMENT OF ARRAY OF STRUCT IS FOR STORING NUMBER OF TOTAL USERS, NUMBER OF TOTAL BOOKS, and NUMBER OF USERS WITH ID STARTS WITH U */
	int nu_users=user_list[0].points[2];
	
	int i,j,m; /* ITERATORS */
	float max; /* MAXIMUM PREDICTION POINT OF RECOMMENDED BOOK */

	int loc; /* INDEX OF USER NUxxx in ARRAY of STRUCT */
	
	/* VARIABLES FOR FORMULA pred(a,p) */
	float up,down;
	float ra;
	float rb;

	float *recommend_points; /* RECOMMENDATION ARRAY */
	int *no_read=NULL; /* BOOKS ARRAY WHICH ARE NOT READ by NUxxx */
	
	int read_count;		/* COUNTER to FIND ra */
	int zero_count;		/* COUNTER to FIND rb */
	int sum;			/* VARIABLE for MEAN */
	int temp;		
/*----------------------------------------------------------------------------------------------------------*/
	/* FINDING LOCATION OF NUxxx */
	i=1;
	while(i<total_users)
	{
		if(strcmp(user_list[i].name,user)==0)
		{
			loc=i;
			i=total_users+1;
		}
		else i++;
	}
/*----------------------------------------------------------------------------------------------------------	*/
/* NUMBER of BOOKS WHICH are NOT READ by NUxxx & FINDING THE INDEXES of THESE BOOKS & FINDING ra in FORMULA*/
	sum=0;
	read_count=0;
	zero_count=0;
	for(i=0;i<book_number;i++)
	{	
		if(user_list[loc].points[i]>0)
		{
			sum+=user_list[loc].points[i];
			read_count++;
		}
		else
		{	
			no_read=(int*)realloc(no_read,((zero_count+1)*sizeof(int)));
			no_read[zero_count]=i;		/* STORING INDEX OF BOOKS NOT READ by NUxxx*/
			zero_count++;
		}
	}
	ra=sum/(float)read_count;			/* ra */
	recommend_points=(float*)calloc(zero_count,sizeof(float));	/* CREATING RECOMMENDATION ARRAY WHOSE SIZE IS = NUMBER OF BOOKS NOT READ by NUxxx */
//--------------------------------------------------------------------------------------------------------------------------------------------------
		
	for(i=0;i<zero_count;i++) /* FILLING RECOMMENDATION ARRAY WITH pred FORMULA */
	{	
		up=0;
		down=0;
		for(j=1;j<2*k+1;j+=2)
		{
			read_count=0;
			sum=0;
			for(m=0;m<book_number;m++)
			{	
				if(user_list[(int)iterator_values[j]].points[m]>0)
				{
					read_count++;
					sum=sum+user_list[(int)iterator_values[j]].points[m];
				}
			}
			rb=sum/(float)read_count; /* FINDING rb */
			temp=user_list[(int)iterator_values[j]].points[no_read[i]]; /* r(b,p) */
			up=up+(iterator_values[j+1]*(temp-rb));	
			down=down+iterator_values[j+1]; /* VALUES of sim(a,b) */
		}
		recommend_points[i]=ra+(up/down); /* pred(a,p)= ra + sum(sim(a,b)*(r(b,p)-rb))/sum(sim(a,b)) for EACH BOOK WHICH ARE NOT READ, RECOMMENDATION POINTS ARE STORED IN FLOAT ARRAY */
	}
	/* RECOMMENDATING MAXIMUM RATIO AND BOOK NAME BY FLOAT ARRAY  ABOVE */
	max=recommend_points[0];
	temp=0;
	printf("BOOKS WHICH ARE NOT READ BY USERNAME %s & ESTIMATED POINTS of THESE BOOKS:\n1. %s %f\n",user_list[loc].name,user_list[loc].books[no_read[0]],recommend_points[0]);
	for(i=1;i<zero_count;i++)
	{	
		printf("%d. %s %f\n",i+1,user_list[loc].books[no_read[i]],recommend_points[i]);
		if(recommend_points[i]>max)
		{
			max=recommend_points[i];
			temp=i;
		}
	}
	printf("\nRECOMMENDED BOOK - ESTIMATED POINT: (%s) - (%f)\n\n",user_list[loc].books[no_read[temp]], max);
	printf("***************************************************************************************\n\n");
}

/*--------------------- READING DATA from CSV FILE STARTS------------------------------*/
struct node *bring_list(struct node *user_list)
{
	int u_count=0;
	int count;
	int books_size;
	const char *delp;
	char **book_names = NULL;
	char line[500];
	FILE *fp=fopen("RecomendationDataSet.csv","r");
	if(!fp)
	{
		printf("FILE ERROR!\n");
		exit(0);
	}
	fgets(line,200,fp);
	line[strlen(line)-1]='\0';
	delp = strtok (line, ";");
	int i=0,j;
	
	while (delp != NULL)
	{
	    book_names = (char**)realloc(book_names, (i+1)*sizeof(*book_names));	
	    book_names[i] = (char*)malloc(sizeof(delp)+1);
	    strcpy(book_names[i], delp);
	    i++;
	    delp = strtok (NULL, ";");
    }
    
    books_size=i;
    user_list=(struct node*)malloc(sizeof(struct node));
	count=1;

	while(fgets(line,200,fp)!=NULL)
	{	
		user_list=(struct node*)realloc(user_list, (count+1)*sizeof(struct node));
		delp = strtok (line, ";");
		if(delp[0]=='U')
			u_count++;
		strcpy(user_list[count].name,delp);
 		delp = strtok (NULL, ";");
		j=0;
		while (delp != NULL )
		{	
	    	user_list[count].books[j]=book_names[j]; /* BOOK NAMES */
			user_list[count].points[j]=atoi(delp);	/* POINTS */
			delp = strtok (NULL, ";");
			j++;
    	}
    	count++;
	}
	/* AT FIRST ELEMENT, 1st CELL of POINTS[] IS STORING NUMBER OF TOTAL NUMBER, 2nd is NUMBER of BOOKS , 3rd is USERS WITH ID STARTS U */
	user_list[0].points[0]=count;	
	user_list[0].points[1]=books_size;
	user_list[0].points[2]=u_count;
	return user_list;	
}
/*--------------------- READING DATA from CSV FILE ENDS------------------------------*/

float similarities(char x[5], char y[5], struct node *user_list)
{	
	/* FUNCTION FOR sim(a,b) FORMULA */
	int found_count; /* FLAG to EXIT FROM WHILE LOOP */	
	int iterator1;	/* INDEXES OF 2 ELEMENTS WHO ARE GONNA BE COMPARED */
	int iterator2;	/* INDEXES OF 2 ELEMENTS WHO ARE GONNA BE COMPARED */
	float u1_sum;	/* TOTAL POINTS of USER 1 to ALL BOOKS */
	float u2_sum;	/* TOTAL POINTS of USER 2 to ALL BOOKS */
	float u1_mean;	/* AVG POINTS of USER 1 for BOOKS WHICH ARE READ*/
	float u2_mean;	/* AVG POINTS of USER 1 for BOOKS WHICH ARE READ */
	float result;	
	int total_readers=user_list[0].points[0];
	int total_books=user_list[0].points[1];
	int *u1_store=(int*)calloc(total_books,sizeof(int)); /* ARRAY INCLUDES POINTS of USER 1 for ALL BOOKS */
	int *u2_store=(int*)calloc(total_books,sizeof(int)); /* ARRAY INCLUDES POINTS of USER 2 for ALL BOOKS */
	float down_left;	/* VARIABLE FOR FORMULA */
	float down_right;	/* VARIABLE FOR FORMULA */
	int common_counter; /* COUNTER FOR BOOKS WHICH ARE READ BY BOTH USER 1 AND USER 2 */
	float up_sum;		/* VARIABLE FOR FORMULA */
	int j=1,i;			/* ITERATORS */
	found_count=0;
/*-----------------------------------------------------------------------------------*/
/* FINDING INDEXES of PARAMETER x and PARAMETER y USER in ARRAY of STRUCTS */
	while(j<total_readers && found_count<2)
	{
		if(strcmp(x,user_list[j].name)==0 || strcmp(y,user_list[j].name)==0)
		{
			if(strcmp(x,user_list[j].name)==0)
			{
				iterator1=j;
				found_count++;						
			}
			else
			{
				iterator2=j;
				found_count++;
			}
		}
		j++;
	}
//------------------------------------------------------------------------------	
	u1_sum=0;
	u2_sum=0;
	common_counter=0;
	for(i=0;i<total_books;i++)	/* P part of FORMULA */
	{	/* BOOKS READ by BOTH USER1 and USER2 */			
		if(user_list[iterator1].points[i]>0 && user_list[iterator2].points[i]>0)
		{
			u1_sum+=user_list[iterator1].points[i];
			u1_store[common_counter]=user_list[iterator1].points[i];
			u2_sum+=user_list[iterator2].points[i];
			u2_store[common_counter]=user_list[iterator2].points[i];
			common_counter++; /* INCREASING THE COMMON COUNTER for P in FORMULA */
		}
	}
	u1_mean=u1_sum/(float)common_counter;	/* ra */
	u2_mean=u2_sum/(float)common_counter;	/* rb */
	up_sum=0;						/* NUMERATOR PART of FORMULA */
	down_left=0;					/* LEFT PART of DENUMERATOR of FORMULA */
	down_right=0;					/* RIGHT PART of DENUMERATOR of FORMULA */
	
	for(i=0;i<common_counter;i++)	
	{										
		up_sum+=(u1_store[i]-u1_mean)*(u2_store[i]-u2_mean);
		down_left+=(u1_store[i]-u1_mean)*(u1_store[i]-u1_mean);
		down_right+=(u2_store[i]-u2_mean)*(u2_store[i]-u2_mean);
	}
	down_left=sqrt(down_left);
	down_right=sqrt(down_right);
	result=up_sum/(float)(down_left*down_right); /* FORMULA sim(a,b) */

	return result;
}

/* CHECKING IF INPUT username is in LIST */
int is_in_list(struct node *user_list, char username[])
{	
	if(strcmp(strupr(username),"EXIT")==0)
	{
		printf("PROGRAM TERMINATED!\n");
		exit(1);
	}
	int i=1;
	int length=user_list[0].points[0];
	int flag=0;
	while(i<length)
	{
		if(strcmp(username,user_list[i].name)==0)
			return 1;
		i++;
	}
	printf("%s IS NOT IN LIST!\n",username);
	return 0;
}


/* CHECKING IF k > NUMBER OF USERS WHOSE ID STARTS WITH 'U' */
int is_k_enough(struct node *user_list, int k)
{	
	if(k==-1)
	{
		printf("PROGRAM TERMINATED!\n");
		exit(1);
	}
	int i=0;
	int length=user_list[0].points[2];
	if(k>length || k<=0)
	{
		printf("ENTER PROPER VALUE of k!\n");
		return 0;
	}
		
	return 1;
}



