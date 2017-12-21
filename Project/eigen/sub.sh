#!/bin/sh
#SBATCH --partition=slurm_shortgpu
#SBATCH --time=0-00:10:00 # run time in days-hh:mm:ss
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --error=eigen_size.err
#SBATCH --output=eigen_size.out
#SBATCH --gres=gpu:1

printf "#size\t#ev no.\t#val\t\t#time(ms)\n"
for((i=0;i<14;i++))
do
	./eigen_serial $i
done
