from RandomNumberGenerator import RandomNumberGenerator
from itertools import permutations

MAX_VALUE=29

class Tasks(object):
    def __init__(self, size, machines, seed):
        self.size=size
        self.machines=machines
        self.task_queue=self.__generate_instance(seed)


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

    def calculate_Cmax(self, permutation):
        for m in range(1, self.machines+1):
            if m == 1:
                self.task_queue[permutation[0]]["S1"] = 0
                self.task_queue[permutation[0]]["C1"] = self.task_queue[0]['p1']
            else :
                self.task_queue[permutation[0]][f"S{m}"] = self.task_queue[permutation[0]][f"C{m-1}"]
            
            for number in permutation[1:]:
                number=number-1
                print(f"{number}")
                if m != 1:
                    self.task_queue[number][f'S{(m)}'] = max( self.task_queue[number-1][f'C{(m)}'], self.task_queue[number][f'C{(m-1)}'])
                else:
                    self.task_queue[number][f'S{(m)}'] = self.task_queue[number-1][f'C{(m)}']
                self.task_queue[number][f'C{(m)}'] = self.task_queue[number][f'S{(m)}'] + self.task_queue[number][f'p{(m)}']

            print(self.task_queue)
        print(permutation[-1])
        Cmax = self.task_queue[permutation[-1]-1][f'C{self.machines}']
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

def brute_force_alghorithm(tasks):
    tasks_numbers=[number for number in range(0,tasks.size)]
    permutation_generator = permutations(tasks_numbers)
    Cmin=float("inf")
    best_permutation=[]
    for random_sequence in permutation_generator:
        new_Cmax=tasks.calculate_Cmax(random_sequence)
        if new_Cmax < Cmin:
            Cmin=new_Cmax
            best_permutation=random_sequence
          


if __name__ == '__main__':
    size = int(input('Podaj ilosc elementów: '))
    machine = int(input('Podaj ilosc maszyn: '))
    seed = int(input('Podaj ziarno RNG: '))
    print(f'Size {size} machine {machine} seed {seed}')
    tasks=Tasks(size, machine, seed)
    tasks.display_instance()
    result = johnson_algorithm(tasks)
    print('Posegregowane!')
    print(result)
    cmax=tasks.calculate_Cmax(result)
    print(cmax)
