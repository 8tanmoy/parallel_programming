#!/bin/sh
#SBATCH --partition=slurm_shortgpu
#SBATCH --time=0-00:10:00 # run time in days-hh:mm:ss
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --error=d_eigen.err
#SBATCH --output=d_eigen.out
#SBATCH --gres=gpu:1

./d_eigen
