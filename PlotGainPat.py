#File: PlotLR.py
#Author: Julie Rolla
#Created on: 2/8

import matplotlib
matplotlib.use('Agg')

import csv
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

phiValue=315

with open('1uan.csv') as file:
    readCSV=csv.reader(file)
    
    FirstColumn = []
    SecondColumn = []
    ThirdColumn = []
    FourthColumn = []
    
    Theta = []
    Phi = []
    GainTheta = []
    GainPhi = []
    
    for row in readCSV:
       
        first=row[0]
        second=row[1]
        third = row[2]
        fourth = row[3]
        
        FirstColumn.append(first)
        SecondColumn.append(second)
        ThirdColumn.append(third)
        FourthColumn.append(fourth)

TotalRows = len(FirstColumn)
      
FirstColumn = list(map(float, FirstColumn))
SecondColumn = list(map(float, SecondColumn))
ThirdColumn = list(map(float, ThirdColumn))
FourthColumn = list(map(float, FourthColumn))
    
for i in range(0, TotalRows):
    if int(SecondColumn[i]) == phiValue:
        print(SecondColumn[i])
        Theta.append(FirstColumn[i])
        GainTheta.append(ThirdColumn[i])
        GainPhi.append(FourthColumn[i])
    print(i)
    i=i+1

 
# plt.figure()
# plt.plot(Theta, GainPhi)
# plt.show()
# 
# plt.figure()
# plt.plot( Theta, GainTheta)
# plt.xlabel('Theta')
# plt.show()

ThetaRad=np.radians(Theta)
# plt.figure()
# plt.polar(ThetaRad, GainPhi)
# plt.ylim(-160,-125)
# plt.xlabel('Gain (Phi)')
# plt.ylabel('Theta')
# plt.show()

plt.figure()
plt.polar(ThetaRad, GainTheta)
plt.ylim(-160,15)
plt.xlabel('Gain')
plt.ylabel('Theta')
plt.title('Individual 1, Phi = 315')
plt.show()
plt.savefig('1uan.png')

with open('2uan.csv') as file:
    readCSV=csv.reader(file)
    
    FirstColumn = []
    SecondColumn = []
    ThirdColumn = []
    FourthColumn = []
    
    Theta = []
    Phi = []
    GainTheta = []
    GainPhi = []
    
    for row in readCSV:
       
        first=row[0]
        second=row[1]
        third = row[2]
        fourth = row[3]
        
        FirstColumn.append(first)
        SecondColumn.append(second)
        ThirdColumn.append(third)
        FourthColumn.append(fourth)

TotalRows = len(FirstColumn)
      
FirstColumn = list(map(float, FirstColumn))
SecondColumn = list(map(float, SecondColumn))
ThirdColumn = list(map(float, ThirdColumn))
FourthColumn = list(map(float, FourthColumn))
    
for i in range(0, TotalRows):
    if int(SecondColumn[i]) == phiValue:
        print(SecondColumn[i])
        Theta.append(FirstColumn[i])
        GainTheta.append(ThirdColumn[i])
        GainPhi.append(FourthColumn[i])
    print(i)
    i=i+1

 
# plt.figure()
# plt.plot(Theta, GainPhi)
# plt.show()
# 
# plt.figure()
# plt.plot( Theta, GainTheta)
# plt.xlabel('Theta')
# plt.show()

ThetaRad=np.radians(Theta)

# plt.figure()
# plt.polar(ThetaRad, GainPhi)
# plt.ylim(-50,-30)
# plt.xlabel('Gain (Phi)')
# plt.ylabel('Theta')
# plt.show()

plt.figure()
plt.polar(ThetaRad, GainTheta)
plt.ylim(-160,15)
plt.xlabel('Gain (Theta)')
plt.ylabel('Theta')
plt.title('Individual 2, Phi = 315')
plt.show()
plt.savefig('2uan.png')

with open('3uan.csv') as file:
    readCSV=csv.reader(file)
    
    FirstColumn = []
    SecondColumn = []
    ThirdColumn = []
    FourthColumn = []
    
    Theta = []
    Phi = []
    GainTheta = []
    GainPhi = []
    
    for row in readCSV:
       
        first=row[0]
        second=row[1]
        third = row[2]
        fourth = row[3]
        
        FirstColumn.append(first)
        SecondColumn.append(second)
        ThirdColumn.append(third)
        FourthColumn.append(fourth)

TotalRows = len(FirstColumn)
      
FirstColumn = list(map(float, FirstColumn))
SecondColumn = list(map(float, SecondColumn))
ThirdColumn = list(map(float, ThirdColumn))
FourthColumn = list(map(float, FourthColumn))
    
for i in range(0, TotalRows):
    if int(SecondColumn[i]) == phiValue:
        print(SecondColumn[i])
        Theta.append(FirstColumn[i])
        GainTheta.append(ThirdColumn[i])
        GainPhi.append(FourthColumn[i])
    print(i)
    i=i+1

 
# plt.figure()
# plt.plot(Theta, GainPhi)
# plt.show()
# 
# plt.figure()
# plt.plot( Theta, GainTheta)
# plt.xlabel('Theta')
# plt.show()

ThetaRad=np.radians(Theta)

# plt.figure()
# plt.polar(ThetaRad, GainPhi)
# plt.ylim(-50,-30)
# plt.xlabel('Gain (Phi)')
# plt.ylabel('Theta')
# plt.show()

plt.figure()
plt.polar(ThetaRad, GainTheta)
plt.ylim(-160,15)
plt.xlabel('Gain (Theta)')
plt.ylabel('Theta')
plt.title('Individual 3, Phi = 315')
plt.show()
plt.savefig('3uan.png')

with open('4uan.csv') as file:
    readCSV=csv.reader(file)
    
    FirstColumn = []
    SecondColumn = []
    ThirdColumn = []
    FourthColumn = []
    
    Theta = []
    Phi = []
    GainTheta = []
    GainPhi = []
    
    for row in readCSV:
       
        first=row[0]
        second=row[1]
        third = row[2]
        fourth = row[3]
        
        FirstColumn.append(first)
        SecondColumn.append(second)
        ThirdColumn.append(third)
        FourthColumn.append(fourth)

TotalRows = len(FirstColumn)
      
FirstColumn = list(map(float, FirstColumn))
SecondColumn = list(map(float, SecondColumn))
ThirdColumn = list(map(float, ThirdColumn))
FourthColumn = list(map(float, FourthColumn))
    
for i in range(0, TotalRows):
    if int(SecondColumn[i]) == phiValue:
        print(SecondColumn[i])
        Theta.append(FirstColumn[i])
        GainTheta.append(ThirdColumn[i])
        GainPhi.append(FourthColumn[i])
    print(i)
    i=i+1

 
# plt.figure()
# plt.plot(Theta, GainPhi)
# plt.show()
# 
# plt.figure()
# plt.plot( Theta, GainTheta)
# plt.xlabel('Theta')
# plt.show()

ThetaRad=np.radians(Theta)

# plt.figure()
# plt.polar(ThetaRad, GainPhi)
# plt.ylim(-50,-30)
# plt.xlabel('Gain (Phi)')
# plt.ylabel('Theta')
# plt.show()

plt.figure()
plt.polar(ThetaRad, GainTheta)
plt.ylim(-160,15)
plt.xlabel('Gain (Theta)')
plt.ylabel('Theta')
plt.title('Individual 4, Phi = 315')
plt.show()
plt.savefig('4uan.png')

with open('5uan.csv') as file:
    readCSV=csv.reader(file)
    
    FirstColumn = []
    SecondColumn = []
    ThirdColumn = []
    FourthColumn = []
    
    Theta = []
    Phi = []
    GainTheta = []
    GainPhi = []
    
    for row in readCSV:
       
        first=row[0]
        second=row[1]
        third = row[2]
        fourth = row[3]
        
        FirstColumn.append(first)
        SecondColumn.append(second)
        ThirdColumn.append(third)
        FourthColumn.append(fourth)

TotalRows = len(FirstColumn)
      
FirstColumn = list(map(float, FirstColumn))
SecondColumn = list(map(float, SecondColumn))
ThirdColumn = list(map(float, ThirdColumn))
FourthColumn = list(map(float, FourthColumn))
    
for i in range(0, TotalRows):
    if int(SecondColumn[i]) == phiValue:
        print(SecondColumn[i])
        Theta.append(FirstColumn[i])
        GainTheta.append(ThirdColumn[i])
        GainPhi.append(FourthColumn[i])
    print(i)
    i=i+1

 
# plt.figure()
# plt.plot(Theta, GainPhi)
# plt.show()
# 
# plt.figure()
# plt.plot( Theta, GainTheta)
# plt.xlabel('Theta')
# plt.show()

ThetaRad=np.radians(Theta)

# plt.figure()
# plt.polar(ThetaRad, GainPhi)
# plt.ylim(-50,-30)
# plt.xlabel('Gain (Phi)')
# plt.ylabel('Theta')
# plt.show()

plt.figure()
plt.polar(ThetaRad, GainTheta)
plt.ylim(-160,15)
plt.xlabel('Gain (Theta)')
plt.ylabel('Theta')
plt.title('Individual 5, Phi = 315')
plt.show()
plt.savefig('5uan.png')
