from RandomNumberGenerator import RandomNumberGenerator
from itertools import permutations
import copy
from queue import PriorityQueue

MAX_VALUE=29

class Tasks(object):
    def __init__(self, size, machines, seed):
        self.size=size
        self.machines=machines
        self.task_queue=self.__generate_instance(seed)
        self.lastPermutation_queue = []

    def __generate_instance(self, seed):
        RNG = RandomNumberGenerator(seed)
        task_queue=[]
        for J in range(0, self.size):
            task_queue.append({'j':J+1})
            for M in range(1, self.machines+1):
                task_queue[J].update({f'p{M}': RNG.nextInt(1, MAX_VALUE)})
                task_queue[J].update({f'S{M}':0})  
                task_queue[J].update({f'C{M}':0})  
        return task_queue

    def display_instance(self):
        print('Displaying instance: \n')
        for i in range(self.machines+1):
            line=' j  :  ' if i==0 else f' p{i} :  '
            for task in self.task_queue:
                if i == 0:
                    elem=task['j']
                else:
                    elem=task[f'p{i}']
                line+=str(elem).rjust(5)
            print(line)
        print('\n')

    def display_lastPermutation(self):
        print('Displaying last calculated instance: \n')
        for i in range(self.machines+1):
            line=' j  :  ' if i==0 else f' C{i} :  '
            for task in self.lastPermutation_queue:
                if i == 0:
                    elem=task['j']
                else:
                    elem=task[f'C{i}']
                line+=str(elem).rjust(5)
            print(line)
        print('\n')

    def calculate_Cmax(self, permutation):
        self.lastPermutation_queue.clear()
        for i in range(len(permutation)):
            dic = copy.deepcopy(self.task_queue[permutation[i]-1])
            self.lastPermutation_queue.append(dic)
        for m in range(1, self.machines+1): # okreslenie czasu rozpoczecia pierwszego taska na wszystkich maszynach
            if m == 1:
                self.lastPermutation_queue[0]["S1"] = 0
                self.lastPermutation_queue[0]["C1"] = self.lastPermutation_queue[0]['p1']
            else :
                self.lastPermutation_queue[0][f"S{m}"] = self.lastPermutation_queue[0][f"C{m-1}"]
                self.lastPermutation_queue[0][f'C{(m)}'] = self.lastPermutation_queue[0][f'S{(m)}'] + self.lastPermutation_queue[0][f'p{(m)}']

            for j in range(1, len(permutation)): # okreslenie czasu rozpoczecia reszty taskow
                if m != 1:
                    self.lastPermutation_queue[j][f'S{(m)}'] = max( self.lastPermutation_queue[j-1][f'C{(m)}'], self.lastPermutation_queue[j][f'C{(m-1)}'])
                else:
                    self.lastPermutation_queue[j][f'S{(m)}'] = self.lastPermutation_queue[j-1][f'C{(m)}']
                self.lastPermutation_queue[j][f'C{(m)}'] = self.lastPermutation_queue[j][f'S{(m)}'] + self.lastPermutation_queue[j][f'p{(m)}']
        Cmax = self.lastPermutation_queue[-1][f'C{self.machines}']
        return Cmax


def johnson_algorithm(tasks):
    l = 0
    k = tasks.size - 1
    m = tasks.machines
    N = tasks.task_queue.copy()
    pi = [None]*tasks.size

    while N:
        min_p=N[0]['p1']
        i_min = 1
        j_min = 0
        for j in range(0, len(N)):
            for i in range(1, m+1):
                if N[j][f'p{i}'] < min_p:
                    min_p = N[j][f'p{i}']
                    i_min = i
                    j_min = j
        if N[j_min][f'p{1}'] < N[j_min][f'p{2}']:
            pi[l] = N[j_min]['j']
            l = l + 1
        else:
            pi[k] = N[j_min]['j']
            k = k - 1
        N.pop(j_min)
    return pi  


def NEH_algorithm(tasks):
    t = copy.deepcopy(tasks)
    N = tasks.task_queue.copy()
    k = 1
    W = PriorityQueue()
    pi_part = []
    current_cmax = 0
    best_pi_part = []
    min_cmax = -1

    for j in range(len(N)):                                     # for each j E N do
        w = 0
        for i in range(1, t.machines+1):
           w += N[j][f'p{i}']
        W.put((-w, N[j]['j']))                                  # END FOR
    while not (W.empty()):                                      # while W =/= 0 do
        j_2= W.get()
        j_w = j_2[1]
        min_cmax = -1                                           # j* <- argmax
        for l in range(k):                                      # for l<-1 ; l<=k do
            pi_part.insert(l, j_w)                              # pi' insert (j*, l)
            current_cmax = t.calculate_Cmax(pi_part)            # pi'.Cmax
            if min_cmax == -1 or current_cmax < min_cmax:       # pi' .Cmax < pi*.Cmax
                best_pi_part = pi_part.copy()                   # pi* <- pi'
                min_cmax = current_cmax                         # pi*.Cmax = pi'.Cmax
            pi_part.pop(l)
        pi_part = best_pi_part.copy()
        k+=1
    return best_pi_part

def NEH_algorithm_mod_4(tasks):
    t = copy.deepcopy(tasks)
    N = tasks.task_queue.copy()
    k = 1
    W = PriorityQueue()
    pi_part = []
    current_cmax = 0
    best_pi_part = []
    min_cmax = -1

    for j in range(len(N)):                                     # for each j E N do
        w = 0
        for i in range(1, t.machines+1):
           w += N[j][f'p{i}']
        W.put((-w, N[j]['j']))                                  # END FOR
    while not (W.empty()):                                      # while W =/= 0 do
        j_2= W.get()
        j_w = j_2[1]
        min_cmax = -1                                           # j* <- argmax
        for l in range(k):                                      # for l<-1 ; l<=k do
            pi_part.insert(l, j_w)                              # pi' insert (j*, l)
            current_cmax = t.calculate_Cmax(pi_part)            # pi'.Cmax
            if min_cmax == -1 or current_cmax < min_cmax:       # pi' .Cmax < pi*.Cmax
                best_pi_part = pi_part.copy()                   # pi* <- pi'
                min_cmax = current_cmax                         # pi*.Cmax = pi'.Cmax
            pi_part.pop(l)


        # 4. Zadanie, którego usunięcie spowoduję największe zmniejszenie wartościCmax
        lowest_cmax = -1
        x_index = 0
        min_cmax = -1
        if k > 1:
            for l in range(k):
                if best_pi_part[l] != j_w:
                    pop_task = best_pi_part.pop(l)
                    current_cmax = t.calculate_Cmax(best_pi_part)
                    if lowest_cmax == -1 or current_cmax < lowest_cmax: 
                        X = pop_task
                        lowest_cmax = current_cmax
                        x_index = l
                    best_pi_part.insert(l, pop_task)
            pi_part = best_pi_part.copy()
            pi_part.pop(x_index)
            for l in range(k):                                      # for l<-1 ; l<=k do
                pi_part.insert(l, X)                                # pi' insert (j*, l)
                current_cmax = t.calculate_Cmax(pi_part)            # pi'.Cmax
                if min_cmax == -1 or current_cmax < min_cmax:       # pi' .Cmax < pi*.Cmax
                   best_pi_part = pi_part.copy()                   # pi* <- pi'
                   min_cmax = current_cmax                         # pi*.Cmax = pi'.Cmax
                pi_part.pop(l)

        pi_part = best_pi_part.copy()
        k+=1
    return best_pi_part


def NEH_algorithm_mod_1(tasks):
    t = copy.deepcopy(tasks)
    N = tasks.task_queue.copy()
    k = 1
    W = PriorityQueue()
    pi_part = []
    current_cmax = 0
    best_pi_part = []
    min_cmax = -1

    for j in range(len(N)):                                     # for each j E N do
        w = 0
        for i in range(1, t.machines+1):
           w += N[j][f'p{i}']
        W.put((-w, N[j]['j']))                                  # END FOR
    while not (W.empty()):                                      # while W =/= 0 do
        j_2= W.get()
        j_w = j_2[1]
        min_cmax = -1                                           # j* <- argmax
        for l in range(k):                                      # for l<-1 ; l<=k do
            pi_part.insert(l, j_w)                              # pi' insert (j*, l)
            current_cmax = t.calculate_Cmax(pi_part)            # pi'.Cmax
            if min_cmax == -1 or current_cmax < min_cmax:       # pi' .Cmax < pi*.Cmax
                best_pi_part = pi_part.copy()                   # pi* <- pi'
                min_cmax = current_cmax                         # pi*.Cmax = pi'.Cmax
            pi_part.pop(l)

        # Trzeba znaleźć zadanie x, które jest inne niż dopiero dodane j_w
        # wybieramy je na podstawie jakiejś reguły np najdłuższa część ścieżki krytycznej
        
        # # 1.  Zadanie zawierające najdłuższą operacje na ścieżce krytycznej.
        pi_part = best_pi_part.copy()

        longest_task_on_crit_path = -1
        longest_time_on_crit_path = -1
        task_checked = 1
        for m in range(0, tasks.machines):
            machine = tasks.machines - m
            for i in range(task_checked, len(pi_part)):
                curr_elem = (pi_part[-i])-1
                prev_elem = (pi_part[-(i+1)])-1
                if tasks.task_queue[prev_elem][f'C{machine}'] == tasks.task_queue[curr_elem][f'S{machine}']:        # critical path is still on this machine
                    if tasks.task_queue[curr_elem][f'p{machine}'] > longest_time_on_crit_path :
                        longest_task_on_crit_path = tasks.task_queue[curr_elem][f'j']
                        longest_time_on_crit_path = tasks.task_queue[curr_elem][f'p{machine}']
                    task_checked+=1
                elif machine!=1:
                    if tasks.task_queue[curr_elem][f'S{machine}'] == tasks.task_queue[curr_elem][f'C{machine-1}']:  # critical path on another machine
                        if tasks.task_queue[curr_elem][f'p{machine}'] > longest_time_on_crit_path :
                            longest_task_on_crit_path = tasks.task_queue[curr_elem][f'j']
                            longest_time_on_crit_path = tasks.task_queue[curr_elem][f'p{machine}'] 
                            break
        X = longest_task_on_crit_path
        if(X != -1):
            pi_part.pop(pi_part.index(X))
            for l in range(k):                                        # for l<-1 ; l<=k do
                pi_part.insert(l, X)                                # pi' insert (j*, l)
                current_cmax = t.calculate_Cmax(pi_part)            # pi'.Cmax
                if min_cmax == -1 or current_cmax < min_cmax:       # pi' .Cmax < pi*.Cmax
                    best_pi_part = pi_part.copy()                   # pi* <- pi'
                    min_cmax = current_cmax                         # pi*.Cmax = pi'.Cmax
                pi_part.pop(l)


        pi_part = best_pi_part.copy()
        k+=1
    return best_pi_part

def NEH_algorithm_mod_2(tasks):
    t = copy.deepcopy(tasks)
    N = tasks.task_queue.copy()
    k = 1
    W = PriorityQueue()
    pi_part = []
    current_cmax = 0
    best_pi_part = []
    min_cmax = -1

    for j in range(len(N)):                                     # for each j E N do
        w = 0
        for i in range(1, t.machines+1):
           w += N[j][f'p{i}']
        W.put((-w, N[j]['j']))                                  # END FOR
    while not (W.empty()):                                      # while W =/= 0 do
        j_2= W.get()
        j_w = j_2[1]
        min_cmax = -1                                           # j* <- argmax
        for l in range(k):                                      # for l<-1 ; l<=k do
            pi_part.insert(l, j_w)                              # pi' insert (j*, l)
            current_cmax = t.calculate_Cmax(pi_part)            # pi'.Cmax
            if min_cmax == -1 or current_cmax < min_cmax:       # pi' .Cmax < pi*.Cmax
                best_pi_part = pi_part.copy()                   # pi* <- pi'
                min_cmax = current_cmax                         # pi*.Cmax = pi'.Cmax
            pi_part.pop(l)

        # Trzeba znaleźć zadanie x, które jest inne niż dopiero dodane j_w
        # wybieramy je na podstawie jakiejś reguły np najdłuższa część ścieżki krytycznej
        
        # # 1.  Zadanie zawierające najdłuższą operacje na ścieżce krytycznej.
        pi_part = best_pi_part.copy()

        current_time_sum = 0
        longest_task_on_crit_path = -1
        longest_time_sum_on_crit_path = -1
        task_checked = 1
        for m in range(0, tasks.machines):
            machine = tasks.machines - m
            for i in range(task_checked, len(pi_part)):
                curr_elem = (pi_part[-i])-1
                prev_elem = (pi_part[-(i+1)])-1
                if tasks.task_queue[prev_elem][f'C{machine}'] == tasks.task_queue[curr_elem][f'S{machine}']:        # critical path is still on this machine
                    if current_time_sum > longest_time_sum_on_crit_path :
                        longest_task_on_crit_path = tasks.task_queue[curr_elem][f'j']
                    task_checked+=1
                    current_time_sum = 0
                elif machine!=1:
                    if tasks.task_queue[curr_elem][f'S{machine}'] == tasks.task_queue[curr_elem][f'C{machine-1}']:  # critical path on another machine
                        
                        longest_task_on_crit_path = tasks.task_queue[curr_elem][f'j']
                        break
        X = longest_task_on_crit_path
        if(X != -1):
            pi_part.pop(pi_part.index(X))
            for l in range(k):                                        # for l<-1 ; l<=k do
                pi_part.insert(l, X)                                # pi' insert (j*, l)
                current_cmax = t.calculate_Cmax(pi_part)            # pi'.Cmax
                if min_cmax == -1 or current_cmax < min_cmax:       # pi' .Cmax < pi*.Cmax
                    best_pi_part = pi_part.copy()                   # pi* <- pi'
                    min_cmax = current_cmax                         # pi*.Cmax = pi'.Cmax
                pi_part.pop(l)


        pi_part = best_pi_part.copy()
        k+=1
    return best_pi_part

if __name__ == '__main__':
    size = int(input('Podaj ilosc elementów: '))
    machine = int(input('Podaj ilosc maszyn: '))
    seed = int(input('Podaj ziarno RNG: '))
    print(f'Size {size} machine {machine} seed {seed}')
    tasks=Tasks(size, machine, seed)
    tasks.display_instance()

    print('Johnson : ')
    result = johnson_algorithm(tasks)
    cmax=tasks.calculate_Cmax(result)
    tasks.display_lastPermutation()
    print(f"Cmax : {cmax}")

    print('NEH alghorithm')
    result = NEH_algorithm(tasks)
    cmax = tasks.calculate_Cmax(result)
    print(result)
    tasks.display_lastPermutation
    print(f"Cmax : {cmax}")

    print('NEH alghorithm mod 1')
    result = NEH_algorithm_mod_1(tasks)
    cmax = tasks.calculate_Cmax(result)
    print(result)
    tasks.display_lastPermutation
    print(f"Cmax : {cmax}")

    print('NEH alghorithm mod 4')
    result = NEH_algorithm_mod_4(tasks)
    cmax = tasks.calculate_Cmax(result)
    print(result)
    tasks.display_lastPermutation
    print(f"Cmax : {cmax}")
   
    
    

