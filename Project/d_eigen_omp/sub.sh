#!/bin/sh
#SBATCH --partition=slurm_shortgpu
#SBATCH --time=0-00:10:00 # run time in days-hh:mm:ss
#SBATCH --nodes=1
#SBATCH --cpus-per-task=20
#SBATCH --error=d_eigen_omp.err
#SBATCH --output=d_eigen_omp.out
#SBATCH --gres=gpu:1

#./d_eigen_omp 8
printf "#th\t#size\t#ev_no\t#step\t#val\t\t\t#error\t\t\t#time(ms)\n"
for((i=4; i<11; i++))
do
	for((j=1; j<21; j++))
	do
		./d_eigen_omp $j $i
	done
done
