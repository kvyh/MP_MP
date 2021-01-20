# -*- coding: utf-8 -*-
"""
Created on Wed Jun 10 12:59:28 2020

@author: karlv
"""

class paper:
    def __init__(self):
        self.height = 2
        self.width = 4
        self.cells = []
        for x in range(2):
            for y in range(4):
                self.cells.append(cell(x,y))
        self.edges = [(0,0,0), (0,1,0), (1,1,0), (2,1,0)]
        
        

    def all_stacks(self):
        # list of (final order, moves made)
        self.stacks = []
        self.next_fold(self.cells, self.edges, self.stacks)
        
    def next_fold(self, cells, edges, stacks, folds = [], depth = 0):
        new_depth = depth
        new_edges = copy.copy(edges)
        new_cells = copy.copy(cells)
        
        for edge in edges:
            for up in [True, False]:
                if up and edge[2] and edge[0] >= -1:
                    continue
                if up and edge[0] == 0:
                    continue
                if up and edge[1] == 0:
                    continue
                
                max_depth = 0
                # if the previous fold was the opposite direction
                if folds[-1][1] != edge[1]:
                    for cell in new_cells:
                        if cell.xy[edge[1]] > edge[0]:
                            

class cell:
    def __init__(self, x, y):
        self.initial = (x,y)
        self.xy = (x, y)
        self.depth = 0