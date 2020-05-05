# -*- coding: utf-8 -*-
"""
Created on Wed Apr  1 09:21:52 2020

@author: karlv
"""

# Matt Parker Train Problem

"""
Solution of the form
Distance d_0, max fuel f_0, trains n

d(n) = f_0(1+1/3+1/5+1/7+1/9+...+1/(2n-1))
where d(n) < d_0
fuel_used = f_0*n
"""

# Solving above equation
def fuel_needed_eq(d: float, f_0: float):
    d_n = 0
    n = 0
    while d_n < d:
        n+=1
        d_n += f_0/(2*n-1)
    n -= (d_n-d)*(2*n-1)/f_0
    print("Fuel used: " + str(n*f_0))
    return n*f_0


# Solution using trains
def fuel_needed(d: float, f_0:float):
    if d <= f_0:
        return d
    distance_needed = d-f_0
    trains_used = 1
    fuel_used = f_0
    while distance_needed > 0:
        distance_gone, new_fuel_used = previous_train(distance_needed, trains_used, f_0)
        trains_used += 1
        distance_needed -= distance_gone
        fuel_used += new_fuel_used
    print("Fuel used: " + str(fuel_used))
    return fuel_used
    

def previous_train(d_unfueled, trains_used, f_0):
    fuel_per_d = 2*trains_used+1
    distance = min(d_unfueled, f_0/fuel_per_d)
    
    return distance, distance*fuel_per_d



