#File: gainPlot.py
#Author: Adam Blenk
#Created On: 3/16/18

import matplotlib #imports matplotlib
matplotlib.use('Agg') #makes it work on this machine

import csv
import numpy as np
import matplotlib.pyplot as plt

NPOP = 5
currentUan = 1

while currentUan <= NPOP: #loop runs through all uans per population member
    currentUanString = str(currentUan)
    junkLineCounter = 0 #counter variable for skipping lines before data
    
    thetas = [0, np.pi/12, np.pi/6, np.pi/4, np.pi/3, 5*np.pi/12, np.pi/2, 7*np.pi/12, 2*np.pi/3, 3*np.pi/4, 5*np.pi/6, 11*np.pi/12, np.pi] #sets theta coordinates to plot values at
    thetaGainArray = []
    plotGains = []
    plotGainsFloats = []
    rowArray = []
    with open(currentUanString + 'uan.csv', 'rt') as f: #opens file
        reader = csv.reader(f)
        for row in reader:
            junkLineCounter = junkLineCounter + 1
            if junkLineCounter > 17: #ignores non-data lines at beginning of file
                rowArray = row[0].split(" ") #splits rows of data into arrays
                thetaGainArray.append(rowArray[2]) #saves theta gains into an array

    f.close #closes file

    iterator = 0

    while iterator < len(thetaGainArray): #creates an array of theta gains at phi = 0
        plotGains.append(thetaGainArray[iterator])
        iterator = iterator + 25

    plotGainsFloats = [float(numeric_string) for numeric_string in plotGains] #converts theta gain array from strings to floats
    #print(plotGainsFloats)
    #print(len(thetas))
    #print(len(plotGainsFloats))

    fig = plt.figure() #declares figure
    ax = fig.add_subplot(111, polar=True) #declares a figure named ax as polar
    ax.set_rlim(-150, 20) #sets limits of the r axis
    ax.set_rticks(np.linspace(-150, 10, 9)) #sets 9 ticks on the r axis between -150 and 10
    ax.set_rlabel_position(-22.5)
    ax.set_theta_zero_location("N") #sets location of zero to north (top of graph)
    ax.set_xticks(np.linspace(0, 23*np.pi/12, 24)) #sets 24 ticks on the theta axis between 0 and 23pi/12
    ax.grid(True) #shows grid

    ax.set_title("Gain Plot " + currentUanString, va = 'bottom') #sets and shows title

    ax.plot(thetas, plotGainsFloats)
    #plots pairs of values from two arrays, the first array gives theta coordinate, the second gives r value (theta gain)

    plt.savefig(currentUanString + 'gainPlot.png') #saves the plot in the same directory as NgainPlot.png (where N = current uan.csv number)

    currentUan = currentUan + 1
