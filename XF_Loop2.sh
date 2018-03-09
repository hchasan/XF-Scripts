#!/bin/bash
#Loop through several generations of antennas using Evolved_Dipole and XF
#Created on 12/06/2017
#Hannah Hasan

RunName='handspin'                ## Replace when needed
WorkingDir=`pwd`
ScriptsDir=$WorkingDir/../Xmacros2
XFexec='/mnt/c/Users/Hannah/Documents/Research/XF/bin/Win64.NET2010'  ##Location of XFUI executable
#XFProj='/mnt/c/Users/Hannah/Documents/Research/XF/ARA_XFProjects_misc/Projects/'$RunName'.xf/'    ##give path to the directory you will save the XF project in. Additionally, please save the XF project with the same name as RunName
XFProj='../../ARA_XFProjects_misc/Projects/'$RunName.xf/
TotalGens=5   ## number of generations (after initial) to run through

## Lines for output.xmacro files ##

line1='var query = new ResultQuery();'
line2='///////////////////////Get Theta and Phi Gain///////////////'
line3='query.projectId = App.getActiveProject().getProjectDirectory();'
line4='query.simulationId = '  ## append 6-digit simulation ID number; for example "000001";
## cat skeleton here
lastline='FarZoneUtils.exportToUANFile(thdata,thphase,phdata,phphase,inputpower,"C:/Users/Hannah/Documents/Research/XF/XF_Scripts/Evolved_Dipole/data/' ##append simulation ID number followed by .uan");


cd hands
mkdir $RunName
cd $RunName
mkdir plots

cd $WorkingDir
#./Evolved_Dipole --start
./handflail --start
#./Roulette_Select --start
cp handshake.csv hands/$RunName/0_handshake.csv

cd data
for i in {1..5}
do
    echo "$line1" >> $ScriptsDir/output.xmacro
    echo "$line2" >> $ScriptsDir/output.xmacro
    echo "$line3" >> $ScriptsDir/output.xmacro
    echo "$line4"'"00000'"$i"'";'>> $ScriptsDir/output.xmacro
    cat outputmacroskeleton.txt >> $ScriptsDir/output.xmacro
    echo "${lastline}${i}"'.uan");' >> $ScriptsDir/output.xmacro
done


echo
echo
echo 'Opening XF user interface...'
echo '*** Please remember to save the project with the same name as RunName! ***'
echo
echo '1. Import and run dipole_PEC.xmacro'
echo '2. Import and run output.xmacro'
echo '3. Close XF'
read -p "Press any key to continue... " -n1 -s

## Run XF -- opens graphical interface. From there, dipole_PEC.xmacro needs to be run, then all output(...).xmacro scripts (1-5) shsould be run.
cd $XFexec
./xfui.exe
echo
read -p "Press any key to continue... " -n1 -s
echo
echo "Resuming..."
echo

cd $WorkingDir
cd data
InputFiles=
for i in {1..5}
do
    InputFiles="${InputFiles}${i}.uan "
done
./makehandshook $InputFiles
mv handshook.csv $WorkingDir
cd $WorkingDir

## make plots
cd $WorkingDir
./gensData.exe
for i in {1..5}
do
    cp data/$i.uan ${i}uan.csv
done

./uanCleaner

python plotLR.py
#python PlotGainPat.py

mv Length.png $WorkingDir/hands/$RunName
mv Radius.png $WorkingDir/hands/$RunName

for i in {1..5}
do
    mv ${i}uan.csv $WorkingDir/hands/$RunName/plots/0_${i}uan.csv
#    mv ${i}uan.png $WorkingDir/hands/$RunName/plots/0_${i}uan.png
done

cp handshook.csv hands/$RunName/0_handshook.csv

for ((gen=1; gen<=$TotalGens; gen++))
do
    cd $WorkingDir
    #./Evolved_Dipole --cont
    ./handflail --cont
    #./Roulette_Select --cont

    cp handshake.csv hands/$RunName/${gen}_handshake.csv
    
    ###
    cd $ScriptsDir
    rm output.xmacro
    cd $WorkingDir
    cd data
    for i in {1..5}
    do
	echo "$line1" >> $ScriptsDir/output.xmacro
	echo "$line2" >> $ScriptsDir/output.xmacro
	echo "$line3" >> $ScriptsDir/output.xmacro
	echo "$line4"'"00000'"$i"'";'>> $ScriptsDir/output.xmacro
	cat outputmacroskeleton.txt >> $ScriptsDir/output.xmacro
	echo "${lastline}${i}"'.uan");' >> $ScriptsDir/output.xmacro
	done
    ###



#    let simID=$(( $gen*5+1 ))
#    #### FIX NUMBERS SO THEY MATCH SIMULATION ID ####
#    for((ID=$simID; ID<simID+5; ID++))
#    do
#	echo $line1 >> $ScriptsDir/output_${gen}.xmacro
#	echo $line2 >> $ScriptsDir/output_${gen}.xmacro
#	echo $line3 >> $ScriptsDir/output_${gen}.xmacro
#    if(("$ID" < 10))
#	then
#	    echo $line4'"00000'${ID}'";' >> $ScriptsDir/output_${gen}.xmacro
#	elif(("$ID" >= 10 && "$ID" < 100 ))
#	then
#	    echo $line4'"0000'${ID}'";' >> $ScriptsDir/output_${gen}.xmacro
#	else
#	    echo $line4'"000'${ID}'";' >> $ScriptsDir/output_${gen}.xmacro
#	fi
#	cat outputmacroskeleton.txt >> $ScriptsDir/output_${gen}.xmacro
#	echo ${lastline}${i}'.uan");' >> $ScriptsDir/output_${gen}.xmacro
#    done

    cd $XFexec
    cd $XFProj
    rm -rf Simulations
    echo
    echo
    echo 'Opening XF user interface...'
    echo 
    echo '1. Run dipole_PEC.xmacro (no new modifications needed)'
    echo "2. Run output.xmacro (no new modifications needed)"
    echo '3. Close XF'
    read -p "Press any key to continue... " -n1 -s
    echo
    ## Run XF -- opens graphical interface. From there, dipole_PEC.xmacro needs to be run, then all output(...).xmacro scripts (1-5) shsould be run.
    cd $XFexec
    ./xfui.exe $XFProj
    
    read -p "Press any key to continue... " -n1 -s
    echo "Resuming..."
    echo


    cd $WorkingDir
    cd data
    InputFiles=
    for i in {1..5}
    do
	InputFiles="${InputFiles}${i}.uan "
    done
    ./makehandshook $InputFiles
    mv handshook.csv $WorkingDir
    cd $WorkingDir


    ## make plots
    cd $WorkingDir
    ./gensData.exe
    for i in {1..5}
    do
	cp data/$i.uan ${i}uan.csv
    done

    ./uanCleaner
    
    python plotLR.py
    #python PlotGainPat.py

    mv Length.png $WorkingDir/hands/$RunName
    mv Radius.png $WorkingDir/hands/$RunName

    for i in {1..5}
    do
	mv ${i}uan.csv $WorkingDir/hands/$RunName/plots/${gen}_${i}uan.csv
#	mv ${i}uan.png $WorkingDir/hands/$RunName/plots/${gen}_${i}uan.png
    done

    
#    cd $WorkingDir
#    cd data
#    InputFiles=
#    for((ID=1; ID<=5; ID++))
#    do
#	InputFiles=${InputFiles}${ID}'.uan '
#    done
#    ./makehandshook $InputFiles
#    mv handshook.csv $WorkingDir
#    cd $WorkingDir
    
    cp handshook.csv hands/$RunName/${gen}_handshook.csv
    
done


echo
echo 'Done!'