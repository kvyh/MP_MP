# -*- coding: utf-8 -*-
"""
Created on Wed Jun 10 11:33:23 2020

@author: karlv
"""

"""
For a paper with height x and width y

if a horizontal fold is made, the next vertical fold must fold all layers; vice versa




"""



class paper:
    def __init__(self, height, width):
        self.height = height
        self.width = width
        self.edges, self.faces = self.generate(height, width)
        
    def generate(self, height, width):
        edges = []
        faces = []
        for x in range(height):
            if x < height-1:
                edges.append(edge(x, 0))
            for y in range(width):
                faces.append(face(x,y))
                if x < height - 1 and y < width - 1:
                    edges.append(edge(y, 1))
        return edges, faces
                    
    def fold(self, edge, direction):
# TODO: handle folded edges requiring multiple layers to fold with/against their direction
        
        # direction is "up" or "down"
        
        for face in self.faces:
            if edge.depth % 2:
                if face.xy[edge.orient] > edge.position:
                    if direction = "up"
                    
                    face.xy[edge.orient] -= 2 * (face.xy[edge.orient] - edge.position) - 1
                    face.orientation = 1 - face.orientation
                    if direction == "up":
                        face.depth -= 1
                    else:
                        face.depth += 1
            else if face.xy[edge.orient] <= edge.position
                face.xy[edge.orient] -=    
                
        for edg in self.edges:
            


class edge:
    def __init__(self, position, orient: int):
        # orient 0 is horizontal
        # position n means it is after row n
        self.position = position
        self.orient = orient
        self.depth = 0
        

class face:
    def __init__(self, x, y):
        self.initial = (x,y)
        self.xy = [x, y]
        
        self.depth = 0
        self.orientation = 0
        
    def facing(self):
        if self.orientation == 0:
            return "up"
        else:
            return "down"
        
    def x(self):
        return self.xy[0]
    
    def y(self):
        return self.xy[1]