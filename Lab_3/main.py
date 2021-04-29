from RandomNumberGenerator import RandomNumberGenerator
from itertools import permutations

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

        print(f'Task N ={self.n}')
        print(f'Start time ={self.S}')
        print(f'End time ={self.C}')
        print(f'Delay time ={self.T}')
    
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

    def set_permutation(new_permutation):
        self.last_permutation = new_permutation.copy()

    def calculate_wiTi(self):
        previous_task_number = self.last_permutation[0]
        self.queue[previous_task_number-1].update_time(0)
        print(f'Previos number {previous_task_number}')
        wiTi = self.queue[previous_task_number].wiTi()
        for task_number in self.last_permutation[1:]:
            print(f'Current number {task_number}')
            print(f'Last finished {self.queue[previous_task_number-1].C}')
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

if __name__ == '__main__':
    size = int(input('Podaj ilosc elementów: '))
    seed = int(input('Podaj ziarno RNG: '))
    print(f'Size {size} seed {seed}')
    tasks=tasks_queue(size, seed)
    tasks.display_instance()
    wiTi=tasks.calculate_wiTi()
    tasks.display_last_permutation()
    print(f"wiTi = {wiTi}")


