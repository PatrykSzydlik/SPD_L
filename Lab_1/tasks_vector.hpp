#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <RandomNumberGenerator.hpp>


using namespace std;

#pragma once

/**
*   @brief Struktura reprezentujaca podstawowe zadanie
*/
struct task{
    int j = 0; // numer w kolejce, indeks problemu
    int r = 0; // czas przygotowania/termin dostepnosci
    int q = 0; // czas dostarczenia/stygniecia
    int p = 0; // czas wykonania
    int S = 0; // czas rozpoczecia
    int C = 0; // czas zakonczenia
};

/**
*   @brief Wykorzysrywane w sortowaniu, oraz kolejce priorytetowej
*/
struct r_compare{inline bool operator()(const task& t1, const task& t2){return (t1.r < t2.r);}};
struct r_reversed_compare{inline bool operator()(const task& t1, const task& t2){return (t1.r > t2.r);}};
struct q_compare{inline bool operator()(const task& t1, const task& t2){return (t1.q < t2.q);}};
struct p_compare{inline bool operator()(const task& t1, const task& t2){return (t1.p < t2.p);}};


class tasks_vector{
    RandomNumberGenerator _generator;
    vector<task> _tasks;
    vector<task> _currentPermutation;
    int _currentCmax;
    int _size; // zmienna pomocnicza, aby unikac ciaglego wywolywania metody _tasks.size()
    /////////
    int _minmaxC;
    vector<task> _bestPermutation;
    /////////
    vector<task> _schrage_pmtn;
    int _minmaxC_pmtn;


    public:
        /**
        *   @brief Algorytm 1: Obliczenie nowych wartosci S i C dla podanej w wektorze permutacji. Wynik zapisywany w _currentPermutation
        *   @param Wektor z nową kolejnoscia na podstawie parametru j np. 2 1 0 3
        *   @return C_max obliczony na podstawie podanego wektora
        */
        int calculate_permutation(vector<int> new_permutation);

        /**
        *   @brief Konstruktor klasy
        *   @param seed - ziarno losowania
        *   @param size - rozmiar problemu
        */
        tasks_vector(int seed, int size);

        /**
        *   @brief Wyswietlenie poczatkowych parametrow po wygenerowaniu instancji ( j, r, p)
        */
        void display_beginning();

        /**
        *   @brief Wyswietlenie j S C dla elementow aktualnie obliczonej permutacji(patrz calculate_permutation)
        */
        void display_permutation();

        /**
        *   @brief Wyswietlenie j S C Cmax dla elementow najlepszej zapamietanej permutacji
        */
        void display_bestpermutation();

        /**
        *   @brief Wyswietlanie j S C Cmax dla wyniku metody schrage_pmtn()
        */
        void display_schrage_pmtn();

        /**
        *   @brief Tworzy nowy wektor permutacji w oparciu o sortowanie elementow wzgledem podanego parametru. Wynik zapisywany w _currentPermutation.
        *   @param ascending = true - sortowanie rosnąco, ascending = false - sortowanie malejaco
        *   @param char =  'r' / 'p' / 'q' - wybor wzgledem czego odbedzie sie sortowanie
        */
        void sort_permutation(bool ascending, char to_sort);

        private:
        /**
        *   @brief Prywatna metoda wykorzystywana wewnatrz metody brute_force().
        *   Źródło: https://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/
        */
        void permute(vector<int> a, int l, int r);

        public:
        /**
        *   @brief Przeszukanie wszystkich mozliwych kombinacji w poszukiwaniu najlepszej. Wynik zapisywany w _bestPermutation.
        */
        void brute_force();

        /**
        *   @brief Algorytm 2: Algorytm schrage w wersji podstawowej
        *   @param Brak parametrow poniewaz algorytm korzysta z instancji problemu wygenerowanej w konstruktorze
        *   @return Obliczona permutacja
        */
        vector<int> schrage();

        /**
        *   @brief Algorytm 3: Algorytm schrage w wersji z kolejka priorytetowa
        *   @param Brak parametrow poniewaz algorytm korzysta z instancji problemu wygenerowanej w konstruktorze
        *   @return Obliczona permutacja
        */
        vector<int> schrage_queue();

        /**
        *   @brief Algorytm 5: Algorytmchrage w wersji z przerwaniami
        *   @param Brak parametrow poniewaz algorytm korzysta z instancji problemu wygenerowanej w konstruktorze
        *   @return Wynik algorytmu nadpisuje wektor _schrage_pmtn, a C_max jest zapisane w _minmaxC_pmtn
        */
        void schrage_pmtn();

        /**
        *   @brief Algorytm 4: Algorytmchrage w wersji z kolejka priorytetowa
        *   @param Brak parametrow poniewaz algorytm korzysta z instancji problemu wygenerowanej w konstruktorze
        *   @return Obliczona permutacja
        */
        vector<int> cariler();
};
