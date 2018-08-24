
*work in progress* --- please feel free to suggest improvements

## Testing SimComp simulators
This page describes a simple set up for testing different versions of the SimComp simulators. 
Currently, we only describe how to test the console version of the simulator following chapter 1 (also called *episode 1*), 
the AdHoc16 processor having only 11 instructions. 

### Testing episode 1
The test runs three SimComp assembler files (.sasm files). These are `test0.sasm`, `test1.sasm` and `test3_correctness_asm3b.sasm`, 
all found [here](https://github.com/LasseNatvig/SimComp/tree/master/\_Simulator/\_sasm).
For each of these, there is an .inp file that should be sent (piped in UNIX-terminology) to the simulator executable (.exe file under windows). There are also three corresponding .out files describing precisely the output that should be produced by running the simulator on the corresponding .inp file. Note that the very simple mechanism used to select the .sasm-file for loading and simulation is dependent on the number of .sasm files in the sasm-directory! 

Our own simplistic test setup is run from a jupyter notebook that calls two python-files `correctness_test.py` and `performance_test.py`. The notebook is the file `SimCompTesting.ipynb` and runs with python 2.
