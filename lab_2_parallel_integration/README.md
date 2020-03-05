## Lab 2 (performance testing)

    Topic:      Two variables integral parallel calculation.
    
    Task:       Find the integral of the given Langermann function, 
                where m, ci, a1i, a2i, i = 1..m -- f_params.
                
                    m=5
                    a1 = [1, 2, 1, 1, 5]
                    a2 = [4, 5, 1, 2, 4]
                    c = [2, 1, 4, 7, 2]
                    
                In intervals: x = -10..10; y = -10 .. 10.
                
    Latex Function Formula: 
    $$ f(x_1, x_2) = \sum_{i=1}^{m} c_i exp(- \frac{1}{\pi} ((x_1 - a_{1i})^2 + (x_2 - a_{2i})^2)) cos(\pi((x_1 - a_{1i})^2 + (x_2 - a_{2i})^2)) $$
    
    Expected result: -1.604646665
    Task Number:    4
    Author:         Yuriy Pasichnyk 
----
### Function:
![formula](https://render.githubusercontent.com/render/math?math=f(x_1,x_2)=\sum_{i=1}^{m}c_i%20exp(-\frac{1}{\pi}%20((x_1%20-%20a_{1i})^2%20+%20(x_2%20-%20a_{2i})^2))%20cos(\pi((x_1%20-%20a_{1i})^2%20+%20(x_2%20-%20a_{2i})^2)))
### Project Structure

    - [cmake-build-debug]   # build directory
    - include               # headers
    - scripts               # scripts
    - src                   # source files
        
    ## Names in [] are generated. 

    build.sh - prepare (create dirs and run cmake) project for work and generate data
    run.sh - runs tests and if nesesary runs build.sh
    
    
#### Conf file structure
Input to the program is path to the configuration file. It is a line separated list of values.
    
    - Absolute precision
    - Relative precision (not less than 0.001)
    - Number of parallel execution flows
    - Expected solution
    - x region start
    - x region end
    - y region start
    - x region end
    [possible additional params not needed yet]

### Console Output

    - Calculation result
    - Absolut error
    - Relative error
    - Calculation time
