#include "RandomNumberGenerator.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>

#define MIN 1
#define MAX_EXEC 29
#define MAX_FIN 29

using namespace std;

// Object structure holding data about the task
struct instance_J{
    int number=0;
    int prep_time=0;      // r
    int exec_time=0;      // p
    int fin_time=0;       // q
    int start_term=0;     // S
    int stop_term=0;      // C
    int delivery_term=0;  // C+q

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

    instance_J& operator=(const instance_J& other){
        this->number=other.number;
        this->prep_time=other.prep_time;
        this->exec_time=other.exec_time;
        this->fin_time=other.fin_time;
        this->start_term=other.start_term;
        this->stop_term=other.stop_term;
        this->delivery_term=other.delivery_term;
        return *this;
    };

};

// DEBUG FUNCTION
bool check_if_empty(instance_J* array, int array_size){
    for(int i=0;i<array_size;i++){
        if(array[i].number!=0){
            return false;
        };
    }; 
    return true;
};

// DEBUG FUNCTION
void display_elements(instance_J* array, int array_size){
    cout<<"\n######    Podgląd kolejki zadań    ######\n";
        cout<<"\n Nr. |  ";
        for(int i=0;i<array_size;i++){
            cout<<array[i].number<<"  ";
        };
        cout<<"\n r |  ";
        for(int i=0;i<array_size;i++){
            cout<<array[i].prep_time<<"  ";
        };
        cout<<"\n p |  ";
        for(int i=0;i<array_size;i++){
            cout<<array[i].exec_time<<"  ";
        };
        cout<<"\n q |  ";
        for(int i=0;i<array_size;i++){
            cout<<array[i].fin_time<<"  ";
        };
        cout<<endl;
};

instance_J * get_elem_max_fin_time(instance_J * task_array, int array_size){
    auto iter = max_element(task_array, task_array+array_size, [](instance_J a, instance_J b) {return (a.fin_time<b.fin_time);});
    return iter;
};

instance_J * get_elem_min_prep_time(instance_J * task_array, int array_size){
    int min_prep_time=task_array[0].prep_time;
    instance_J * min_elem = task_array;
    for(int i=1; i<array_size; i++){
        if(min_prep_time==0){
            min_prep_time=task_array[i].prep_time;
            min_elem = &task_array[i];
        }else{
            if(task_array[i].prep_time != 0){
                if(task_array[i].prep_time < min_prep_time){
                    min_prep_time = task_array[i].prep_time;
                    min_elem = &task_array[i];
                };
            };
        };
    };
    return min_elem;
};

void insert_into_array_and_clear_original(instance_J * task_array, int array_size, instance_J* elem_to_insert){
    for(int i=0;i<array_size;i++){
        if(task_array[i].number==elem_to_insert->number){
            // Elem is already inside the array and should be removed from origin
            break;
        }else if(task_array[i].number == 0){
            // There's an empty space in the array
            task_array[i] = *elem_to_insert;
            break;
        };
    };
    elem_to_insert->zero_all();
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
        };

        for(int i=0; i<size; i++){
            this->tasks[i].fin_time=RNG.nextInt(MIN, sum);
        };
    };

    void display_instance(){
        cout<<"\n######    Podgląd kolejki zadań    ######\n";
        cout<<"\n Nr. |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].number);
        };
        cout<<"\n   r |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].prep_time);
        };
        cout<<"\n   p |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].exec_time);
        };
        cout<<"\n   q |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].fin_time);
        };
        cout<<endl;

        this->calculate_time();
        this->display_instance_time();
    };

    void sort_by_prep_time(){
        sort(this->tasks, this->tasks+this->size, [](instance_J a, instance_J b) {return (a.prep_time<b.prep_time);});
        cout<<"\n Sortowanie po czasie przygotowanie \n";
        this->calculate_time();
        this->display_instance_time();
    };

    void sort_with_Schrage_array_based(){
        int k = 0;
        instance_J G_tasks[this->size];
        instance_J N_tasks[this->size];
        instance_J PI_tasks[this->size];

        for(int i=0;i<this->size;i++){
            N_tasks[i]=this->tasks[i];
 //           cout<<"Skopiowano task : "<<N_tasks[i].number<<"\n";
        };

        int current_time = get_elem_min_prep_time(N_tasks, this->size)->prep_time;
 //       cout<<"Minimum znalezione to: "<<current_time<<endl;


        cout<<"\n Sortowanie algorytmem Schrage \n";
        while(check_if_empty(G_tasks,this->size) == false || check_if_empty(N_tasks,this->size) == false ){
/* DEBUG
            cout<<"\n Zmienna K wynosi "<<k<<" \n";
            cout<<"\nTablica G tasks \n";
            display_elements(G_tasks, this->size);
            cout<<"\nCzy tablica G jest pusta? :  ";
            if(check_if_empty(G_tasks,this->size)==true){cout<<"true";}else{cout<<"false";};
            cout<<"\n";
            cout<<"\nTablica N tasks \n";
            display_elements(N_tasks, this->size);
            cout<<"\nCzy tablica N jest pusta? :  ";
            if(check_if_empty(N_tasks,this->size)==true){cout<<"true";}else{cout<<"false";};
            cout<<"\n";
 */           
            while(check_if_empty(N_tasks,this->size) == false  && get_elem_min_prep_time(N_tasks, this->size)->prep_time <= current_time){
//              cout<< "Inserting into G_tasks \n";
                insert_into_array_and_clear_original(G_tasks, this->size, get_elem_min_prep_time(N_tasks, this->size));
/* DEBUG
                cout<<"\nTablica G tasks \n";
                display_elements(G_tasks, this->size);
                cout<<"\nCzy tablica G jest pusta? :  ";
                if(check_if_empty(G_tasks,this->size)==true){cout<<"true";}else{cout<<"false";};
                cout<<"\n";
                cout<<"\nTablica N tasks \n";
                display_elements(N_tasks, this->size);
                cout<<"\nCzy tablica N jest pusta? :  ";
                if(check_if_empty(N_tasks,this->size)==true){cout<<"true";}else{cout<<"false";};
                cout<<"\n";
*/

            };
            if(not check_if_empty(G_tasks,this->size)){
//              cout<< "Inserting into PI_tasks \n";
                insert_into_array_and_clear_original(PI_tasks, this->size, get_elem_max_fin_time(G_tasks, this->size));
                current_time += PI_tasks[k].exec_time;
                k++;
            }else{
                current_time = get_elem_min_prep_time(N_tasks, this->size)->prep_time;
            };
        };
        for(int i=0;i<this->size;i++){
            this->tasks[i] = PI_tasks[i];
        };
        this->calculate_time();
        this-> display_instance();
    };

    void sort_with_Schrage_priority_queue_based(){

    };

    private:
    void display_instance_time(){
        cout<<"\n######    Podgląd czasu wykonywania zadań    ######\n";
        cout<<"\n Nr. |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].number);
        };
        cout<<"\n   S |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].start_term);
        };
        cout<<"\n   C |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].stop_term);
        };
        cout<<"\n C_q |  ";
        for(int i=0;i<this->size;i++){
            printf("%6d,", this->tasks[i].delivery_term);
        };
        cout<<endl;
        cout<<"\n C_max = "<<this->C_max<<"  \n";
        cout<<endl;
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
    task_series task_queue(size);
    task_queue.generate_instance(seed);
    task_queue.display_instance();
    task_queue.sort_by_prep_time();
    task_queue.sort_with_Schrage_array_based();

    return 0;
}