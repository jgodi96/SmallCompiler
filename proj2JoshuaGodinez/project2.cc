/*
 * Copyright (C) Mohsen Zohrevandi, 2017
 *
 * Do not share this file with anyone
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <map>//using map,set, and vector for containers
#include <set>
#include <vector>

#include <iterator> //used to traverse containers
#include <algorithm>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;
/*struct SortOrder
{
    bool operator ()(const string &first, const string &second)
    {
        int length1=first.length();
        int length2=second.length();
        if (length1==length2)
            return (first<second);

            return(length1<length2);
    }
};*/
class project2 {

public:
    typedef struct prodRule {
        Token LHS;
        vector<Token> RHS;

        prodRule(Token lh, vector<Token> rh) {
            LHS = lh;
            RHS = rh;
        }
    } production;

    vector<bool> uselessSymbols(set<string> , vector<production> ); //task2
    map<string, set<string> > First(set<string> , vector<production> ); //task3
    map<string, set<string> > Follow(set<string>, set<string>, vector<production> );//task4
};

/*
    //identifier is string type and value stored is an integer type.
    //allows custom keys to access data
    map<string,int> map; //key string type
    int tempArray[2] = {2,5};
string weapon="Weapon2";
map["weapon1"]=100;
map["weapon2"]=120;
cout<<"damage: "<<map[weapon]<<endl;
system("pause");
return 0;*/


vector<bool> project2::uselessSymbols(set<string> terminals, vector<production> productions)
{
    //useless if no derivation

    map<string, bool> generatingSymbols; //string identifier, bool value for generating symbols and reachable symbols
    map<string, bool> reachableSymbols;

    bool loop=true;
    vector<production> :: iterator it1;
    vector<Token> :: iterator it2;
    for(it1 = productions.begin(); it1 != productions.end(); it1++)
    { //loop setting it1 to false and it2 to true;

        generatingSymbols[it1->LHS.lexeme] = false;
        for(it2 = it1->RHS.begin(); it2 != it1->RHS.end(); it2++)
        {
            if(terminals.find(it2->lexeme) != terminals.end())
            {
                generatingSymbols[it2->lexeme] =true;
            }
        }
    }

    generatingSymbols["#"] = true;
    while(loop){

        loop= false; //break out of loop if complete
        for(it1 = productions.begin(); it1 != productions.end(); it1++)
        {

            if(generatingSymbols[it1->LHS.lexeme] == false)
            {
                bool add = true;
                for(it2 = it1->RHS.begin(); it2 != it1->RHS.end(); it2++)
                {
                    if(generatingSymbols[it2->lexeme] == false  || (it2->lexeme == it1->LHS.lexeme))
                    {
                        add = false;
                        break;
                    }
                }
                if(add)
                {
                    generatingSymbols[it1->LHS.lexeme] = true;
                    loop = true;
                }
            }
         }
     }
    loop= true; //set to true again to go into the loop

    vector<bool> rules(productions.size(), false);
    while(loop){

        loop = false;

        int i = 0;

        for(it1 = productions.begin(); it1 != productions.end(); it1++) //traverse through rules
        {
            if(it1 == productions.begin())
            {
                if(generatingSymbols[it1->LHS.lexeme])
                    reachableSymbols[it1->LHS.lexeme] = true;
            }

            if(reachableSymbols[it1->LHS.lexeme])
            {
                bool addition = true;
                for(it2 = it1->RHS.begin(); it2 != it1->RHS.end(); it2++)
                {
                    if(!generatingSymbols[it2->lexeme]){
                        addition = false;
                        break;
                    }
                }
                if(addition && (rules[i] == false)){
                    for(it2= it1->RHS.begin(); it2 != it1->RHS.end(); it2++){
                        reachableSymbols[it2->lexeme] = true;
                    }
                    reachableSymbols[it1->LHS.lexeme] = true;
                    rules[i] = true;
                    loop = true;
                }
            }
            i++;
        }
    }
    return rules;
}
map<string, set<string> > project2::First(set<string> terminals, vector<production> rules)
{

    map<string, set<string> > firstSets; //string identifier, set string for first sets
    set<string> firstRHS;

    vector<production> :: iterator itr1;
    vector<Token> :: iterator itr2;
    set<string> :: iterator itr3;
    set<string> :: iterator itr4;

    firstRHS.insert("#"); //empty string representation
    firstSets["#"] = firstRHS;

    for(itr3 = terminals.begin(); itr3 != terminals.end(); ++itr3) //traverse through terminals
    {
        set<string> firstRHS;
        firstRHS.insert(*itr3);
        firstSets[*itr3] = firstRHS;
    }
    bool loop = true;
    while(loop){
        loop = false;
        for(itr1 = rules.begin(); itr1 != rules.end(); ++itr1) //traverse through rules
        {
            set<string> firstRHS;
            bool addition = true;
            for(itr2 = itr1->RHS.begin(); itr2 != itr1->RHS.end(); ++itr2) //traverse through righ hand side
            {
                if(itr2 == itr1->RHS.begin() || addition){
                    addition = false;
                    firstRHS = firstSets[itr2->lexeme];
                    set<string> symbol = firstSets[itr1->LHS.lexeme];
                    for (itr3 = firstRHS.begin(); itr3 != firstRHS.end(); ++itr3) //first rhs reverse
                    { if (symbol.find(*itr3) == symbol.end())
                        {
                            if((*itr3).compare("#") != 0)
                            {
                                symbol.insert(*itr3);
                                loop = true;
                            }
                        }
                        firstSets[itr1->LHS.lexeme] = symbol;
                    }

                    if(firstRHS.find("#") != firstRHS.end())
                    {
                        if(itr2 == --(itr1->RHS.end())) //decrecment if end
                        {
                            firstSets[itr1->LHS.lexeme].insert("#");
                            for (itr4 = symbol.begin(); itr4 != symbol.end(); ++itr4)
                            {
                                firstSets[itr1->LHS.lexeme].insert(*itr4);
                            }
                            break;
                        }
                        addition = true;

                        continue; //force next iteration to take place
                    }
                }
                else{
                    break;
                }
            }
        }
    }
    return firstSets;
}

map<string, set<string> > project2:: Follow(set<string> terminals, set<string> nonterminals, vector<production> rules){
    map<string, set<string> > followSets; //string identifier, set string for follow set
    set<string> followRHS;
    set<string> :: iterator itr3;
    set<string> :: iterator itr4;
    vector<production> :: iterator itr1;
    vector<Token> :: reverse_iterator itr2;
    vector<Token> :: reverse_iterator itr5;

    map<string, set<string> > firstSet;
    firstSet = First(terminals, rules);

    followRHS.insert("$");           //EOF representation
    followSets[rules.begin()->LHS.lexeme] = followRHS;

    bool loop = true;
    while(loop){
        loop = false;
        for(itr1 = rules.begin(); itr1 != rules.end(); ++itr1)
        {
            set<string> followRHS;
            bool term = false;
            bool hasEmptySet = true;
            for(itr2 = (itr1->RHS).rbegin(); itr2 != (itr1->RHS).rend(); ++itr2)
            {
                if(terminals.find(itr2->lexeme) != terminals.end())
                    term = true;

                if(itr2 == itr1->RHS.rbegin() && nonterminals.find(itr2->lexeme) != nonterminals.end())
                {
                    followRHS = followSets[itr1->LHS.lexeme];
                    set<string> symbol = followSets[itr2->lexeme];
                    for (itr3 = followRHS.begin(); itr3 !=followRHS.end(); ++itr3)
                    {
                        if (symbol.find(*itr3) == symbol.end())
                        {
                            symbol.insert(*itr3);
                            loop = true;
                        }
                        followSets[itr2->lexeme] = symbol;
                    }
                }
                bool emptySet = true;
                bool followAdd = false;

                int ruleit = 0;
                for(itr5 = itr2; itr5 != itr1->RHS.rend(); ++itr5)
                {
                    if(firstSet[itr2->lexeme].find("#") != firstSet[itr2->lexeme].end())
                        followAdd = true;
                    if(emptySet && ruleit > 0)
                    {
                        emptySet = false;
                        set<string> symbol2 = followSets[itr5->lexeme];
                        if(itr5 != itr1->RHS.rend() &&  !firstSet[itr2->lexeme].empty())
                        {
                            if(followAdd)
                            {
                                followRHS = followSets[itr1->LHS.lexeme];
                                for (itr4 = followRHS.begin(); itr4 !=followRHS.end(); ++itr4)
                                {
                                    if (symbol2.find(*itr4) == symbol2.end() && term == false && hasEmptySet)
                                        symbol2.insert(*itr4);

                                }
                                followAdd = false;
                            }
                            for(itr3 = firstSet[itr2->lexeme].begin(); itr3 != firstSet[itr2->lexeme].end(); ++itr3)
                            {
                                if(*itr3 != "#" && symbol2.find(*itr3) == symbol2.end())
                                {
                                    symbol2.insert(*itr3);
                                    loop = true;
                                }
                            }
                            followSets[itr5->lexeme] = symbol2;
                        }
                        if(firstSet[itr5->lexeme].find("#") != firstSet[itr5->lexeme].end())
                            emptySet = true;
                        else
                            hasEmptySet = false;

                    }
                    else if(ruleit == 0)
                        emptySet = true;

                    ruleit++;
                 }
            }
        }
    }
    return followSets;
}

int main (int argc, char* argv[])
{
    int task;

    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }

    task = atoi(argv[1]);

    LexicalAnalyzer lexer;
    project2 projobj;

    vector<project2::production> prodrules;
    set<string> terminalSet;
    set<string> nonterminalSet;
    set<string> lexemesSet;
    vector<string> terminalVec;
    vector<string> nonterminalVec;
    vector<string> lexemes;



    Token token = lexer.GetToken();

    while (token.token_type != DOUBLEHASH)
    {
        vector<Token> rhsTokens;

        if(lexemesSet.find(token.lexeme) == lexemesSet.end()){
            lexemes.push_back(token.lexeme);
            lexemesSet.insert(token.lexeme);
        }

        Token token2 = lexer.GetToken();

        if(token2.token_type == ARROW){
            token2 = lexer.GetToken();
        }

        while(token2.token_type != HASH){
            rhsTokens.push_back(token2);

            if(lexemesSet.find(token2.lexeme) == lexemesSet.end()){
                lexemes.push_back(token2.lexeme);
                lexemesSet.insert(token2.lexeme);
            }

            token2 = lexer.GetToken();
        }
        if(rhsTokens.size() == 0){
            token2.lexeme = "#";
            rhsTokens.push_back(token2);
        }
        prodrules.push_back(project2::prodRule(token, rhsTokens));
        token = lexer.GetToken();
    }
    vector<string> :: iterator it1;
    vector<project2::production> :: iterator it2;
    for (it1 = lexemes.begin(); it1 != lexemes.end(); it1++) {
        bool isNonterminal = false;
        for (it2 = prodrules.begin(); it2 != prodrules.end(); it2++) {
            if ((it2->LHS).lexeme.compare(*it1) == 0) {
                isNonterminal = true;
                break;
            }
        }

        if (isNonterminal) {
            nonterminalVec.push_back(*it1);
            nonterminalSet.insert(*it1);
        } else {
            terminalVec.push_back(*it1);
            terminalSet.insert(*it1);
        }
    }

    switch (task) {
        case 1:{
            /*set <string, SortOrder> names;
typedef set <string> iterator it;
names.insert("peter"); //sets cannot have same name
names.insert("coding made easy");

for (iterator = names.begin(); iterator != names.end(); iterator++)
    cout<<"iterator" <<endl;
    //user pointer to print iterator
    */
            vector <string> :: iterator itNonterminal;
            vector <string> :: iterator itTerminal;

            for (itNonterminal = nonterminalVec.begin(); itNonterminal != nonterminalVec.end(); ++itNonterminal)
            {
                cout << *itNonterminal << "\t";
            }
            for (itTerminal = terminalVec.begin(); itTerminal != terminalVec.end(); ++itTerminal)
            {
                cout<<*itTerminal << "\t" ;
            }
            cout<< "\n";
            break;
        }
        case 2:{
            vector<bool> Rules = projobj.uselessSymbols(terminalSet, prodrules);

            for(int i = 0; i < Rules.size(); i++){
                if(Rules[i]){
                    cout << prodrules[i].LHS.lexeme << "\t->\t";
                    for(int j = 0; j < prodrules[i].RHS.size(); j++){
                        cout << prodrules[i].RHS[j].lexeme << "\t";
                    }
                    cout << "\n";
                }
            }
            break;
        }

        case 3:{
            vector<string> :: iterator it1;
            set<string> :: iterator it2;
            vector<string>::iterator terminalIterator;

            map<string, set<string> > aFirstSet;
            aFirstSet = projobj.First(terminalSet, prodrules);

            for(it1 = nonterminalVec.begin(); it1 != nonterminalVec.end(); ++it1){
                cout << "FIRST(" << *it1 << ") = { ";

                int printCount = 0;

                if (aFirstSet[*it1].find("#") != aFirstSet[*it1].end()) {
                    if(printCount == aFirstSet[*it1].size()-1){
                        cout << "#";
                    }
                    else{
                        cout << "#" << ", ";
                        printCount++;
                    }
                }

                for (terminalIterator = lexemes.begin(); terminalIterator != lexemes.end(); ++terminalIterator) {
                    if (aFirstSet[*it1].find(*terminalIterator) != aFirstSet[*it1].end()) {
                        if(printCount == aFirstSet[*it1].size()-1){
                            cout << *terminalIterator;
                        }
                        else{
                            cout << *terminalIterator << ", ";
                            printCount++;
                        }
                    }
                }
                cout << " }\n";
            }
            break;
        }
        case 4:{

            vector<string> :: iterator itr1;
            vector<string> :: iterator itr2;

            map<string, set<string> > aFollowSet;
            aFollowSet = projobj.Follow(terminalSet, nonterminalSet, prodrules);
            lexemes.insert(lexemes.begin(), "$");

            for(itr1 = nonterminalVec.begin(); itr1 != nonterminalVec.end(); ++itr1){
                cout << "FOLLOW(" << *itr1 << ") = { ";
                int i = 0;
                for(itr2= lexemes.begin(); itr2 != lexemes.end(); ++itr2){
                    if(aFollowSet[*itr1].find(*itr2) != aFollowSet[*itr1].end()){
                        if(i == aFollowSet[*itr1].size()-1){
                            cout << *itr2;
                        }
                        else{
                            cout << *itr2 << ", ";
                        }
                        i++;
                    }
                }
                cout << " }\n";
            }
            break;
        }
        case 5:{
            set<string> :: iterator it1;
            set<string> :: iterator it2;
            vector<Token> :: iterator it4;
            vector<Token> :: iterator it5;

            vector<bool> Rules = projobj.uselessSymbols(terminalSet, prodrules);
            map<string, set<string> > FirstSet = projobj.First(terminalSet, prodrules);

            map<string, set<string> > FollowSet = projobj.Follow(terminalSet, nonterminalSet, prodrules);

            bool predictiveParser= true;

            for(int i1 = 0; i1 < Rules.size(); i1++) //start parse to see if there is no backtrack
            {
                for(int i2 = 0; i2 < Rules.size(); i2++)
                {
                    if(!Rules[i1]){
                        predictiveParser = false;
                        break;
                    }
                    bool emptyString = true;
                    if(prodrules[i1].LHS.lexeme == prodrules[i2].LHS.lexeme && i1 != i2)
                    {
                        for(it4 = prodrules[i1].RHS.begin(); it4 != prodrules[i1].RHS.end(); ++it4)
                        {
                            for(it5 = prodrules[i2].RHS.begin(); it5 != prodrules[i2].RHS.end(); ++it5)
                            {
                                if(FirstSet[it5->lexeme].find(it4->lexeme) != FirstSet[it5->lexeme].end())
                                {
                                    predictiveParser= false;
                                    break;
                                }
                                if(FirstSet[it5->lexeme].find("#") == FirstSet[it5->lexeme].end())
                                {
                                    emptyString = false;
                                    break;
                                }
                            }
                            if(!emptyString)
                                break;
                        }
                    }
                    for(int i = 0; i < prodrules[i1].RHS.size(); i++)
                    {
                        if(prodrules[i1].RHS[i].lexeme == ("#")){
                            for(it1 = FirstSet[prodrules[i1].LHS.lexeme].begin(); it1 != FirstSet[prodrules[i1].LHS.lexeme].end(); ++it1)
                            {
                                for(it2 = FollowSet[prodrules[i1].LHS.lexeme].begin(); it2 != FollowSet[prodrules[i1].LHS.lexeme].end(); ++it2)
                                {
                                    if(*it1 == *it2)

                                    { predictiveParser = false; //set to false if you can backtrack
                                        break; }
                                 }
                             }
                        }
                     }
                 }
              }
              if(predictiveParser) //if has a predictive parse
                cout << "YES\n";
            else
                cout << "NO\n";
            break;
        }
        default:
            cout << "Error: unrecognized task number " << task << "\n";
            break;
    }
    return 0;
}