# Lab work 3: count the number of words in the archive_obj of text files.
    Topic: Multi-threaded word countinf Ed.1
    Task: Simple threading on word counting.
    Authors: Morhunenko Mykola & Yuriy Pasichnyk
# Usage
from `count_number_of_all_words/`
```$shell
./start.py config.dat [parameters]
```
!important! parameter -b required for the first start. for more details `./start.py -h`
 
# config.dat
infile:     path to input txt or archive_obj file
out_by_a:   path to output file by alpha
out_by_n:   path to  output file by number of occurrences
threads:    number of threads