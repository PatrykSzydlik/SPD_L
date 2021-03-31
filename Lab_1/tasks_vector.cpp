#include <tasks_vector.hpp>
#define inf 99999999


using namespace std;

int tasks_vector::calculate_permutation(vector<int> new_permutation){
            if(_size != int(new_permutation.size())){
                cout<< "Podany wektor nowej permutacji ma zly rozmiar! Powinien miec rozmiar "<<_size<<" a nie "<<new_permutation.size()<<endl;
                return -1;
            }
            _currentPermutation.clear();
            int maxC = 0, c_plus_q;
            for (int i = 0; i<_size; i++){
                int ind = new_permutation[i];
                _currentPermutation.push_back(_tasks[ind]);
            }
            _currentPermutation[0].S = _currentPermutation[0].r;
            _currentPermutation[0].C = _currentPermutation[0].S + _currentPermutation[0].p;
            for( int i = 1; i<_size; i++){
                if(_currentPermutation[i-1].C < _currentPermutation[i].r) _currentPermutation[i].S = _currentPermutation[i].r;
                else _currentPermutation[i].S = _currentPermutation[i-1].C;
                _currentPermutation[i].C = _currentPermutation[i].S + _currentPermutation[i].p;
                c_plus_q = _currentPermutation[i].C + _currentPermutation[i].q;
                if(maxC < c_plus_q) maxC = c_plus_q;
            }
            _currentCmax = maxC;
            return maxC;
}

tasks_vector::tasks_vector(int seed, int size): _generator(seed){
            _size = size;
            int A = 0,X;
            for( int i = 0; i<_size; i++){
                _tasks.push_back(task());
                _tasks[i].j=i;
                _tasks[i].p = _generator.nextInt(1,29);
                A+=_tasks[i].p;
            }
            for( int i = 0; i<_size; i++) _tasks[i].r = _generator.nextInt(1,A);
            X = A; // X = 29; //
            for( int i = 0; i<_size; i++) _tasks[i].q = _generator.nextInt(1,X);

            vector<int> natural_permutation;
            for( int i = 0; i< _size; i++) natural_permutation.push_back(i);
            calculate_permutation(natural_permutation);
            _minmaxC = _currentPermutation.back().C;
}

void tasks_vector::display_beginning(){
            cout<<"\nParametry wygenerowanych zadan: "<<endl;
            cout<<"=========================\nnr: [";
            for( int i = 0; i<(_size-1); i++) cout<<(_tasks[i].j)<<", ";
            cout<<_tasks[_size-1].j<<"]"<<endl;
            cout<<"r: [";
            for( int i = 0; i<(_size-1); i++) cout<<_tasks[i].r<<", ";
            cout<<_tasks[_size-1].r<<"]"<<endl;
            cout<<"p: [";
            for( int i = 0; i<(_size-1); i++) cout<<_tasks[i].p<<", ";
            cout<<_tasks[_size-1].p<<"]"<<endl;
            cout<<"q: [";
            for( int i = 0; i<(_size-1); i++) cout<<_tasks[i].q<<", ";
            cout<<_tasks[_size-1].q<<"]"<<endl;
            cout<<"========================="<<endl;
}

void tasks_vector::display_permutation(){
            cout<<"\nOstatnio obliczona permutacja: "<<endl;
            cout<<"=========================\npi: [";
            for( int i = 0; i<(_size-1); i++) cout<<(_currentPermutation[i].j)<<", ";
            cout<<_currentPermutation[_size-1].j<<"]"<<endl;

            cout<<"S: [";
            for( int i = 0; i<(_size-1); i++) cout<<_currentPermutation[i].S<<", ";
            cout<<_currentPermutation[_size-1].S<<"]"<<endl;

            cout<<"C: [";
            for( int i = 0; i<(_size-1); i++) cout<<_currentPermutation[i].C<<", ";
            cout<<_currentPermutation[_size-1].C<<"]"<<endl;
            cout<<"========================="<<endl;
            cout<<"=========================\nC_max = "<<_currentCmax<<"\n========================="<<endl;
}

void tasks_vector::display_bestpermutation(){
            cout<<"\nNajlepsza obliczona permutacja: "<<endl;
            cout<<"=========================\npi: [";
            for( int i = 0; i<(_size-1); i++) cout<<(_bestPermutation[i].j)<<", ";
            cout<<_bestPermutation[_size-1].j<<"]"<<endl;

            cout<<"S: [";
            for( int i = 0; i<(_size-1); i++) cout<<_bestPermutation[i].S<<", ";
            cout<<_bestPermutation[_size-1].S<<"]"<<endl;

            cout<<"C: [";
            for( int i = 0; i<(_size-1); i++) cout<<_bestPermutation[i].C<<", ";
            cout<<_bestPermutation[_size-1].C<<"]"<<endl;

            cout<<"=========================\nmin{C_max} = "<<_minmaxC<<"\n========================="<<endl;
}

void tasks_vector::display_schrage_pmtn(){
            cout<<"\n Dla algorytmu schrage_pmtn: "<<endl;
            cout<<"=========================\npi: [";
            for( int i = 1; i<(_schrage_pmtn.size()-1); i++) cout<<(_schrage_pmtn[i].j)<<", ";
            cout<<_schrage_pmtn[_schrage_pmtn.size()-1].j<<"]"<<endl;

            cout<<"S: [";
            for( int i = 1; i<(_schrage_pmtn.size()-1); i++) cout<<_schrage_pmtn[i].S<<", ";
            cout<<_schrage_pmtn[_schrage_pmtn.size()-1].S<<"]"<<endl;

            cout<<"C: [";
            for( int i = 1; i<(_schrage_pmtn.size()-1); i++) cout<<_schrage_pmtn[i].C<<", ";
            cout<<_schrage_pmtn[_schrage_pmtn.size()-1].C<<"]"<<endl;

            cout<<"=========================\nC_max = "<<_minmaxC_pmtn<<"\n========================="<<endl;
}


void tasks_vector::sort_permutation(bool ascending, char to_sort){
            if( to_sort == 'r') sort(_currentPermutation.begin(), _currentPermutation.end(), r_compare());
            if( to_sort == 'p') sort(_currentPermutation.begin(), _currentPermutation.end(), p_compare());
            if( to_sort == 'q') sort(_currentPermutation.begin(), _currentPermutation.end(), q_compare());
            if(!ascending){
                vector<task> v;
                for(int i = _currentPermutation.size()-1; i > -1 ; i--) v.push_back(_currentPermutation[i]);
                _currentPermutation = v;
            }
            vector<int> newPermutation;
            for (unsigned int i =0; i<_tasks.size(); i++) newPermutation.push_back(_currentPermutation[i].j);
            calculate_permutation(newPermutation);
}

void tasks_vector::permute(vector<int> a, int l, int r){
            if (l == r){
                int currentC = calculate_permutation(a);
                if(currentC < _minmaxC || !_minmaxC){
                    _minmaxC = currentC;
                    _bestPermutation = _currentPermutation;
                }
            }
            else
            {
                for (int i = l; i <= r; i++)
                {
                swap(a[l], a[i]);
                permute(a, l+1, r);
                swap(a[l], a[i]);
                }
            }
}

void tasks_vector::brute_force(){
            cout<<"\nUruchomiono brute_force(). To moze troche potrwac"<<endl;
            vector<int> v;
            _minmaxC = 0;
            for (unsigned int i = 0; i < _currentPermutation.size(); i++) v.push_back(i);
            permute(v, 0, v.size()-1);
}

vector<int> tasks_vector::schrage(){
    vector<int> final_pi;
    vector<task> G;
    vector<task> N = _tasks;

    int min_r = 0; int min_r_ind;  //inicjalizacja wartosci aby nie bylo ostrzezen
    int max_q = 0; int max_q_ind;
    for (unsigned int i = 0, min_r = N[0].r+1; i < N.size(); i++){ if( N[i].r < min_r){ min_r = N[i].r; min_r_ind = i;}} // min_r
    int t = min_r;

    while( !G.empty() || !N.empty() ){
        for (unsigned int i = 0, min_r = N[0].r+1; i < N.size(); i++){ if( N[i].r < min_r){ min_r = N[i].r; min_r_ind = i;}} // min_r
        while(!N.empty() && (min_r <= t) ){
            for (unsigned int i = 0, min_r = N[0].r+1; i < N.size(); i++){ if( N[i].r < min_r){ min_r = N[i].r; min_r_ind = i;}} // min_r
            G.push_back(N[min_r_ind]);
            N.erase(N.begin() + min_r_ind);
        }
        if( !G.empty()){
            for (unsigned int i = 0, max_q =0; i < G.size(); i++){ if( G[i].q >= max_q){ max_q = G[i].q; max_q_ind = i;}} // max_q
            final_pi.push_back(G[max_q_ind].j); // brak potrzeby tworzenia zmiennej iteracyjnej k
            G.erase(G.begin() + max_q_ind);
            t = t + _tasks[max_q_ind].p;
        }
        else{
            for (unsigned int i = 0, min_r = N[0].r+1; i < N.size(); i++){ if( N[i].r < min_r){ min_r = N[i].r; min_r_ind = i;}} // min_r
            t = min_r;
        }
    }
    return final_pi;
}

vector<int> tasks_vector::schrage_queue(){
    vector<int> final_pi;
    priority_queue<task, vector<task>, r_reversed_compare> N; // min_r
    priority_queue<task, vector<task>, q_compare> G; // max_q
    for (unsigned int i = 0; i < _tasks.size(); i++){ N.push(_tasks[i]);}
    int t = N.top().r;

    while( !G.empty() || !N.empty() ){
        while(!N.empty() && (N.top().r <= t) ){
            G.push(N.top());
            N.pop();
        }
        if( !G.empty()){
            final_pi.push_back(G.top().j);
            t = t + _tasks[final_pi.back()].p;
            G.pop();
        }
        else{
            t = N.top().r;
        }
    }
    return final_pi;
}

void tasks_vector::schrage_pmtn(){
    _schrage_pmtn.clear();
    _minmaxC_pmtn = 0;
    vector<task> l;
    priority_queue<task, vector<task>, r_reversed_compare> N; // min_r
    priority_queue<task, vector<task>, q_compare> G; // max_q
    for (unsigned int i = 0; i < _currentPermutation.size(); i++){ N.push(_currentPermutation[i]);}
    task j;

    int t, t_next; // chwila czasowa
    t = N.top().r;
    t_next = 0;

    l.push_back(task());

    while( !G.empty() || !N.empty() ){
        while(!N.empty() && (N.top().r <= t) ){
            j = N.top();
            G.push(j);
            N.pop();
            if ( (j.q > l.back().q)){
                l.back().p = t - j.r;
                t = j.r;
                if(l.back().p > 0) {
                        G.push(l.back());
                }
            }
        }
        if( !G.empty()){
            j = G.top();
            G.pop();
            l.push_back(j);
            t = t + j.p;
            if(_minmaxC_pmtn <  (t + j.q))  _minmaxC_pmtn =  t + j.q;
        }
        else{
            t = N.top().r;
        }
    }
    _schrage_pmtn =  l;
}



