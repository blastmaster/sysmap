#! /bin/bash

#SBATCH -A zihforschung
#SBATCH --job-name sysmap
#SBATCH --nodes 4
#SBATCH --ntasks 4
#SBATCH --ntasks-per-node 1
#SBATCH --partition haswell
#SBATCH --time=00:04:00
#SBATCH --output=sysmap-%j.out
#SBATCH --error=sysmap-%j.err

srun ./bin/sysmap -f SQL -e filesystem kernel_extractor disk hwloc_memory hwloc_machine hwloc_pci -o test
