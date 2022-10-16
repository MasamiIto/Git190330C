
# coding: utf-8

# In[158]:


import  os
import  sys
import  csv
import  pandas as pd
import  numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib import patches


# In[191]:


def mkcsv_v1( fn , col,row,dx,dy):
    print( "FILE ANME:"+ fn )
    #--------------------------------------- 
    X    = np.zeros((col*row+1, 4))
    node = np.zeros((col*row+1, 2))
    for i in range(0,col):
        for j in range(0,row):
            y1= i*dy
            y2= (i+1)*dy -0.1
            x1= j*dx
            x2=(j+1)*dx-0.1
            X[i*col+j,0]=x1
            X[i*col+j,1]=y1
            X[i*col+j,2]=x2
            X[i*col+j,3]=y2
            node[i*col+j,0]=(x1+x2)/2
            node[i*col+j,1]=(y1+y2)/2
#            print( i*col+j, x1 )
#    print(X)        
    #--------------------------------------- データ書き込み CSV
    with open(fn, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(X)
    #--------------------------------------- 書いたファイルの読み込み Numpy
    a = np.loadtxt(fn, delimiter=',')
#    print(type(a))
#    print(a)
    #--------------------------------------- データ書き込み CSV
    with open("node.csv", 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(X)
    #--------------------------------------- 書いたファイルの読み込み Numpy
    nodea = np.loadtxt("node.csv", delimiter=',')
#    print(type(a))
#    print(a)
    #--------------------------------------- 表示
#    patch = patches.Rectangle(xy=(1, 2), width=2, height=3, fill=False)
    fig, ax = plt.subplots(figsize=(6, 6))
#    plt.plot([1,2,3,4], [1,4,9,16], 'ro')
    for index, nodeb in enumerate(node):
        plt.plot(nodeb[0], nodeb[1], 'ro') 
    for index, b in enumerate(a):
#        print(index, b)
        patch = patches.Rectangle(xy=(b[0],b[1]), width=b[2]-b[0], height=b[3]-b[1], fill=False)
        ax.add_patch(patch)
    ax.set_xlim(0,15)
    ax.set_ylim(0,15)
    ax.grid()

    plt.show()


# In[192]:


def draw( ):
 #---------------------------------------   

    patch = patches.Rectangle(xy=(1, 2), width=2, height=3, fill=False)

    fig, ax = plt.subplots(figsize=(6, 6))
    ax.add_patch(patch)
    ax.set_xlim(0,15)
    ax.set_ylim(0,15)
    ax.grid()

    plt.show()


# In[193]:


def draw_v1( ):
    x = np.linspace(0, 10, 100)
    y = x ** 2
 #---------------------------------------   
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.xlim  ( 0 , 15   )        # (F) y 軸の範囲
    plt.ylim  ( 0 , 15   )        # (F) y 軸の範囲
    ax.plot(x, y)
    plt.show()


# In[194]:


###------------------------------------------------------------------
def mkcsv_v0( fn , col,row,dx,dy):
    print('>------------------------------------------------- SIMPLE' );
    print( fn )
    X = np.zeros((col*row+1, 4))
    for i in range(col):
        for j in range(row):
            y1= i*dy
            y2= (i+1)*dy -0.1
            x1= j*dx
            x2=(j+1)*dx
#            print(x1,y1,x2,y2)
            X[i*col+j,0]=x1
            X[i*col+j,1]=y1
            X[i*col+j,2]=x2
            X[i*col+j,3]=y2
    #--------------------------------------- データ書き込み CSV
    with open(fn, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerows(X)
    #--------------------------------------- 書いたファイルの読み込み CSV STRING
    f = open( fn, 'r')
    reader = csv.reader( f )
    for row in reader:
        print (row )
    f.close()


# In[195]:


#draw()
mkcsv_v1( "test.csv",10,10,1,1)

