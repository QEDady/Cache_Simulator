# Cache_Simulator
This project takes as an input a file containing a sequence of memory addresses in the RAM. The output will be the entries corresponding to the cache chosen along with the hit and miss ratios, and the AMAT.

This project is for the "Computer Organization and Assembly Language Programming" course. It was developed by Amr Sallam, Zeyad Tolba, and Amer Elsheikh in Spring 2022.

## User Guide:
1. Enter the cache size (S) and line size (L)
2. Choose the cache type (D-> Direct mapping, F-> Fully associate, S-> Set associative)
3. Specify the number of cycles of the hit access
4. Enter the name of the memory file

The specifications entered by the user will provide the necessary information to construct the appropriate cache.
When trying to access every memory address in the input file, the script will decide whether this memory address is in the cache or not and calculate the updated hit and miss ratios. 

The prject supports two different caches for instructions and data; a prefix "i" refers to instruction and prefix "d" refers to data in the input file. 
