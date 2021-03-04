#include "RandomNumberGenerator.hpp"
#include <iostream>

#define MIN 1
#define MAX 29

using namespace std;

struct instance_J{
    int number;
    int prep_time;
    int exec_time;
    int start_time;
    int stop_time;
};

void display_instance(instance_J* tasks, int size){
    cout<<"\n Nr. |  ";
    for(int i=0;i<size;i++){
        cout<<tasks[i].number<<"  ";
    };
    cout<<"\n R_j. |  ";
    for(int i=0;i<size;i++){
        cout<<tasks[i].prep_time<<"  ";
    };
    cout<<"\n P_j. |  ";
    for(int i=0;i<size;i++){
        cout<<tasks[i].exec_time<<"  ";
    };
};

void calculate_time(instance_J* tasks, int size){
    
};



int main(){
    int seed=0;
    int size=0;
    do{
        cout<<"Złożoność problemu:\n";
        cin>>size;
    }while(!size);
    do{
        cout<<"Podaj seed dla generatora liczb losowych:\n";
        cin>>seed;
    }while(!seed);
    //cout<<"Seed "<<seed<<" Size "<<size<<endl;
    
    RandomNumberGenerator RNG(seed); 
    instance_J tasks[size];

    int sum=0;
    for(int i=0; i<size; i++){
        tasks[i].number=i+1;
        tasks[i].exec_time=RNG.nextInt(MIN, MAX);
        sum+=tasks[i].exec_time;
    };

    for(int i=0; i<size; i++){
        tasks[i].prep_time=RNG.nextInt(MIN, sum);
    };

    display_instance(tasks, size);
    calculate_time(tasks, size);
    return 0;
}