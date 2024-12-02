#!/bin/bash
#SBATCH --job-name=diffusion_task1          # Job name
#SBATCH --output=task1.out        # Standard output and error log
#SBATCH --error=task1.err         # Separate file for error logs
#SBATCH --partition=spgpu        # Partition name
#SBATCH --gpus=1                  # Number of nodes
#SBATCH --cpus-per-task=4          # Number of CPU cores per task
#SBATCH --time=0-02:00:00            # Runtime (HH:MM:SS)
#SBATCH --mem=48G                   # Memory per node
#SBATCH --account=eecs498s014f24_class                   

# Load any required modules (if applicable)
module load python3.10-anaconda

# Command to execute
source ~/.bashrc
conda activate Diffusion
python execute_DDPM_DDIM.py --scheduler_name DDPM

