#include<iostream>
#include<list>
#include<typeinfo>
#include <random>
#include "time.h"
#include <string>
using namespace std;

class Character;

class Person{

protected:

int hitpoints;

public:

Character *has_character ;

Character& get_has_character();

void hit( Person *p);

void set_hp(int hp){
  hitpoints=hp;
}

Person();

int get_hp(){
 return hitpoints;
}

bool is_dead = false;

};

class Character{

public:

Person *is_attached_to;

string get_character_type(Person *p);

virtual void attack (Person *p)=0;

void lower_hp(){

    (this->is_attached_to)->set_hp(((this->is_attached_to)->get_hp())-10);

    if (((this->is_attached_to)->get_hp())==0){
                                                (this->is_attached_to)->is_dead = true;
    }
}

void convert(Person *p);
};

class Zombie : public Character {

void attack(Person *p){

    if(this->get_character_type(p)=="3Bad"){
        (p->has_character)->lower_hp();
        (p->has_character)->convert(this->is_attached_to);
    }

    if(this->get_character_type(p)=="4Good"){
        (p->has_character)->lower_hp();
        (p->has_character)->convert(this->is_attached_to);
    }
}
};

class Good : public Character {

void attack(Person *p){

    if(this->get_character_type(p)=="3Bad"){
        (p->has_character)->lower_hp();
    }

    if(this->get_character_type(p)=="6Zombie"){
        (p->has_character)->lower_hp();
        (p->has_character)->convert(this->is_attached_to);
    }
}
};

class Bad : public Character {

void attack(Person *p){
    if(this->get_character_type(p)=="4Good"){
        (p->has_character)->lower_hp();
    }
    if(this->get_character_type(p)=="6Zombie"){
        (p->has_character)->lower_hp();
    }
}
};

void Person :: hit(Person *p){
    this->has_character->attack(p);
}

Person :: Person(){

    hitpoints=100;
    int x=rand()%9;

    if (x<=2){
        this->has_character = new Zombie;
    }

    if (x>=3 && x<=5){
        this->has_character = new Good;
    }

    if (x>=6 && x<=8){
        this->has_character = new Bad;
    }
}

Character& Person :: get_has_character(){
  return *has_character;
}

class World{

private:

int players;
int rounds;
list<Person*> myList;

public:

list<Person*>::iterator di,po,it;

bool telos = false;

World (int p = 100 ){

 players = p;
 rounds = 1;
 Person *o;

 for (int i = 1;i <= players;i ++){
    o = new Person();
    myList.push_back(o);
 }

  for(di = myList.begin();di != myList.end();di ++){
    ((*di)->has_character)->is_attached_to = *di;
  }
}
void arxh(){

    cout<<"[ ";

    for(it = myList.begin(); it != myList.end(); it++) {
    cout <<((*it)->has_character)->get_character_type((((*it)->has_character))->is_attached_to)<<'-'<<(*it)->get_hp()<<' ';
    }

    cout <<']'<<endl;
}
void round(){

   for(di = myList.begin(),po = myList.begin(),++po;po != myList.end();di++,po++){
     if(((*di)->is_dead)==0) (*di)->hit((*po));
  }

    po = myList.end();

    --po;

    if(((*po)->is_dead)==0) (*po)->hit(*(myList.begin()));

    myList.remove_if([](const Person *p){ return(p->is_dead); });

    int x=0,y=0,z=0;

    cout<<"[ ";

    for(it = myList.begin(); it != myList.end(); it++) {
        cout <<((*it)->has_character)->get_character_type((((*it)->has_character))->is_attached_to)<<'-'<<(*it)->get_hp()<<' ';
        if(((*it)->has_character)->get_character_type(((*it)->has_character)->is_attached_to)=="4Good") x++;
        if(((*it)->has_character)->get_character_type(((*it)->has_character)->is_attached_to)=="3Bad" ) y++;
        if(((*it)->has_character)->get_character_type(((*it)->has_character)->is_attached_to)=="6Zombie" ) z++;
    }

    cout <<']'<<endl;

    if(x==0 && y==0 && z==0){
        cout<<"Ισοπαλία";
        telos =  true;
    }

    else{
            if(x==0 && y==0){
                telos =  true;
                cout<<"Νίκησαν οι Zombie";
            }

            if(z==0 && x==0){
                telos =  true;
                cout<<"Νίκησαν οι Bad";
            }

            if(z==0 && y==0){
                telos =  true;
                cout<<"Νίκησαν οι Good";
            }
    }



    if(rounds==100){
        telos =  true;
        if (x>y && x>z){ cout<<"Νίκησαν οι Good";}
        if (y>x && y>z){ cout<<"Νίκησαν οι Bad";}
        if (z>y && z>x){ cout<<"Νίκησαν οι Zombie";}
    }
 ++rounds;
}
};

string Character :: get_character_type(Person *p){
    return typeid(p->get_has_character()).name();
}

void Character :: convert(Person *p){

    int x=rand()%100,y=rand()%100;

    if (((this->get_character_type(p)) == "6Zombie") && ((this->get_character_type(this->is_attached_to) == "4Good")
       ||(this->get_character_type(this->is_attached_to) == "3Bad")) && x<=9){
        is_attached_to->has_character = new Zombie;
        Character *new_char = is_attached_to->has_character ;
        new_char->is_attached_to = this->is_attached_to;
        delete this;
       }

    if ((this->get_character_type(p) == "4Good") && ((this->get_character_type(this->is_attached_to))== "6Zombie")){
        if(y<=19){
            if (x<=39) {
                is_attached_to->has_character= new Good;
                Character *new_char = is_attached_to->has_character ;
                new_char->is_attached_to = this->is_attached_to;
                delete this;
            }

            else {
                is_attached_to->has_character= new Bad;
                Character *new_char = is_attached_to->has_character ;
                new_char->is_attached_to = this->is_attached_to;
                delete this;
            }
                                                                             }
    }
}

int main(){

srand (time(NULL));

int p=100;

cin >> p;

World  agwnas(p);

agwnas.arxh();

while(agwnas.telos == false){
 agwnas.round();
}
}
