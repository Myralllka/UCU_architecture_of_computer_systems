# Lab work 6: solving a two-dimensional thermal conductivity problem using MPI.
    Topic: Solving a two-dimensional thermal conductivity problem using MPI
    Task: Write a program that solves the two-dimensional thermal conductivity problem using MPI. As always, the efficient disposal of all available computers is critical.
    Authors: Morhunenko Mykola & Yuriy Pasichnyk & Leno Solomia
# Usage
### Before 
You have to make correct config file located in the root, coled `config.onf` where `[main]` is the block for simulation and `[field-properties]` is properties for field.
more info is in comments in config file.

The default parameters for iron: <br>
specific_heat_capacity = 460 <br>
thermal_conduction = 80 <br>
density = 7800 <br>

### Usage
```shell script
chmod +x ./start.sh
./start.sh [options]
```
### Important!
Possible options:
```
    -h      --help                  Show help message.
    -O      --optimize-build        Compile with optimization before executing.
    -D      --debug-build           Compile with debug options.
    -d      --debug                 Run executable with debug symbols.
    -i      --image                 Save the images, not only gif, to res/ folder.
    
    -np     --number_of_processes   Number of processes for mpirun. Require second numerical parameter. 2 by default.
    -f      --file                  Config filename. Require second parameter. ./config.conf by default.
```
### Results
The results are in `res/` folder.
If it is  needed to save each image, use `-i` flag with script.  