for j in `seq 0 7`
do
    t_size=$((10**$j))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/onecore.xml $t_size
    t_size=$((5*$t_size))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/onecore.xml $t_size
done
for j in `seq 0 7`
do
    t_size=$((10**$j))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/twocore.xml $t_size
    t_size=$((5*$t_size))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/twocore.xml $t_size
done
for j in `seq 0 7`
do
    t_size=$((10**$j))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/threecore.xml $t_size
    t_size=$((5*$t_size))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/threecore.xml $t_size
done
for j in `seq 0 7`
do
    t_size=$((10**$j))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/fourcore.xml $t_size
    t_size=$((5*$t_size))
    ./multicore_logimp_thr platforms/multCPU.xml deployment/fourcore.xml $t_size
done
