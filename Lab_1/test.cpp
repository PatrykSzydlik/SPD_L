void sort_with_Schrage_array_based_pmtn(){
        int k = 0;
        instance_J G_tasks[this->size];
        instance_J N_tasks[this->size];
        int PI_task_size = 2*this->size+1;
        instance_J PI_tasks[PI_task_size];
        instance_J temp;

        for(int i=0;i<this->size;i++){
            N_tasks[i]=this->tasks[i];
        };
        int current_time = get_elem_min_prep_time(N_tasks, this->size)->prep_time;
        int time_diff=0;


        cout<<"\n Sortowanie algorytmem Schrage z przerwaniami\n";
        while(check_if_empty(G_tasks,this->size) == false || check_if_empty(N_tasks,this->size) == false ){   
            while(check_if_empty(N_tasks,this->size) == false  && get_elem_min_prep_time(N_tasks, this->size)->prep_time <= current_time){
                insert_into_array_and_clear_original(G_tasks, this->size, get_elem_min_prep_time(N_tasks, this->size));
                
            };
            // Gdy są zadania gotowe do wykonania
            if(not check_if_empty(G_tasks,this->size)){
                // Jeśli to nie jest dodawanie pierwszego zadania
                if(k>0){
                    // Sprawdź czy nie ma nowego zadania o dłuższym czasie stygnięcia
                    if(get_elem_max_fin_time(G_tasks, this->size)->fin_time > PI_tasks[k-1].fin_time && PI_tasks[k].stop_term < current_time){
                        //obliczamy ile czasu minęło
                        time_diff = current_time - PI_tasks[k-1].start_term;
                        temp = PI_tasks[k-1];
                        //zapisujemy ile czasu przerwane zadanie już się odbywało
                        PI_tasks[k-1].exec_time = time_diff;
                        PI_tasks[k-1].stop_term = current_time;
                        PI_tasks[k-1].fin_time = 0;
                        //zapisujemy ile mu zostało
                        temp.exec_time -= time_diff;
                        temp.start_term = 0;
                        temp.stop_term = 0;
                        //cofamy zadanie do kolejki przygotowanych
                        insert_into_array_and_clear_original(G_tasks, this->size, &temp);
                    };
                };

                insert_into_array_and_clear_original(PI_tasks, PI_task_size, get_elem_max_fin_time(G_tasks, this->size));
                PI_tasks[k].start_term = current_time;
                PI_tasks[k].stop_term =  PI_tasks[k].start_term +  PI_tasks[k].exec_time;
                k++;
            };
            current_time = get_elem_min_prep_time(N_tasks, this->size)->prep_time;
            if(k>0 && check_if_empty(G_tasks,this->size) == false){
                if(current_time > PI_tasks[k-1].stop_term){
                    current_time = PI_tasks[k-1].stop_term;
                };
            };
            cout<<"\n Current time is "<<current_time<<"\n";
            display_elements(G_tasks, this->size);
            display_elements(N_tasks, this->size);
            display_elements(PI_tasks, PI_task_size);
        };


        delete this->tasks;
        this->size=k;
        this->tasks = new instance_J[this->size];

        for(int i=0;i<this->size;i++){
            this->tasks[i] = PI_tasks[i];
        };
        this-> display_instance();
    };