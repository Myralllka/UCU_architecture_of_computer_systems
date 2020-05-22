# Lab work 3: count the number of words in the archive_obj of text files.
    Topic: Multi-threaded word countinf Ed.2
    Task: Parallel counting + parallel merging.
    Authors: Morhunenko Mykola & Yuriy Pasichnyk
# Usage
from `lab_4/`
```$shell
./start.sh [config filename] [parameters]
```
!important! parameter -b required for the first start. for more details `./start.py -h`
 
# config.dat
infile:         path to input txt or archive_obj file.
out_by_a:       path to output file by alpha.
out_by_n:       path to  output file by number of occurrences.
threads:        number of threads for indexing.
map_threads:    number of threads for merging maps.

# Result
We tested this app on  16.3Gb `guttenberg_2020_03_06.iso` file

Result files are here: [Link](https://drive.google.com/drive/folders/1OHPD4gVCiXG-dPlWHVMebgdNE37IvXtW)

Time spent: 1274778673mcs (about 21.5m) (Intel Core I7-7700HQ 4 physical / 8 logical cores, Linux system)
