from graphics import *
import sys
import random
from PIL import Image as NewIm

def readFile():
    
    file = open("log-02-04-24.txt",'r')
    
    data = file.read()
    
    result_dict = {}
    
    for line in data.strip().split('\n'):
        values = list(map(int, line.split(', ')))
        if values[2] == 3:
            result_dict[values[1]] = values[4]
    
    return(result_dict)


def plotData(data):
    win = GraphWin("plotData", 750, 120)
    
    totalCPUTime = 0
    
    
    for x in data.values():
        totalCPUTime += x
        
    print(totalCPUTime)

    rectangleWidth = 500
    imagePlacement = 200
    
    for x, y in data.items():
        a = random.randint(0,255)
        b = random.randint(0,255)
        c = random.randint(0,255)
        
        processPercentage = y / totalCPUTime
        
        recWidth = processPercentage * rectangleWidth
        
        imagePlacement2 = imagePlacement + recWidth
        
        
     
        aRectangle = Rectangle(Point(imagePlacement,3), Point(imagePlacement2,100))
        aRectangle.setFill(color_rgb(a,b,c))
        centerPoint = aRectangle.getCenter()
        text = Text(centerPoint, str(x))
        aRectangle.draw(win)  
        text.draw(win)
        
        
        imagePlacement = imagePlacement2
        
    
    

    
     
    #win.postscript(file = fileName + '.eps') 
    #img = NewIm.open(fileName + '.eps') 
    #mg.save(fileName + '.png', 'png')
    
            
    win.getMouse() # pause for click in window
    win.close()    
    


if __name__ == "__main__":
    
   # fileName = sys.argv[1]
        
    data = readFile()
    
    plotData(data)