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
        for i in range(self.size):
            dic = copy.deepcopy(self.task_queue[permutation[i]-1])
            self.lastPermutation_queue.append(dic)
        for m in range(1, self.machines+1): # okreslenie czasu rozpoczecia pierwszego taska na wszystkich maszynach
            if m == 1:
                self.lastPermutation_queue[0]["S1"] = 0
                self.lastPermutation_queue[0]["C1"] = self.lastPermutation_queue[0]['p1']
            else :
                self.lastPermutation_queue[0][f"S{m}"] = self.lastPermutation_queue[0][f"C{m-1}"]
                self.lastPermutation_queue[0][f'C{(m)}'] = self.lastPermutation_queue[0][f'S{(m)}'] + self.lastPermutation_queue[0][f'p{(m)}']

            for j in range(1, self.size): # okreslenie czasu rozpoczecia reszty taskow
                if m != 1:
                    self.lastPermutation_queue[j][f'S{(m)}'] = max( self.lastPermutation_queue[j-1][f'C{(m)}'], self.lastPermutation_queue[j][f'C{(m-1)}'])
                else:
                    self.lastPermutation_queue[j][f'S{(m)}'] = self.lastPermutation_queue[j-1][f'C{(m)}']
                self.lastPermutation_queue[j][f'C{(m)}'] = self.lastPermutation_queue[j][f'S{(m)}'] + self.lastPermutation_queue[j][f'p{(m)}']
        Cmax = self.lastPermutation_queue[-1][f'C{self.machines}']
        return Cmax


def johnson_algorithm(tasks) -> list(int):
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


def NEH_algorithm(tasks) -> list(int):
    N = tasks.task_queue.copy()
    k = 1
    W = PriorityQueue()
    pi_part = [[]*N.size]*N.machines 

    for j in range(N.size):
        w = 0
        for i in range(N.machines+1):
           w += N.task._queue[j][f'p{i}']
        W.put((-w, j))
        while not (W.empty()):
            j_w = W.get()
            for l in range(1,k+1):
                pi_part[l].append(j_w['j'])
                if pi_part[l][j_w['j']]
    

if __name__ == '__main__':
    size = int(input('Podaj ilosc elementów: '))
    machine = int(input('Podaj ilosc maszyn: '))
    seed = int(input('Podaj ziarno RNG: '))
    print(f'Size {size} machine {machine} seed {seed}')
    tasks=Tasks(size, machine, seed)
    tasks.display_instance()
