#include "RandomNumberGenerator.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

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

class task_series{
    private:
    instance_J* tasks = nullptr;
    int size = 0;

    public:
    task_series(int size){
        this->tasks = new instance_J[size];
        this->size = size;
    };

    ~task_series(){
        delete(this->tasks);
    };

    void generate_instance(int RNG_seed){
       
        RandomNumberGenerator RNG(RNG_seed);
        int sum=0;
        for(int i=0; i<this->size; i++){
            this->tasks[i].number=i+1;
            this->tasks[i].exec_time=RNG.nextInt(MIN, MAX);
            sum+=this->tasks[i].exec_time;
        };

        for(int i=0; i<size; i++){
            this->tasks[i].prep_time=RNG.nextInt(MIN, sum);
        };
    };

    void display_instance(){
        cout<<"\n Nr. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].number<<"  ";
        };
        cout<<"\n R_j. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].prep_time<<"  ";
        };
        cout<<"\n P_j. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].exec_time<<"  ";
        };
    };

    void display_instance_time(){
        cout<<"\n Nr. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].number<<"  ";
        };
        cout<<"\n S_j. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].start_time<<"  ";
        };
        cout<<"\n C_j. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].stop_time<<"  ";
        };
    };

    void calculate_time(){
        int current_time=0;
        this->tasks[0].start_time=this->tasks[0].prep_time;
        this->tasks[0].stop_time=this->tasks[0].start_time + this->tasks[0].exec_time;
        for(int i=1;i<this->size;i++){
                if(this->tasks[i-1].stop_time < this->tasks[i].prep_time){
                this->tasks[i].start_time=this->tasks[i].prep_time;
                }else{
                this->tasks[i].start_time=this->tasks[i-1].stop_time;
                };
                this->tasks[i].stop_time=this->tasks[i].start_time + this->tasks[i].exec_time;
        };
    };

    void sort_by_prep_time(){
        instance_J temp;
        instance_J sorted[this->size];
        sort(this->tasks, this->tasks+this->size, [](instance_J a, instance_J b) {return (a.prep_time<b.prep_time);});
    };

    void sort_with_Schrage(){
        int current_time=0;
        instance_J G_tasks[this->size];
        instance_J N_tasks[this->size];

    };
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
    
    task_series task_queue(size);
    task_queue.generate_instance(seed);
    task_queue.display_instance();
    task_queue.calculate_time();
    task_queue.display_instance_time();
    task_queue.sort_by_prep_time();
    task_queue.calculate_time();
    task_queue.display_instance_time();

    return 0;
}