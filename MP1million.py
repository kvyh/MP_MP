# -*- coding: utf-8 -*-
"""
Created on Wed May 13 07:49:51 2020

@author: karlv
"""
import time
import math
import matplotlib.pyplot as plt
"""
a series of solutions of the form:

1922, 1182
1182, 740
740, 442
442, 298
298, 144 finishes in 18 days
144, 154 finishes in 19 days
...

first, second           finishes in n days
second, first - second  finishes in n + 1 days
"""


def until_1_million(first, second):
    if first == second and first == 0:
        return
    days = 2
    total = second + first
    previous = first
    while total < 10000000:
        new_total = total + previous
        previous = total
        total = new_total
        print(new_total)
        days += 1
        
        if total == 1000000:
            if second > first:
                print((first, second), "; ", days, ": ", new_total)
            return True
    return False
        
def follow_chain(x, y):
    if until_1_million(x, y) and x - y >= 0:
        follow_chain(y, x - y)

#for x in range(20000):
#    for y in range(20000):
#        if until_1_million(x, y) and x - y >= 0:
#            follow_chain(y, x - y)
    
def reverse_search(second_to_last, final):
    if second_to_last < 0 or final < 0 or second_to_last > final:
        return False
    
    total = final
    previous_total = second_to_last
    days = 1
    while previous_total > 0:
        new_prev = total - previous_total
        if new_prev < 0:
            break
        total = previous_total
        previous_total = new_prev
        days += 1
#    if days > math.log(final):
#        print(previous_total, total, days)
    
    return previous_total, total, days

init = 600000
shortest_shown = math.log(init)

finals = []
longest_days = []

for x in range(init):
    longest = 0
    finish = init + x
    ideal_previous = int(finish / ((1 + 5 ** 0.5) / 2))
    best = [0,0,0,0]
    
    for y in [-1, 0, 1]:
        ft, sd, days = reverse_search(ideal_previous + y, finish)
        if days > longest:
            
            longest = days
            best[0] = finish
            best[1] = ft
            best[2] = sd
            best[3] = days
            
    finals.append(best[0])
    longest_days.append(best[3])
    
    #if days >= shortest_shown:
    #    print(best[0], ": ", best[1], best[2], best[3])

plt.plot(finals, longest_days)
plt.show()
    