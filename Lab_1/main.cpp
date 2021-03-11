#include "RandomNumberGenerator.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

#define MIN 1
#define MAX_EXEC 29
#define MAX_FIN 29

using namespace std;

struct instance_J{
    int number=0;
    int prep_time=0;
    int exec_time=0;
    int fin_time=0;
    int start_term=0;
    int stop_term=0;
    int delivery_term=0;

    public:
    void zero_all(){
        number=0;
        prep_time=0;
        exec_time=0;
        fin_time=0;
        start_term=0;
        stop_term=0;
        delivery_term=0;
    };
};

class task_series{
    private:
    instance_J* tasks = nullptr;
    int size = 0;
    int C_max = 0;

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
            this->tasks[i].exec_time=RNG.nextInt(MIN, MAX_EXEC);
            sum+=this->tasks[i].exec_time;
        };

        for(int i=0; i<size; i++){
            this->tasks[i].prep_time=RNG.nextInt(MIN, sum);
            this->tasks[i].fin_time=RNG.nextInt(MIN, MAX_FIN);
        };
    };

    void display_instance(){
        cout<<"\n Podgląd kolejki zadań \n";
        cout<<"\n Nr. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].number<<"  ";
        };
        cout<<"\n r |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].prep_time<<"  ";
        };
        cout<<"\n p |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].exec_time<<"  ";
        };
        cout<<"\n q |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].fin_time<<"  ";
        };
    };

    void display_instance_time(){
        cout<<"\n Podgląd czasu wykonywania zadań \n";
        cout<<"\n Nr. |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].number<<"  ";
        };
        cout<<"\n S |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].start_term<<"  ";
        };
        cout<<"\n C |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].stop_term<<"  ";
        };
        cout<<"\n C_q |  ";
        for(int i=0;i<this->size;i++){
            cout<<this->tasks[i].delivery_term<<"  ";
        };
        cout<<"\n C_max = "<<this->C_max<<"  \n";
    };

    void calculate_time(){
        int final_target=0;
        this->tasks[0].start_term=this->tasks[0].prep_time;
        this->tasks[0].stop_term=this->tasks[0].start_term + this->tasks[0].exec_time;
        this->tasks[0].delivery_term=this->tasks[0].stop_term+this->tasks[0].fin_time;
        final_target=this->tasks[0].delivery_term;
        for(int i=1;i<this->size;i++){
                if(this->tasks[i-1].stop_term < this->tasks[i].prep_time){
                    this->tasks[i].start_term=this->tasks[i].prep_time;
                }else{
                    this->tasks[i].start_term=this->tasks[i-1].stop_term;
                };
                this->tasks[i].stop_term=this->tasks[i].start_term + this->tasks[i].exec_time;
                this->tasks[i].delivery_term=this->tasks[i].stop_term+this->tasks[i].fin_time;
                if(this->tasks[i].delivery_term > final_target){
                    final_target=this->tasks[i].delivery_term;
                };
        };
        cout<<"\n Pomiar czasu wykonania w obecnej kolejności \n";
        this->C_max=final_target;
    };

    void sort_by_prep_time(){
        instance_J temp;
        instance_J sorted[this->size];
        sort(this->tasks, this->tasks+this->size, [](instance_J a, instance_J b) {return (a.prep_time<b.prep_time);});
        cout<<"\n Sortowanie po czasie przygotowanie \n";
    };

    void sort_with_Schrage(){
        int k = 1;
        instance_J G_tasks[this->size];
        instance_J N_tasks[this->size];
        for(int i=1;i<this->size;i++){N_tasks[i]=this->tasks[i];};
        int current_time=min_element(N_tasks, N_tasks+this->size, [](instance_J a, instance_J b) {return (a.prep_time<b.prep_time);})->prep_time;
        cout<<"minimum znalezione to: "<<current_time<<endl;
        cout<<"\n Sortowanie algorytmem Schrage \n";

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
    task_queue.sort_with_Schrage();

    return 0;
}