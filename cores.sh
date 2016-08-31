for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/onecore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/twocore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/threecore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/fourcore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/fivecore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/sixcore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/sevencore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/eightcore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/ninecore.xml $t_size
done
for j in `seq 1 20`
do
    t_size=$((50000*$j))
    ./multicoregetdata platforms/platform.xml deployment/tencore.xml $t_size
done
