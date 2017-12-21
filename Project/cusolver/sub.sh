#!/bin/sh
#SBATCH --partition=slurm_shortgpu
#SBATCH --nodes=1
#SBATCH --tasks=40
#SBATCH --cpus-per-task=1
#SBATCH -e cusolver.err
#SBATCH -o cusolver.out
#SBATCH --gres=gpu:1
printf "#size\t#ev_no\t#value\t\t\t#time(ms)\n"
for((i=4; i<13; i++))
do
	./cusolver $i
done
