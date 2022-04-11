#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>

#define Ang_type	200
#define Ang		201
#define Delay		202
#define Deg		203
#define Ask_press	204
#define Wait		205
#define Ang_step	206
#define Screen		207
#define Dist		208
#define Inclination	209
#define Home		210

#define Var		300
#define If		301
#define Loop		302
#define For		303
#define EndFor		304
#define EndLoop		305
#define EndIf		306


#define MATH_SIN		400
#define MATH_COS		401
#define MATH_EQUAL		402
#define MATH_LESS		403
#define MATH_GRTR		404
#define MATH_PLUS		405
#define MATH_MINS		406
#define MATH_MULTIPLY	407
#define MATH_DIVIDE		408
#define MATH_NUMBER		409
#define MATH_ISTO		410


#define MAX_LINE_LENGTH 60
#define MAX_PRG_LENGTH 100

#define TOKEN_ID 90
#define TOKEN_NAME 91
#define TOKEN_VAL 92
#define TOKEN_PARAM 93

#define TOKEN_OTHER 94

struct node{
	char *name;
	int id;
	int val;
	int uniqid;
	struct node *param;
	struct node *next;
	struct node *block;
}; 

struct node *token_tree;

int prg_lines = 0;

node* token_stack[10];
int token_stack_pointer = 0;

int generateID();
bool checkifempty(char *line);
void reset_enviornment(char* line);
int gather_vars(char* line,int prg_lines);
void split_tokens(char* line);
struct node* getlastnode(struct node** n);
int get_id(char* word);
struct node* get_param(char* word);
void displayList(struct node *node);
void push(struct node** head, int id,char* name, struct node* param);
struct node* append(struct node** head,  int id,char* name, struct node* param,struct node* block);
void insertAfter(struct node* prev_node, int id,char* name, struct node* param,struct node* block);
bool checkisnumber(char *word);
bool checkif_aUN(char *word);
const char* get_char(int id);
void append_token(int id,bool param);
struct node* findnode(struct node** head,int uniqid);
struct node* createnode( int id,char* name, struct node* param,struct node* block);

int main(void)
{
    char program[] = "var i = 10 \n\
ang_type deg \n\
ang 30d,30d wait \n\
delay 1000 \n\
ask_press \n\
\n\
\n\
 \n\
	\n\
if i < 10 : \n\
var k = 500\n\
k = 21 \n\
endif";

   char *prg_ptr = NULL;


    char *dummy_line;
    dummy_line = strtok_r(program,"\n",&prg_ptr);


    char line[MAX_PRG_LENGTH][MAX_LINE_LENGTH];

    //count total lines
    while(dummy_line){
	  if(!checkifempty(dummy_line)){
	  	++prg_lines;
	  	strcpy(line[prg_lines],dummy_line);
	  	split_tokens(dummy_line);

	  }

	  dummy_line = strtok_r(NULL,"\n",&prg_ptr);
	}

    printf ("\nTotal line count> %d \n",prg_lines);
	printf("______PROG__________\n");
	for (int i = 0 ; i<= prg_lines;i++)
		printf("%s\n", line[i]);

	printf("____________________\n");
    

    displayList(token_tree);

    return 0;
}





//linked list helpers

//insert a new node in front of the list
void push(struct node** head, int id,char* name, struct node* param, struct node* block)
{
   struct node* newNode = new node;
 
   newNode->id = id;
   newNode->name = name;
   newNode->param = param;
   newNode->param = block;

   newNode->uniqid = generateID();
 	
   newNode->next = (*head);
   (*head) = newNode;
}
 
//insert new node after a given node
void insertAfter(struct node* prev_node, int id,char* name, struct node* param, struct node* block)
{
if (prev_node == NULL)
{
   printf("the given previous node is required,cannot be NULL"); return; } 
 
   struct node* newNode = new node; 
   
   newNode->id = id;
   newNode->name = name;
   newNode->param = param;

   newNode->block = block;

   newNode->uniqid = generateID();

   newNode->next = prev_node->next;
    prev_node->next = newNode;
}

struct node* createnode( int id,char* name, struct node* param,struct node* block){
	struct node* newNode = new node;

	int uniqid = generateID();

	newNode->id = id;
	newNode->name = name;
	newNode->param = param;
	newNode->block = block;

	newNode->uniqid = uniqid;
    newNode->next = NULL;
	return newNode
}

 
struct node* append(struct node** head,  int id,char* name, struct node* param,struct node* block)
{
struct node* newNode = new node;
struct node *last = *head;

int uniqid = generateID();

   newNode->id = id;
   newNode->name = name;
   newNode->param = param;
   newNode->block = block;

   newNode->uniqid = uniqid;

newNode->next = NULL;
 
if (*head == NULL)
{
*head = newNode;
return newNode;
}
 
/* 5. Else traverse till the last node */
while (last->next != NULL)
last = last->next;
 
/* 6. Change the next of last node */
last->next = newNode;
return newNode;
}


struct node* getlastnode(struct node** head){
	struct node *last = *head;

	if (*head == NULL)
	{
	return *head;
	}
	 
	/* 5. Else traverse till the last node */
	while (last->next != NULL)
	last = last->next;
	 
	return last;

}

struct node* findnode(struct node** head,int uniqid){
	printf("Searching for : %d \n",uniqid);
	struct node *last = *head;

	if (last == NULL)
		return NULL;

	if (last->uniqid == uniqid)
		return last;
	 
	/* 5. Else traverse till the last node */
	while (last != NULL){
		if(last->uniqid == uniqid){
			printf("Current uniq: %d id: %s \n",last->uniqid,get_char(last->id));
			return last;}

		if (last->block != NULL){
			struct node* fnd = findnode(&last->block,uniqid);
			if (fnd)
				return fnd;
		}

	last = last->next;}
	 
	return NULL;

}

void displayList(struct node *node)
{
   //traverse the list to display each node
   while (node != NULL)
   {
      printf("%s-->", get_char(node->id) );
      if(node->param != NULL){
      		printf("(");
      		displayList(node->param);
      		printf(") ");
  		}

  		if(node->block != NULL){
      		printf(" [");
      		displayList(node->block);
      		printf("] ");
  		}
      node = node->next;
   }

} 

//programming variable setters

void split_tokens(char* line){

 char *word;
 char *rest = NULL;
 word = strtok_r(line," ",&rest);
 int i = 0;
 int id = get_id(word);

 append_token(id,false);
 struct node *last_node = getlastnode(&token_tree);

 //append(&token_tree,id,NULL,NULL);

 while(word){
 		id = get_id(word);
		printf("token %d %s ;id = %d \n",i,word,id);

		if (i > 0 )
 			append_token(id,true);

		word = strtok_r(NULL," ",&rest);
		++i;
    }
}

void append_token(int id,bool param){
	static int state_var;
	struct node* node_found ;

	switch(id){
		case If:
			node_found = findnode(&token_tree,token_stack[token_stack_pointer]);
			state_var = 0;
			printf("statevar > %d\n",state_var);

		case MATH_ISTO:
			token_stack
			++token_stack_pointer;
			state_var = 0;
			break;

		case EndIf:
			--token_stack_pointer;
			break;

		default:
			
			if(token_stack[token_stack_pointer] != NULL)
				if (param){
					if(token_stack[token_stack_pointer]->param != NULL)
						append(getlastnode(token_stack[token_stack_pointer]->param),id,NULL,NULL,NULL);
					else
						token_stack[token_stack_pointer]->param = createnode(id,NULL, NULL,NULL);
				}else
					token_stack[token_stack_pointer] = append(token_stack[token_stack_pointer],id,NULL,NULL,NULL);


			if(token_stack_pointer == 0)
				token_stack[token_stack_pointer] = append(&token_tree,id,NULL,NULL,NULL);
			

			

			break;


	}

}

const char* get_char(int id){

 if( id == If)
 	return "if";

 if( id == Loop)
 	return "loop";

 if( id == For)
 	return "for";

 if( id == EndFor)
 	return "endfor";

 if( id == EndLoop)
 	return "endloop";

 if( id == EndIf)
 	return "endif";

 if( id == MATH_SIN)
 	return "sin";

 if( id == MATH_COS)
 	return "cos";

 if( id == Ang_type)
 	return "ang_type";

  if( id == Ang)
 	return "ang";

  if( id == Delay)
 	return "delay";

  if( id == Deg)
 	return "deg";

  if( id == Ask_press)
 	return "ask_press";

  if( id == Wait)
 	return "wait";

  if( id == Ang_step)
 	return "ang_step";

  if( id == Dist)
 	return "dist";

  if( id == Inclination)
 	return "inclination";

  if( id == Home)
 	return "home";

 if( id == Var)
 	return "var";

 if( id == MATH_EQUAL)
 	return "=";

 if( id == MATH_LESS)
 	return "<";

 if( id == MATH_GRTR)
 	return ">";

 if( id == MATH_PLUS)
 	return "+";

 if( id == MATH_MINS)
 	return "-";


 if( id == MATH_DIVIDE)
 	return "/";

 if( id == MATH_MULTIPLY)
 	return "*";


 if( id == MATH_ISTO)
 	return ":";

 if(id == MATH_NUMBER)
 	return "numb";

 if( id == TOKEN_ID )
 	return "id";


 return "other";
}


int get_id(char* word){

 if( strcmp(word,"if")  == 0)
 	return If;

 if( strcmp(word,"loop") == 0)
 	return Loop;

 if( strcmp(word,"for") == 0)
 	return For;

 if( strcmp(word,"endfor") == 0)
 	return EndFor;

 if( strcmp(word,"endloop") == 0)
 	return EndLoop;

 if( strcmp(word,"endif") == 0)
 	return EndIf;

 if( strcmp(word,"sin") == 0)
 	return MATH_SIN;

 if( strcmp(word,"cos") == 0)
 	return MATH_COS;

 if( strcmp(word,"ang_type") == 0)
 	return Ang_type;

  if( strcmp(word,"ang") == 0)
 	return Ang;

  if( strcmp(word,"delay") == 0)
 	return Delay;

  if( strcmp(word,"deg") == 0)
 	return Deg;

  if( strcmp(word,"ask_press") == 0)
 	return Ask_press;

  if( strcmp(word,"wait") == 0)
 	return Wait;

  if( strcmp(word,"ang_step") == 0)
 	return Ang_step;

  if( strcmp(word,"dist") == 0)
 	return Dist;

  if( strcmp(word,"inclination") == 0)
 	return Inclination;

  if( strcmp(word,"home") == 0)
 	return Home;

 if( strcmp(word,"var") == 0)
 	return Var;

 if( strcmp(word,"=") == 0)
 	return MATH_EQUAL;

 if( strcmp(word,"<") == 0)
 	return MATH_LESS;

 if( strcmp(word,">") == 0)
 	return MATH_GRTR;

 if( strcmp(word,"+") == 0)
 	return MATH_PLUS;

 if( strcmp(word,"-") == 0)
 	return MATH_MINS;


 if( strcmp(word,"/") == 0)
 	return MATH_DIVIDE;

 if( strcmp(word,"*") == 0)
 	return MATH_MULTIPLY;

 if( strcmp(word,":") == 0)
 	return MATH_ISTO;

 if(checkisnumber(word))
 	return MATH_NUMBER;

 if(checkif_aUN(word))
 	return TOKEN_ID;


 return TOKEN_OTHER;
}


struct node* get_param(char *word){
	return NULL;
}

int gather_vars(char *line,int prg_lines){
 
for (int i = 0;i < prg_lines;i++){
     break; 
  }
 return 0;
}

void reset_enviornment(char *line){
 if (prg_lines != 0)
 {
  delete [] line;
  printf ("%s","redefined array");
 }
}

//string helper 


bool checkisnumber(char *word){
for (int i = 0; i< strlen(word);i++)
	if(!isdigit(word[i]))
		return false;

return true;
}


//alpha,not uppercase,not number
bool checkif_aUN(char *word){
	if (isdigit(word[0]) || isupper(word[0]))
		return false;

	for (int i = 0; i< strlen(word);i++)
		if(!isalnum(word[i]))
			if(!(strcmp(&word[i],"_") == 0))
				return false;

	return true;
}

bool checkifempty(char *line){
for (int i =0 ;i< strlen(line);i++){
    if ( !(line[i] == '\r'|| line[i]  == '\t' || line[i] == '\0' || line[i] == ' ' || line[i] == '\n' )){
	return false;
     }
  }
  return true;
}

int generateID()
{
    static int s_itemID{ 0 };
    return s_itemID++;
}