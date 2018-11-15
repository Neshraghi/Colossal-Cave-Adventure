

#define	PLAYER_H
#include <iostream>
#include <syslog.h>
#include "Socket.h"
#include "Socket.cpp"
#include <cstdlib>
#include <cctype>
#include <signal.h>
#include <vector>
#include <string.h>
#include <map>
#include <fstream>
using namespace std;

#define SEP -1
#define ENDLINE "-*-"
#define LOC 142
#define VERB 50


#define LONGDESSECT       0pr
#define SHORTDESSECT      1
#define TRAVELTAB            2
#define VOC              3
#define ELEMENTDES            4
#define ELEMENTLOC        6
#define ABBRMSG                5
#define ACT                 7
#define LIQUIDASSET            8
#define CLASSMSG               9
#define HINTS                   10
#define MAGICWORDS             11

#define HOST "172.0.0.1"
#define PORT 3302

////////////////////////ELEMENT////////////////
class Element {
public:
    int id;
	
    int num;
    bool  moveable;
    string desc;
    string mess[6];
    
    
    Element(string desc);
    ~Element();



};

///////////////////////NODE//////////////


class Node{

public:
	int id;
	int num;
    int item_need;
	int directions[10];
    int active_verbs[10][10];
    string clue;
    string short_clue;
    
        vector<Element*> items;
    

    Node(string clue);
    ~Node();

 string getclue();
void append(string);
    void setclue(string clue);
    void setshortclue(string short_clue);

   

    
};




////////////////////////////////////////////////////////////PLAYER///////////

class Player {

private:
    
    int score;
	int curritem;
    int currlocindex;
    Node* loc;
string name;
 bool checkloc(int indx);
    int propval(int m);   

public:
int max_items;
    string ans;
    string verb;
    string noun;
    int moves;
    int message_id;
    vector<Element * > carrying;

    Player(string name,int score, int loc);

    ~Player();

    
    int  getcurrloc();

    int  getscore();
	int getitem();
	void setcurrloc(int loc);
    void setscore(int score);
	void gotonextloc();

    
    void takeitem(int element_id);
    
    void setloc(Node* loc);

    void setitem(int item);
    
	string getinfo();
    string getinput();
    Node* getloc();
    string formatans();
    

};
//////////////////////parser//////////

class Parser {

private:
     int getFirstNumberOf(string &line);
public:
	size_t firstindex;
    size_t lastindex;
    int elementnum;
    int actverb[32];
    string abbr_mess[202];
    string fileData;
    Node* locations[LOC] ;
    Element* elements[65];
    map <string, int> vocabulary;


//funcation
    Parser(string file);
    ~Parser();
    void parseloc(string &line, int firstnumber=0);
    void parsevacb(string &line);
    void parsetabletrav(string &line, int firstnumber=0);
    void parseelementdesc(string &line);
    void parseelementloc(string &line);
    void parseact(string &line);
    void parseplayerclassify(string &line);
    void parseabbrmsg(string &line);


    bool is_object(string key);
    bool is_special(string key);
    bool is_motion(string key);
    bool is_action(string key);
    string tokenizer(string &line , string d="\t");
 
};
////////////////////////ELEMENT///////////////////

Element::Element(string desc="")
{
   desc=desc;
   moveable = true;
   num = 0;
}

/////////////////NODE//////////////////////////

Node::Node(string clueword)
{
    clue = clueword;
    num = 0;
}



void Node::setclue(string clue)
{
    this->clue = clue;

}
string Node::getclue()
{
    return this->clue;
}
void setshortclue(string shortclue)
{
}

void Node::append(string txt)
{
    this->clue = this->clue + txt;
}


Node::~Node()
{
}
//////////////////////PLAYER///////////////////

Player::Player(string name, int score, int loc)
{
    this->name = name;
    this->score = score;
    this->currlocindex = loc;
    this->message_id=0;
}



string Player::getinput()
{
    return ans;
}

void Player::setcurrloc(int loc)
{
    this->currlocindex = loc;
}

int Player::getcurrloc()
{

    return this->currlocindex;
}

void Player::setitem(int item)
{
    curritem = item;
}

int Player::getitem()
{
    return curritem;
}


void Player::setscore(int score)
{
    this->score  = score;
}




int Player::getscore()
{
    return this->score;
}

string Player::getinfo()
{
    return name;
}

Node* Player::getloc()
{
    return this->loc;
}

void Player::setloc(Node* loc)
{
    this->loc = loc;
}


void Player::gotonextloc()
{

    loc->directions;
    loc->active_verbs;

    for(int i=0 ; i < 10; i++){
        if(loc->directions[i] > 0){
            for(int j=0 ; j < 10 ; j++){
                if(loc->active_verbs[i][j] > 0){
                    if( loc->active_verbs[i][j]== moves){
                        if(checkloc(i)){
                            setcurrloc(this->currlocindex);
                        }
                    }
                }
            }
        }
    }
}

void Player::takeitem(int item_id)
{

    int item_size = this->loc->items.size();
    item_id = item_id % 1000;
    for(int i=0 ; i < item_size; i++){
        if(this->loc->items[i]->id == item_id){
            this->carrying.push_back(this->loc->items[i]);
            break;
        }
    }
    this->noun=""; //setting to empty to fallback to if-else ladder
}

bool Player::checkloc(int indx)
{

        
	int x;
        int y;
        bool status = false;

	int n; 
    int m ;
	
        y = this->loc->directions[indx];

        m = y / 1000;
        n = y % 1000;

        cout << y << "\t n:" << n << "\t m:" << m << endl;

        if(n <= 300)

	{
            if (m==0) 
		{
                this->currlocindex = y;// jump
                status = true;
            }
            else if((m>0) and (m<100))
		{
                cout << "what to do with probability" << endl;
                //rand(m % 100);
            }
            else if((m>100) and (m<=200))
		{
		
                cout << "Please take object :) " << endl;   // must be carrying an Element=M-100
                int item_id= m-100;
                int item_size = this->carrying.size();

                cout << item_size << "\t" << item_id << endl;
                if(item_size > 0){
                  for(int i=0; i < item_size ; i++){
                   if(this->carrying[i]->id == item_id ){
                        this->currlocindex = y;
                        status = true; // make the jump
                    }
                  }
                }
            }
            else if((m>200) and (m <= 300))
		{
                cout << "R00M" << (m- 200) << endl;  // must carry element and same room=M-200
                int item_id= m-200;
                int item_size = this->carrying.size();

                cout << item_size << "\t" << item_id << endl;
                if(item_size > 0){
                  for(int i=0; i < item_size ; i++){
                   if(this->carrying[i]->id == item_id ){

                      this->currlocindex = n;
                      status = true; // takE A jump
                    }
                  }
                }

            }
		else if((m >300) && (m <=400) && (propval(m) != 0))
		{
                cout << "Prop value not 0" <<endl;
                this->currlocindex = n;
                status=true;

                }
		else if((m>400)  && (m <=500) && (propval(m)!=1))
		{
                cout << "Prop value not 1" << endl;
                this->currlocindex = n;
                status=true;

               } ///check 1
		else if((m> 500) && (m <=600) && (propval(m)!=2))
		{
                cout << "Prop value not 2" << endl;
                this->currlocindex = n;
                status=true;
                }
		else
		{
                
                cout << "Not matched" << endl;    //default condition
                this->currlocindex = this->loc->directions[indx+1];
                status=true;
            }

        }
        else if( (n > 300) && (n <=500) ) {
            
            status =false;  //whn  greater than 300 ;
        }
        else
        {
            
            this->message_id=(n - 500);   // n-500 to print section 6
            status= false;
        }


        return status;
}






string Player::formatans() 
{
    int len = verb.length() - 2;
    verb.erase(len);
    for (int i = 0; i !=len ; i++)
    {
        verb[i] = toupper(verb[i]);
    }
    return verb;
}
int Player::propval(int m)
{
    return m % 100 ;
}
Player::~Player(){}



//////////////////////////////PARSER/////////////////////////////

Parser::Parser(string fileName)
 {

    
    ifstream dataFile;
    string line;
    string data;
    firstindex = 0;
    elementnum=0;

    elements[17]=new Element(">$<"); // no object desc
    elements[18]=new Element(">$<");


    for(int i=0 ; i < 141; i++) 
    locations[i] = NULL; 

    int section = 0;

    dataFile.open(fileName.c_str());

    if(!dataFile.eof())
    {
        while(getline(dataFile,line))
	{
            data += line + ENDLINE;

            int first_number = getFirstNumberOf(line);

            if(SEP == first_number)
	    {
               section++;
            }

            switch(section)
	    {

				case VOC:
                    parsevacb(line);
                    break;
                case ELEMENTDES:
                    parseelementdesc(line);
                    break;
                case ELEMENTLOC:
                    parseelementloc(line);
                    break;
                case ABBRMSG:
                    parseabbrmsg(line);
                    break;
                case LONGDESSECT:
                    parseloc(line, first_number);
                    break;
                case SHORTDESSECT:
                    break;
                case TRAVELTAB:
                    parsetabletrav(line, first_number);
                    break;

		    
                case LIQUIDASSET:
                    break;
                case CLASSMSG:
                    parseplayerclassify(line);
                    break;
                case HINTS:
                    break;
                case MAGICWORDS:
                    break;
               
                case ACT:
		parseact(line);
                    break;
                
            }
        }
        dataFile.close();
    }

}





int Parser::getFirstNumberOf(string &line1)
{
    size_t tab_start = line1.find_first_of("\t");
    return atoi (line1.substr(0, tab_start).c_str());
}



void Parser::parseloc(string &line, int first_number )
{

    size_t tab_ends = line.find_first_of("\t") + 1; //END  TAB CHAR

    string direction = line.substr( tab_ends);
    if(!direction.empty()){
        if(locations[first_number]==NULL){
            locations[first_number]= new Node(direction);
            locations[first_number]->id = first_number;
        }else{
            locations[first_number]->append(direction);
        }
    }
}






/////important method parses for most important table
void Parser::parsetabletrav(string &line, int first_number)
{

    string temp1[11];

    for(int i=0 ; i < 12 ; i++)
    temp1[i] = tokenizer(line);

    firstindex=0;


    if(!temp1[1].empty())
    {
        //now table parsing beings
        int y = atoi(temp1[1].c_str());

        //changinf this array this to array no need for vector here
        locations[first_number]->directions[locations[first_number]->num]=y;


        for (int i = 2; i < 10; i++)
	 {
            locations[first_number]->active_verbs[locations[first_number]->num][i-2]= atoi(temp1[i].c_str());
        }

        locations[first_number]->num++;

    }//ifEnds
}

void Parser::parseabbrmsg(string &line2)
{
    string temp2[2];
    for(int i=0 ; i < 2 ; i++) temp2[i] = tokenizer(line2);
    firstindex=0;
    int indx = atoi(temp2[0].c_str());
    if(!temp2[1].empty())
    {
        abbr_mess[indx] += temp2[1];
    }
}

void Parser::parsevacb(string &line)
{

    size_t tab = line.find_first_of("\t");
    int value  = atoi(line.substr(0,tab).c_str());
    string key = line.substr(tab+1);

    if(key!="4") //removing the section number
        this->vocabulary[key]=value;
}

void Parser::parseelementdesc(string &line)
{
    
    string temp[2];
    for(int i=0 ; i < 2 ; i++) temp[i] = tokenizer(line);
    firstindex=0;

    int element_id = atoi(temp[0].c_str());
    if(!temp[1].empty()){
       
        if((element_id >0) && (element_id < 65)){
            elements[element_id]= new Element(temp[1]);
            elementnum=element_id;
            elements[element_id]->id = element_id;
        }else{
		
            
            
            elements[elementnum]->mess[element_id/100] = temp[1];//element_id is three DIGIT number--adds descriptions blocks with index
        }

    }
}



void Parser::parseelementloc(string& line)
{
    string temp[3];
    for(int i=0 ; i < 3 ; i++)
    temp[i] = tokenizer(line);
    firstindex=0;

    int num=0;

    if(!temp[1].empty())
      {
        int element_id = atoi(temp[0].c_str());
        int loc_idx = atoi(temp[1].c_str());
        int secon_loc_idx = atoi(temp[2].c_str());

        if(element_id > 0 && element_id != 17 && element_id !=18) 
	{
            
            if(loc_idx>0){
                locations[loc_idx]->items.push_back(elements[element_id]);
            }
            if(secon_loc_idx>0){
                locations[secon_loc_idx]->items.push_back(elements[element_id]);
            }

        }
        loc_idx = 0;
        secon_loc_idx=0;
    }
}


void Parser::parseact(string& line)
{
    string temp[2];
    for(int i=0 ; i < 2 ; i++) 
    temp[i] = tokenizer(line);
    firstindex=0;

   if(!temp[1].empty())
   {
        actverb[atoi(temp[0].c_str())] = atoi(temp[1].c_str());
   }
}



void Parser::parseplayerclassify(string& line){
    string temp[2];
    for(int i=0 ; i < 2 ; i++) temp[i] = tokenizer(line);
    firstindex=0;

     
}



///cantain all logic of the game

bool Parser::is_motion(string key)
{
    return this->vocabulary[key]/1000 == 0 ? true : false;
}
bool Parser::is_action(string key)
{
    return this->vocabulary[key]/1000 == 2 ? true : false;
}
string Parser::tokenizer(string &line, string delimiter)
{
    
    if(firstindex == string::npos) return "";               // return a line using delimeter with tab
    lastindex = line.find_first_of(delimiter , firstindex);
    string tokn(line.substr(firstindex, lastindex- firstindex));
    firstindex = line.find_first_not_of(delimiter, lastindex);
    return tokn;
}

bool Parser::is_special(string key)
{
    return this->vocabulary[key]/1000 == 3? true : false;
}

bool Parser::is_object(string key)
{
    return this->vocabulary[key]/1000 == 1 ? true : false;
}




Parser::~Parser()
{

}



//////////////////MAIN//////////////////////

int exit_with_error(string msg){
    cout << msg << endl;
    return EXIT_FAILURE;
}



int main() {

    string name ="\n          START	..\n";
    cout << name <<endl;

    Parser game("advent.dat");


    string client_say="";
    Socket server;
    int client_num = 0;

    if(!server.create())    return exit_with_error(" creating Error ");
    if(!server.bind(PORT))  return exit_with_error(" binding Error  ");
    if(!server.listen())    return exit_with_error(" listenting Error");

    while(true)
	{
      Socket client;
      client.create();
      if(server.accept(client)){
        int pid = fork();
        if( pid > 0 )
	{


          Player player("Player 1", 0 , 1);
          map<string, int>::iterator it;
          string words[3];
          bool show_item= true;


          client.send(name + "\n");

          while(true)
	  {

              //this is a socket message
              client.send(game.locations[player.getcurrloc()]->getclue() + "\n");
              player.setloc(game.locations[player.getcurrloc()]);



              
              int items_no = player.getloc()->items.size(); ///checking the loc player
              int items_take = player.carrying.size();

              if(items_no > 0)
		{
                  for (int i = 0; i < items_no; i++)
		 {
                      for(int j=0; j< items_take ; j++)
			{
                        if(player.carrying[j]->id == player.getloc()->items[i]->id)
			{
                          show_item=false; break;
                        }
                      }
                      if(show_item)
			{
                        client.send("--> " + player.getloc()->items[i]->mess[0] + "\n" ) ;
                      }
                      show_item = true;
                  }
              }

              
              client.send("---> ") ;// handling input
             
              client.recv(player.verb);
              player.formatans();

              

              game.firstindex=0;
              for(int i=0; i<3 ;i++)
	      {
                  words[i] = game.tokenizer(player.verb," ");
              }

              !words[0].empty() ? player.verb = words[0] : "";
              !words[1].empty() ? player.noun = words[1] : "";

              if(player.verb == "Q")
	      {
                client.close();
                break;
              }
              
              if(!player.verb.empty() && !player.noun.empty()){
                  it = game.vocabulary.find(player.verb);
                  if(it!=game.vocabulary.end())
			{
                      if(game.is_action(player.verb) && game.is_object(player.noun))   /////player take element
			{
                        
                        int element_id = game.vocabulary.find(player.noun)->second;
                        player.takeitem(element_id);
                      }
                  }

              
              }
		else if(!player.verb.empty())
		{
                  it = game.vocabulary.find(player.verb);   //player move only 
                  if(it!=game.vocabulary.end())
			{
                    if(game.is_motion(player.verb))
			{

                       player.moves = game.vocabulary.find(player.verb)->second;
                       
                       player.gotonextloc();

                       if(player.message_id>0)
			{
                          client.send("==> "+ game.abbr_mess[player.message_id] + "\n");
                          player.message_id=0;
                       }
                    }
                  }
		else
		  {
                      cout << "What ??" << endl;
                  }
              }
          }
        }
      }
    }

    return EXIT_SUCCESS;
}
