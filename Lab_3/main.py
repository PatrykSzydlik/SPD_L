from RandomNumberGenerator import RandomNumberGenerator
from itertools import permutations
import copy


MAX_VALUE=29
MAX_D = 29

memory = []

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
    greedy_tasks = sorted(tasks_copy.queue, key=lambda x: x.d , reverse=False)
    greedy_permutation = []
    for i in greedy_tasks:
        greedy_permutation.append(i.n)
    return greedy_permutation


def PD1(D):
    sum = 0
    min = -1
    for j in D.queue: #sum
        sum += j.p
    for j in range(len(D.last_permutation)): # min
        D_copy = copy.deepcopy(D)
        new_perm = D_copy.last_permutation
        new_perm.pop(j)
        D_copy.set_permutation(new_perm)
        currentMin = max(sum- D.queue[j].d, 0)*D.queue[j].w + F1(D_copy)
        if min == -1 or currentMin < min:
            min = currentMin
    return min


def F1(D):
    global memory
    memory_ind=0
    for i in D.last_permutation:
        memory_ind |= 1<<(i-1)
    if memory[memory_ind] == -1:
        memory[memory_ind] = PD1(D)
    return memory[memory_ind]

local_memory = [0]

def PD_sum(tasks, D):
    new_sum = 0
    for i in range(0, tasks.size):
        if ((D >> i) & 1) == 1:
            new_sum += tasks.queue[i].p
    return new_sum  

def PD_minimum(tasks, D, sum):
    global local_memory
    task_list = []
    for i in range(0, tasks.size):
        if ((D >> i) & 1) == 1:
            task_list.append(i)
    if D == 0:
        return 0
    min_wiTi = 999999
    wiTi=0
    for j in task_list:
        wiTi = (sum-tasks.queue[j].d) * tasks.queue[j].w 
        if wiTi < 0:
            wiTi = 0
        #print(f"wiTi   {wiTi}")
        #print(local_memory)
        #print(f"COS   {cos}")
        wiTi += local_memory[D&(~(1<<j))]
        if wiTi < min_wiTi:
            min_wiTi = wiTi
    return min_wiTi

def PD_Patryk(tasks):
    global local_memory
    local_memory = [-1]*2**tasks.size
    #print(f"FRESH  {local_memory}")
    for D in range(0, 2**tasks.size):
        sum = PD_sum(tasks, D)
        local_memory[D] = PD_minimum(tasks, D, sum)
    return local_memory[2**tasks.size - 1]

if __name__ == '__main__':
    size = int(input('Podaj ilosc elementów: '))
    seed = int(input('Podaj ziarno RNG: '))
    print(f'Size {size} seed {seed}')
    tasks=tasks_queue(size, seed)
    tasks.display_instance()
    wiTi=tasks.calculate_wiTi()
    tasks.display_last_permutation()
    print(f"wiTi = {wiTi}")

    print("Brute")
    bruteforce_result = brute_force_alghorithm(tasks)
    tasks.set_permutation(bruteforce_result)
    wiTi=tasks.calculate_wiTi()
    tasks.display_last_permutation()
    print(f"wiTi = {wiTi}")

    print("Greedy")
    greedy_result = greedy_algorithm(tasks)
    tasks.set_permutation(greedy_result)
    wiTi=tasks.calculate_wiTi()
    tasks.display_last_permutation()
    print(f"wiTi = {wiTi}")
    
    
    print("PD_Patryk")
    print(PD_Patryk(tasks))
    
    print("PD1")
    memory = [-1]*(2**tasks.size - 1)
    natural_permutation = [number for number in range(1, tasks.size+1)]
    tasks.set_permutation(natural_permutation)
    print(PD1(tasks))