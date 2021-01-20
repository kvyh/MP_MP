# -*- coding: utf-8 -*-
"""
Created on Wed Jun 24 11:03:58 2020

@author: karlv
"""


class triangle:
    def __init__(self, num1, num2, num3):
        self.mid = num1
        self.left = num2
        self.right = num3
        self.prev_sort = [sorted([num1, num2, num3])]
        self.looping = False
        self.verbose = True
        
    def summed(self):
        return self.mid + self.left + self.right
        
    def check_loop(self,state):
        if sorted(state) in self.prev_sort:
            if self.verbose:
                print("looped")
            return True
        else:
            return False
        
    def next_step(self):
        
        mid = abs(self.left - self.right)
        left = abs(self.left - self.mid)
        right = abs(self.right - self.mid)
        
        if self.verbose:
            print("left: ", left, "; mid: ", mid, "; right: ", right, "; sum: ", mid + left + right)
        
        if self.check_loop([mid, left, right]):
            self.looping = True
        self.prev_sort.append(sorted([mid, left, right]))
            
        self.mid = mid
        self.left = left
        self.right = right
        
    def play(self):
        if self.verbose:
            print("left: ", self.left, "; mid: ", self.mid, "; right: ", self.right, 
                  "; sum: ", self.summed())
        
        while not self.looping:
            self.next_step()
            
            
def find_stable_loops(target):
    candidates = []
    for x in range(target + 1)[::-1]:
        for y in range(target + 1 - x)[::-1]:
            z = target - x - y
            if x >= y and y >= z:
                tri = triangle(x, y, z)
                tri.verbose = False
                tri.play()
                if tri.summed() == target:
                    candidates.append([x, y, z])
                    
    for cand in candidates:
        print("Candidate: ", cand)
        
    if not candidates:
        print("No Candidates Found")
        