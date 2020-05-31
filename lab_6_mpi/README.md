# Lab work 3: count the number of words in the archive_obj of text files.
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

### Results
The results are in `res/` folder.
if uncomment the 