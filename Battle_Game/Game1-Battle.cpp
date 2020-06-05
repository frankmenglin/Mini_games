// classes example
#include <iostream>
#include <math.h>
#include <random>
#include <ctime>
using namespace std;

class Player {
    int HP, MP;
    string Name;
  public:
    void set_values (int,int,string);
    void display() {
        cout<< Name <<endl;
        cout<<"HP: "<< HP <<endl;
        cout<<"MP: "<< MP <<endl;
        }
    string getName() {
        return Name;
    }
    int getHP() {
        return HP;
    }
    int getMP() {
        return MP;
    }
    void defense() {
        HP--;
    }
    void attack() {
        MP=0;
    }
    void store_power() {
        if (MP>=6){
            HP-=3;
        }
        MP++;
    }
    void get_attacked(int h) {
        HP=max(HP-int(pow(2,h)),0);
    }
    void suicide(){
        HP=0;
    }
};

void Player::set_values (int x, int y, string z) {
  HP = x;
  MP = y;
  Name = z;
}

void display_rule() {
  cout << "This is a battle game between you and the computer (enemy)." << endl;
  cout << "Each of you start with 100HP and 0MP, the one that runs out of HP loses the game (draw if on the same turn)." << endl;
  cout << "On each turn, you and enemy each chooses an action simultaneously between the followings: attack, defense, store power." << endl;
  cout << "If you defense, you lose 1 HP regardless of enemy's action." << endl;
  cout << "If you store power, you gain 1MP regardless of enemy's action. If you start the turn with MP=6 or more, you lose 3HP to store power." << endl;
  cout << "If you attack and enemy does not defense, then enemy lose 2^(your MP) amount of HP (if enemy defense then enemy only loses 1HP)." << endl;
  cout << "Your MP is reset to 0 after you attack, regardless of enemy's action." << endl;
  cout << "The effect of enemy's choice is the same as yours with the roles swap." << endl;
}

void user_choice_this_turn(int Turn_counter) {
    cout<<" "<<endl;
    cout<<"Turn "<<Turn_counter<<endl;
    cout<<"Select your action on this turn: a=attack, d=defense, s=store power, anything else=forfeit the game."<<endl;
}

void display_player_information(Player p1, Player p2){
  p1.display();
  p2.display();
}

Player attacker_update(Player p1, Player p2, string act1, string act2){
    if (act1=="a"){
        cout<<p1.getName()<<" attack!"<<endl;
        p1.attack();
    }else if (act1=="d"){
        cout<<p1.getName()<<" defense!"<<endl;
        p1.defense();
    }else if (act1=="s"){
        cout<<p1.getName()<<" store power!"<<endl;
        p1.store_power();
    }else{
        p1.suicide();
    }//User's action
    return p1;
}

Player defender_update(Player p1, Player p2, string act1, string act2){
    if (act1=="a" && act2!="d"){
        p2.get_attacked(p1.getMP());}
    return p2;
}

string Computer_Strategy(Player p1, Player p2, float r){
    float def_prob;
    float store_prob;
    if (p1.getMP()==0){
        def_prob=0;
    }else if (p1.getMP()==1){
        def_prob=0.02;
    }else if (p1.getMP()==2){
        def_prob=0.05;
    }else if (p1.getMP()==3){
        def_prob=0.25;
    }else if (p1.getMP()==4){
        def_prob=0.6;
    }else if (p1.getMP()==5){
        def_prob=0.9;
    }else{
        def_prob=1.0;
    }
    store_prob=(1.0f-def_prob)*(6.0f-min(float(p2.getMP()),6.0f))/6.0f;
    if (r<=def_prob){
        return "d";
    }else if (r<=def_prob+store_prob){
        return "s";
    }else {
        return"a";
    }
}
int main () {

  int Turn_counter=0;
  int c_choice;
  string user_action, computer_action;
  default_random_engine randomGenerator(time(0));
  uniform_real_distribution<float> Computer_choice(0, 1);

  string main_manual_choice;
  cout<<"Welcome to the battle game! What do you want to do?"<<endl;
  cout<<"1=Read the rule then play    2=Start playing    Others=Quit"<<endl;
  cin>>main_manual_choice;
  if(main_manual_choice=="1"){
    display_rule();
  }else if (main_manual_choice=="2"){

  }else{
    return 0;
  }

  Player user, computer;
  string username, computername;
  cout<<"Please enter your name."<<endl;
  cin>>username;
  cout<<"Please enter your enemy's name"<<endl;
  cin>>computername;
  user.set_values (100,0,username);
  computer.set_values (100,0,computername);
  display_player_information(user, computer);

  while (user.getHP()>0 && computer.getHP()>0){
    Turn_counter++;

    user_choice_this_turn(Turn_counter);
    cin>> user_action;

    computer_action=Computer_Strategy(user,computer,Computer_choice(randomGenerator));

    user=defender_update(computer,user,computer_action,user_action);
    computer=defender_update(user,computer,user_action,computer_action);

    user=attacker_update(user,computer,user_action,computer_action);
    computer=attacker_update(computer,user,computer_action,user_action);

    display_player_information(user, computer);
  }

  if (user.getHP()<=0 && computer.getHP()>0){
    cout << "You lose the game." << endl;
  } else if (computer.getHP()<=0 && user.getHP()>0){
    cout << "You win the game." << endl;
  } else {
    cout << "Draw" << endl;
  }
  return 0;
}

