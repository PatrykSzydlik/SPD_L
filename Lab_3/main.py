from RandomNumberGenerator import RandomNumberGenerator
from itertools import permutations
import copy


MAX_VALUE=29
MAX_D = 29

class task():
    def __init__(self, number, p_time, w_weight, d_time):
        self.n=number
        self.p=p_time
        self.w=w_weight
        self.d=d_time
        self.S = 0
        self.C = 0
        self.T = 0
    
    def update_time(self, start_time):
        self.S = start_time
        self.C = self.S + self.p
        self.T = self.C - self.d
        if self.T < 0:
            self.T = 0
    
    def wiTi(self):
        return self.w*self.T


class tasks_queue():
    def __init__(self, size, seed):
        self.size = size
        self.last_permutation = []
        RNG = RandomNumberGenerator(seed)
        p_list=[]
        w_list=[]
        d_list=[]
        p_sum=0
        self.queue = []
        for J in range(0, self.size):
            p_list.append(RNG.nextInt(1, MAX_VALUE))
            p_sum+=p_list[-1]

        for J in range(0, self.size):
            w_list.append(RNG.nextInt(1, 9))
        
        for J in range(0, self.size):
            d_list.append(RNG.nextInt(1, MAX_D))

        for J in range(0, self.size):
            self.queue.append(task(J+1, p_list[J], w_list[J], d_list[J]))
            self.last_permutation.append(J+1)

    def set_permutation(self, new_permutation):
        self.last_permutation = list(new_permutation)

    def calculate_wiTi(self):
        previous_task_number = self.last_permutation[0]
        self.queue[previous_task_number-1].update_time(0)
        wiTi = self.queue[previous_task_number-1].wiTi()
        for task_number in self.last_permutation[1:]:
            self.queue[task_number-1].update_time(self.queue[previous_task_number-1].C)
            wiTi+=self.queue[task_number-1].wiTi()
            previous_task_number=task_number
        return wiTi

    def display_instance(self):
        numbers_display = " N : "
        p_display = " P : "
        w_display = " W : "
        d_display = " D : "
        for task in self.queue:
            numbers_display += f'{task.n}'.rjust(5)
            p_display += f'{task.p}'.rjust(5)
            w_display += f'{task.w}'.rjust(5)
            d_display += f'{task.d}'.rjust(5)
        
        print(numbers_display)
        print(p_display)
        print(w_display)
        print(d_display)
        print('\n\n')

    def display_last_permutation(self):
        numbers_display = " N : "
        S_display = " S : "
        C_display = " C : "
        T_display = " T : "
        for task_number in self.last_permutation:
            task=self.queue[task_number-1]
            numbers_display += f'{task.n}'.rjust(5)
            S_display += f'{task.S}'.rjust(5)
            C_display += f'{task.C}'.rjust(5)
            T_display += f'{task.T}'.rjust(5)
        
        print(numbers_display)
        print(S_display)
        print(C_display)
        print(T_display)
        print('\n\n')

def brute_force_alghorithm(tasks):
    tasks_numbers=[number for number in range(1,tasks.size+1)]
    permutation_generator = permutations(tasks_numbers)
    wiTimin=float("inf")
    best_permutation=[]
    for current_sequence in permutation_generator:
        tasks.set_permutation(current_sequence)
        new_wiTi=tasks.calculate_wiTi()
        if new_wiTi < wiTimin:
            wiTimin=new_wiTi
            best_permutation=current_sequence
    return best_permutation

def greedy_algorithm(tasks):   
    tasks_copy = copy.deepcopy(tasks)
    greedy_tasks = sorted(tasks_copy.queue, key=lambda x: x.d , reverse=True)
    greedy_permutation = []
    for i in greedy_tasks:
        greedy_permutation.append(i.n)
    return greedy_permutation

def PD1(tasks):
    D = copy.deepcopy(tasks)
    sum = 0
    memory = [2**tasks.size - 1]
    for i in memory:
        memory = -1
    min = 0

    for j in D.queue:
        sum += j.p
    for j in range(D.size):

        D_copy = copy.deepcopy(tasks)
        D_copy.queue.pop(j)
        D_copy.last_permutation.pop(j)
        D_copy.size -=1

        currentMin = max(sum- D.queue[j].d, 0)*D.queue[j].w + D_copy.calculate_wiTi()
        if min == 0 or currentMin < min:
            min = currentMin
    return min

if __name__ == '__main__':
    size = int(input('Podaj ilosc elementów: '))
    seed = int(input('Podaj ziarno RNG: '))
    print(f'Size {size} seed {seed}')
    tasks=tasks_queue(size, seed)
    tasks.display_instance()
    wiTi=tasks.calculate_wiTi()
    tasks.display_last_permutation()
    print(f"wiTi = {wiTi}")
    
    # print("Brute")
    # #bruteforce_result = brute_force_alghorithm(tasks)
    # tasks.set_permutation(bruteforce_result)
    # wiTi=tasks.calculate_wiTi()
    # tasks.display_last_permutation()
    # print(f"wiTi = {wiTi}")

    # print("Greedy")
    # greedy_result = greedy_algorithm(tasks)
    # tasks.set_permutation(greedy_result)
    # wiTi=tasks.calculate_wiTi()
    # tasks.display_last_permutation()
    # print(f"wiTi = {wiTi}")

    print("PD")
    print(PD1(tasks))
