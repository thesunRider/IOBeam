#include <stdio.h>
#include <stdlib.h>

#define Ang_type  200
#define Ang   201
#define Delay   202
#define Deg   203
#define Ask_press 204
#define Wait    205
#define Ang_step  206
#define Screen    207
#define Dist    208
#define Inclination 209
#define Home    210

#define Var   300
#define If    301
#define Loop    302
#define For   303
#define EndFor    304
#define EndLoop   305
#define EndIf   306


#define MATH_SIN    400
#define MATH_COS    401
#define MATH_EQUAL
#define MATH_LESS
#define MATH_GRTR
#define MATH_PLUS
#define MATH_MINS
#define MATH_NUMBER


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
  struct node *param;
  struct node *next;
}; 

struct node *token_tree;

int prg_lines = 0;

bool checkifempty(char *line);
void reset_enviornment(char* line);
int gather_vars(char* line,int prg_lines);
void split_tokens(char* line);
struct node* getlastnode(struct node** n);
int get_id(char* word);
struct node* get_param(char* word);
void displayList(struct node *node);
void push(struct node** head, int id,char* name, struct node* param);
void append(struct node** head,  int id,char* name, struct node* param);
void insertAfter(struct node* prev_node, int id,char* name, struct node* param);

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
var k = 500\n\
k = 21";

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
void push(struct node** head, int id,char* name, struct node* param)
{
   struct node* newNode = new node;
 
   newNode->id = id;
   newNode->name = name;
   newNode->param = param;
 
   newNode->next = (*head);
   (*head) = newNode;
}
 
//insert new node after a given node
void insertAfter(struct node* prev_node, int id,char* name, struct node* param)
{
if (prev_node == NULL)
{
   printf("the given previous node is required,cannot be NULL"); return; } 
 
   struct node* newNode = new node; 
   
   newNode->id = id;
   newNode->name = name;
   newNode->param = param;
   newNode->next = prev_node->next;
    prev_node->next = newNode;
}
 
void append(struct node** head,  int id,char* name, struct node* param)
{
struct node* newNode = new node;
struct node *last = *head;

   newNode->id = id;
   newNode->name = name;
   newNode->param = param;

newNode->next = NULL;
 
if (*head == NULL)
{
*head = newNode;
return;
}
 
/* 5. Else traverse till the last node */
while (last->next != NULL)
last = last->next;
 
/* 6. Change the next of last node */
last->next = newNode;
return;
}


struct node* getlastnode(struct node** head){
struct node *last = *head;

 
if (*head == NULL)
{
printf("HEAD CANT BE NULL\n");
return NULL;
}
 
/* 5. Else traverse till the last node */
while (last->next != NULL)
last = last->next;
 
return last;

}

void displayList(struct node *node)
{
   //traverse the list to display each node
   while (node != NULL)
   {
      printf("%d-->",node->id);
      if(node->param != NULL){
          printf("(");
          displayList(node->param);
          printf(") ");
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
 int id;


append(&token_tree,get_id(word),NULL,NULL);
struct node *last_node = getlastnode(&token_tree);
 

 while(word){
    printf("token %d %s ;id = %d \n",i,word,get_id(word));
    if (i > 0)
      append(&last_node->param,get_id(word),NULL,NULL);

    word = strtok_r(NULL," ",&rest);
    ++i;
    }
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

bool checkifempty(char *line){
for (int i =0 ;i< strlen(line);i++){
    if ( !(line[i] == '\r'|| line[i]  == '\t' || line[i] == '\0' || line[i] == ' ' || line[i] == '\n' )){
  return false;
     }
  }
  return true;
}
