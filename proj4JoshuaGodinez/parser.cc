#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include "ir_debug.h"
#include "compiler.h"
#include "lexer.h"
struct parsing
{
public:
    void parse_id_list();

};
struct StatementNode* parse_body();
struct ValueNode* max[100];
LexicalAnalyzer lexer;

int counter= 0, icounter=0;
 char* chr;


struct ValueNode* match_up(char* x)
{
 for(int i = 0; i < counter; i++)
{
   if(strcmp(x, max[i]-> name.c_str()) == 0)
   {
   return max[i];
   }
   else
   {
    return NULL;
   }
}

}

void parsing::parse_id_list()
{

  Token tok = lexer.GetToken();
  while (tok.token_type != SEMICOLON)    // when read semicolon, next line
  {
   max[counter] = new ValueNode{};
   max[counter] -> value = 0;
   const char *temp = (tok.lexeme).c_str();
   max[counter] -> name = tok.lexeme;

   counter++; //increment,
   tok = lexer.GetToken();
   }
 }
 
 struct StatementNode* parse_assign()
{    struct StatementNode* stmt = new StatementNode;
    stmt->next = NULL;
    Token tok = lexer.GetToken();
    if (tok.token_type == ID)
    {
        stmt->type = ASSIGN_STMT;
        stmt->assign_stmt = new AssignmentStatement;
        for(int i =0; i< counter; i++) {

           if(max[i]->name == tok.lexeme)
           {
           stmt->assign_stmt->left_hand_side = max[i];
           }
        }
    }
    tok = lexer.GetToken();
    if (tok.token_type == EQUAL) {
        tok = lexer.GetToken();

        switch (tok.token_type)
            {
                case ID:
                     for(int i =0; i< counter; i++)
                     {
                     if(max[i]->name == tok.lexeme)
                     {
                       stmt->assign_stmt->operand1 = max[i];
                     }
                     }
                    break;

                case NUM:
                    max[counter+icounter] = new ValueNode{};
                    chr = strdup((tok.lexeme).c_str());
                    max[counter+icounter]->value = atoi(chr);
                    max[counter+icounter]->name = "";
                    stmt->assign_stmt->operand1 = max[counter+icounter];
                    icounter++;
                    break;
            }

        tok = lexer.GetToken();
        if (tok.token_type == SEMICOLON)
        {
            stmt->assign_stmt->op = OPERATOR_NONE;
            stmt->assign_stmt->operand2 = NULL;
        }
        else
        {
            if (tok.token_type ==  PLUS)
            {
            stmt->assign_stmt->op = OPERATOR_PLUS;
            }
            else if(tok.token_type == MINUS)
            {
             stmt->assign_stmt->op =  OPERATOR_MINUS;
            }
            else if(tok.token_type == MULT)
            {
              stmt->assign_stmt->op =  OPERATOR_MULT;
            }
            else if(tok.token_type == DIV)
            {
               stmt->assign_stmt->op =  OPERATOR_DIV;
            }
            tok = lexer.GetToken();
            switch (tok.token_type)
            {
                case ID:
                     for(int i =0; i< counter; i++)
                     {
                      if(max[i]->name == tok.lexeme)
                     {
                      stmt->assign_stmt->operand2 = max[i];
                     }

                     }
                    break;
                case NUM:
                    max[counter+icounter] = new ValueNode{};
                    chr = strdup((tok.lexeme).c_str());
                    max[counter+icounter]->value = atoi(chr);
                    max[counter+icounter]->name = "";
                    stmt->assign_stmt->operand2 = max[counter+icounter];
                    icounter++;
                    break;
            }
            tok = lexer.GetToken();
        }
    }
    return stmt;
}

struct StatementNode* parse_while()
{
    struct StatementNode* stmt = new  StatementNode;
    struct StatementNode* end = new StatementNode;
    struct StatementNode* loop = new StatementNode;
    loop->type = GOTO_STMT;
    end->type = NOOP_STMT;
    stmt->type = IF_STMT;
    stmt->if_stmt = new IfStatement;
    stmt->next = end;
    loop->next = NULL;

    end->next = NULL;
    loop->goto_stmt = new GotoStatement;
 
    Token tok = lexer.GetToken();
    if (tok.token_type == WHILE)
    {

        tok = lexer.GetToken();
        switch (tok.token_type)
        {
            case ID:
                 for(int i =0; i< counter; i++)
                 {
                      if(max[i]->name == tok.lexeme)
                 {
                      stmt->if_stmt->condition_operand1 = max[i];
                 }
                 }
                break;
            case NUM:
                max[counter+icounter] = new ValueNode{};
                chr = strdup((tok.lexeme).c_str());
                   max[counter+icounter]->value = atoi(chr);
                   max[counter+icounter]->name = "";
                   stmt->if_stmt->condition_operand1 = max[counter+icounter];
                   ++icounter;
                   break;
        }
        tok = lexer.GetToken();
          if(tok.token_type == LESS )
        {
        stmt->if_stmt->condition_op = CONDITION_LESS;
        }

        else if(tok.token_type == GREATER)
        {
        stmt->if_stmt->condition_op = CONDITION_GREATER;
        }

        else if(tok.token_type == NOTEQUAL)
        {
        stmt->if_stmt->condition_op =  CONDITION_NOTEQUAL;
        }

        tok = lexer.GetToken();
        switch (tok.token_type)
        {
                case ID:
                    for(int i =0; i< counter; i++)
        {
           if(max[i]->name == tok.lexeme)
           {
           stmt->if_stmt->condition_operand2 = max[i];
           }

        }
                   break;
                case NUM:
                  max[counter+icounter] = new ValueNode{};
                   chr = strdup((tok.lexeme).c_str());
                   max[counter+icounter]->value = atoi(chr);
                   max[counter+icounter]->name = "";
                   stmt->if_stmt->condition_operand2 = max[counter+icounter];
                   ++icounter;
                   break;
        }
        stmt->if_stmt->true_branch = parse_body();
        stmt->if_stmt->false_branch = end;
        loop->goto_stmt->target = stmt;
        struct StatementNode* current = stmt->if_stmt->true_branch;

        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = loop;
    }
    return stmt;
}

struct StatementNode* parse_print()
{
    struct StatementNode* stmt = new StatementNode;
    Token tok2 = lexer.GetToken();
    if (tok2.token_type == PRINT)
    {
        tok2 = lexer.GetToken();
        stmt->type = PRINT_STMT;
        stmt->print_stmt = new PrintStatement;
         for(int i =0; i< counter; i++)
        {
           if(max[i]->name == tok2.lexeme)
           {
           stmt->print_stmt->id = max[i];
           }
        }
    }
    tok2 = lexer.GetToken(); // SEMICOLON
    return stmt;
}

struct StatementNode* parse_if()
{
    struct StatementNode* stmt = new StatementNode;
    
    struct StatementNode* st_end = new StatementNode;
    st_end->type = NOOP_STMT;
    stmt->type = IF_STMT;
    stmt->if_stmt = new IfStatement;
    stmt->next = st_end;
    st_end->next = NULL;
    Token tok3 = lexer.GetToken();
    if (tok3.token_type == IF)
    {
        tok3 = lexer.GetToken(); //operand1
        switch (tok3.token_type)
        {
                case ID:
                    for(int i =0; i< counter; i++)
        {
           if(max[i]->name == tok3.lexeme)
           {
           stmt->if_stmt->condition_operand1 = max[i];
           }

        }
                   break;
                case NUM:
                   max[counter+icounter] = new ValueNode{};
                   chr = strdup((tok3.lexeme).c_str());
                   max[counter+icounter]->value = atoi(chr);
                   max[counter+icounter]->name = "";
                   stmt->if_stmt->condition_operand1 =max[counter+icounter];
                   ++icounter;
                   break;
        }
        tok3 = lexer.GetToken();
        if(tok3.token_type == LESS )
        {
        stmt->if_stmt->condition_op = CONDITION_LESS;
        }
        else if(tok3.token_type == GREATER)
        {
        stmt->if_stmt->condition_op = CONDITION_GREATER;
        }
        else if(tok3.token_type == NOTEQUAL)
        {
        stmt->if_stmt->condition_op =  CONDITION_NOTEQUAL;
        }
        tok3 = lexer.GetToken(); //operand2
        switch (tok3.token_type)
        {
                case ID:;
                    for(int i =0; i< counter; i++)
        {
           if(max[i]->name == tok3.lexeme)
           {
           stmt->if_stmt->condition_operand2 = max[i];
           }

        }
                   break;
                case NUM:
                   max[counter+icounter] = new ValueNode{};
                   chr = strdup((tok3.lexeme).c_str());
                   max[counter+icounter]->value = atoi(chr);
                  max[counter+icounter]->name = "";
                   stmt->if_stmt->condition_operand2 = max[counter+icounter];
                   ++icounter;
                   break;
        }
        stmt->if_stmt->true_branch = parse_body();
        stmt->if_stmt->false_branch = st_end;
        struct StatementNode* current = stmt->if_stmt->true_branch;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = st_end;
    }    
    return stmt;
}



struct StatementNode* parse_for()
{
  struct StatementNode* stmt = new StatementNode;
  struct StatementNode* noop = new StatementNode;
  struct StatementNode* goto_ = new StatementNode;
  struct StatementNode* stmt1;
  struct StatementNode* stmt2;
  struct StatementNode* st_list;
  struct StatementNode* temp;
  
  stmt->if_stmt = new IfStatement;
  stmt->type = IF_STMT;
  noop->type = NOOP_STMT;
  goto_->type = GOTO_STMT;
  stmt->next =noop;
  goto_ ->goto_stmt = new GotoStatement;
  goto_->goto_stmt->target = stmt;
  goto_->next = noop;
  Token tokfor = lexer.GetToken();   // For loop token
  tokfor= lexer.GetToken();
  stmt1 = parse_assign();
  stmt1 ->next = stmt;
  tokfor = lexer.GetToken();
  if(tokfor.token_type == NUM)
  {
    max[counter] = new ValueNode{};
    max[counter]->name = "";
    max[counter]->value = atoi((tokfor.lexeme).c_str());
    stmt->if_stmt->condition_operand1 = max[counter];
    counter++;
  }
  else if(tokfor.token_type == ID)
  {
    for(int i = 0; i< counter; i++)
    {
       if(max[i]->name == tokfor.lexeme)
       {
           stmt->if_stmt->condition_operand1 =max[i];
         
       }
    }
  }
  
  tokfor = lexer.GetToken();
  switch(tokfor.token_type)
  {
   case GREATER:
           stmt->if_stmt->condition_op = CONDITION_GREATER;
           break;
   case LESS:
           stmt->if_stmt->condition_op = CONDITION_LESS;
           break;
   case NOTEQUAL:
           stmt->if_stmt->condition_op = CONDITION_NOTEQUAL;
           break;    
  }
  
   tokfor = lexer.GetToken(); // second operand
   
   if(tokfor.token_type == NUM)
   {
    max[counter] = new ValueNode{};
     max[counter]->name = "";
    max[counter]->value = atoi((tokfor.lexeme).c_str());
    stmt->if_stmt->condition_operand2 =max[counter];
    counter++;
    
   }
   else if(tokfor.token_type == ID)
   {
     for(int i = 0; i < counter; i++)
     {
       if(tokfor.lexeme == max[i]->name)
       {
         stmt->if_stmt->condition_operand2 = max[i];
       }
     }

   }
   
   tokfor = lexer.GetToken();   //  ;
   stmt2 = parse_assign();   // assign stmt 2
   tokfor = lexer.GetToken();    // RPARCE
   
   st_list = parse_body();
   temp = st_list;
   while(temp->next != NULL)
   {
      temp = temp-> next;
   }
   temp->next = stmt2;
    stmt->if_stmt->false_branch = noop;

   stmt->if_stmt->true_branch = st_list;
  temp = stmt->if_stmt->true_branch;
  while(temp->next != NULL)
  {
    temp = temp->next;
  }
  temp->next = goto_;
  
  return stmt1;
}

struct StatementNode* parse_case(struct ValueNode* temp_operand1, struct StatementNode* temp)
{
   struct StatementNode* stmt = new StatementNode;
   stmt->if_stmt = new IfStatement;

   stmt->type = IF_STMT;
   
   stmt->if_stmt->condition_operand1 = temp_operand1;

   stmt->if_stmt->condition_op = CONDITION_NOTEQUAL;
   
   Token tok5 = lexer.GetToken();  // get CASE or DEFAULT
   
   switch(tok5.token_type)
   {
     case CASE:
        //read the operand2 
        tok5 = lexer.GetToken();
        max[counter] = new ValueNode{};
        max[counter] -> name = "";
        max[counter] -> value = atoi(tok5.lexeme.c_str());
        stmt->if_stmt->condition_operand2 = max[counter];
        counter++;
        break;
     case DEFAULT:
        //make no changes 
        stmt -> if_stmt -> condition_operand2 = temp_operand1;
        break;
       
     }
   tok5 = lexer.GetToken();  // get COLON ;
   
   stmt->if_stmt->false_branch = parse_body();
   struct StatementNode* temp2 = stmt->if_stmt->false_branch;
   while(temp2->next != NULL)
   {
       temp2 = temp2 -> next;
   }
   temp2->next = temp;
   
   tok5 = lexer.GetToken(); //check next TOKEN if CASE or DEFAULT
   if(tok5.token_type == DEFAULT|| tok5.token_type == CASE)
   {
      lexer.UngetToken(tok5);
      stmt->if_stmt->true_branch = parse_case(temp_operand1, temp);
   }
   else
   {
      lexer.UngetToken(tok5);
      stmt->if_stmt->true_branch = temp;
   }
   return stmt;
}

struct StatementNode* parse_switch()
{
  struct StatementNode* stmt;
  struct StatementNode* temp = new StatementNode;
  struct StatementNode* noop = new StatementNode;
  
  noop->type = NOOP_STMT;
  temp->type = GOTO_STMT;
  temp->goto_stmt = new GotoStatement;
  temp->goto_stmt->target = noop;
  
  Token tokswitch = lexer.GetToken();   // get the SWITCH
  struct ValueNode* temp_operand1;
  
  tokswitch = lexer.GetToken();
  if(tokswitch.token_type == ID)
  {
     for(int i = 0; i< counter; i++)
     {
       if(tokswitch.lexeme == max[i]->name)
       {
       temp_operand1 = max[i];
       }
     }
  }

  else if(tokswitch.token_type == NUM)
  {
    max[counter] = new ValueNode{};
     max[counter] -> name = "";
     max[counter] -> value = atoi(tokswitch.lexeme.c_str());
     temp_operand1 = max[counter];
     counter++;
  }

  else
  {
    lexer.UngetToken(tokswitch);
    return NULL;
  }
  
  
  tokswitch = lexer.GetToken(); //LBRACE
  
  stmt = parse_case(temp_operand1, temp);
  stmt -> next = noop;
  
  tokswitch = lexer.GetToken(); //RBRACE
  
  return stmt;

}

struct StatementNode* parse_stmt()
{
	struct StatementNode* stmt;
	Token tokp = lexer.GetToken();
	switch (tokp.token_type)
	{
		case ID:
			lexer.UngetToken(tokp);
			stmt = parse_assign();
			break;
		case PRINT:
			lexer.UngetToken(tokp);
			stmt = parse_print();
			break;
		case WHILE:
			lexer.UngetToken(tokp);
			stmt = parse_while();
			break;
		case IF:
			lexer.UngetToken(tokp);
			stmt = parse_if();
			break;
		case SWITCH:
			lexer.UngetToken(tokp);
			stmt = parse_switch();
			break;	
	    case FOR:
	        lexer.UngetToken(tokp);
	        stmt = parse_for();
	}
	return stmt;
}

struct StatementNode* parse_stmt_list()
{
    
    struct StatementNode *stmt1;
    struct StatementNode *stmt2;
    Token tok = lexer.GetToken();
    TokenType t_type = tok.token_type;

    lexer.UngetToken(tok);
    stmt1 = parse_stmt();
    tok = lexer.GetToken();

    if ((tok.token_type == ID) || (tok.token_type == WHILE) || (tok.token_type == PRINT) || (tok.token_type == IF) || (tok.token_type == SWITCH) || (tok.token_type == FOR))
    {
        lexer.UngetToken(tok);
        stmt2 = parse_stmt_list();
	    if (stmt1->type == IF_STMT || t_type == FOR)
	    {
            struct StatementNode*current = stmt1->next;
            while (current->type != NOOP_STMT)
            {
                current = current->next;
            }
            current->next = stmt2;
	    } else
	    {
	      stmt1->next = stmt2;
	    } 
    }
    else
    {
        lexer.UngetToken(tok);
    }
    return stmt1;
}

struct StatementNode* parse_body()
{
	struct StatementNode* stmt;
	Token tokb = lexer.GetToken(); //{

	if (tokb.token_type == LBRACE)
	{
		stmt = parse_stmt_list();  // parse stmt
	}

	tokb = lexer.GetToken(); //}
	return stmt;
}

struct StatementNode* parse_generate_intermediate_representation()
{
  parsing parse;
  parse.parse_id_list();
  return parse_body();
}