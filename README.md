# CS-3210: Programming Assignment 1, Spring '25, Ashoka University

**Suraj Sharma**  
suraj.sharma_ugt2023@ashoka.edu.in  

## Structure of the Program

This directory contains the following files:

- **`main.c`** – Main implementation of the algorithm.  
- **`nT_data`** – File to collect (n, T) for each sample run.  
  - Every time `main` is run on some input, it extracts the number of points and T from the run and appends it to the `nT_data` file.  
  - **Note:** It appends data, doesn’t overwrite, so be careful if you plan to run it twice on the same input.  
- **`util.py`** – Program to generate a random sample of points and run the main program on it.  
- **`util_plot.py`** – A Python script to plot the `nT_data` from the file `nT_data`.  

## Running the Program

### Step 1: Compile `main.c`  
Please read the `main.c` header first.  

For ready referece, 

1. Compile via the following command:  

gcc main.c -o main -lm

2. Generate Inputs and Run main

python3 util.py  

Be careful! It will generate around 1000 input and output files, so it will fill the directory. Maybe don't do this if you like a clean directory

3. Plot the nT_data, it will create a pdf titled nT_plot in the same directory.

python3 util_plot.py

Note:
- Please follow the above procedure to run the program.
- The commands are for Linux distributions; For Windows or Mac please use your own wisdom. 
