# Lab work 3: count the number of words in the archive of text files.
    Topic: Multi-threaded word countinf Ed.1
    Task: Simple threading on word counting.
    Authors: Solomia Leno & Morhunenko Mykola & Yuriy Pasichnyk
# Usage
from `count_number_of_all_words/`
```$shell
./start.py config.dat [parameters]
```
!important! parameter -b required for the first start. for more details `./start.py -h`
# Results
the histogram is in the img/ folder.
testing was on the "lorem ipsum..." 459720525B file.
26 and 100 threads was just for fun, but, as we can se, there was small increases of productivity.
I took "Analyzing" time for analyzing the time of program. 
# config.dat
infile:     path to input txt or archive file
out_by_a:   path to output file by alpha
out_by_n:   path to  output file by number of occurrences
threads:    number of threads