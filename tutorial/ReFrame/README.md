# ReFrame Tutorial


## Installing ReFrame

```
git clone https://github.com/eth-cscs/reframe.git
cd reframe
./bootstrap.sh
./bin/reframe -V
export PATH=$PWD/bin:$PATH
```

## Configuring For Hackathon

```
mkdir ~/.reframe
wget -O ~/.reframe/settings.py <path to setting.py> 
```


## Hello World

Hello World test from [ReFrame Tutorial](https://reframe-hpc.readthedocs.io/en/stable/tutorial_basics.html#the-hello-world-test)

Source code:
```
mkdir src
cat > src/hello.c << EOF
#include <stdio.h>


int main()
{
    printf("Hello, World!\n");
    return 0;
}
EOF
```

ReFrame File:
```
cat > hello.py << EOF
import reframe as rfm
import reframe.utility.sanity as sn


@rfm.simple_test
class HelloTest(rfm.RegressionTest):
    valid_systems = ['*']
    valid_prog_environs = ['*']
    sourcepath = 'hello.c'

    @sanity_function
    def assert_hello(self):
        return sn.assert_found(r'Hello, World\!', self.stdout)
EOF
```

### Running the Example

```
reframe -c hello.py -r
```

### Example Output (AWS Cluster)

```
[ReFrame Setup]
  version:           3.10.0-dev.0+4e6665d8
  command:           '/home/train25/reframe/bin/reframe -c ./hello.py -r'
  launched by:       train25@ip-10-0-0-115.ec2.internal
  working directory: '/home/train25/example'
  settings file:     '/home/train25/.reframe/settings.py'
  check search path: '/home/train25/example/hello.py'
  stage directory:   '/home/train25/example/stage'
  output directory:  '/home/train25/example/output'

[==========] Running 1 check(s)
[==========] Started on Wed Nov 10 21:39:23 2021 

[----------] started processing HelloTest (HelloTest)
[ RUN      ] HelloTest on aws-cluster:login using builtin
[ RUN      ] HelloTest on aws-cluster:login using aws_gcc_10.3
[ RUN      ] HelloTest on aws-cluster:login using aws_gcc_11.2
[ RUN      ] HelloTest on aws-cluster:login using aws_acfl_21.1
[ RUN      ] HelloTest on aws-cluster:login using aws_nvhpc_21.2
[ RUN      ] HelloTest on aws-cluster:compute using builtin
[ RUN      ] HelloTest on aws-cluster:compute using aws_gcc_10.3
[ RUN      ] HelloTest on aws-cluster:compute using aws_gcc_11.2
[ RUN      ] HelloTest on aws-cluster:compute using aws_acfl_21.1
[ RUN      ] HelloTest on aws-cluster:compute using aws_nvhpc_21.2
[----------] finished processing HelloTest (HelloTest)

[----------] waiting for spawned checks to finish
[       OK ] ( 1/10) HelloTest on aws-cluster:login using aws_nvhpc_21.2 [compile: 0.154s run: 0.987s total: 1.157s]
[       OK ] ( 2/10) HelloTest on aws-cluster:login using aws_gcc_11.2 [compile: 0.088s run: 1.394s total: 1.498s]
[       OK ] ( 3/10) HelloTest on aws-cluster:login using builtin [compile: 0.042s run: 1.663s total: 1.722s]
[       OK ] ( 4/10) HelloTest on aws-cluster:login using aws_gcc_10.3 [compile: 0.086s run: 1.835s total: 1.938s]
[       OK ] ( 5/10) HelloTest on aws-cluster:login using aws_acfl_21.1 [compile: 0.116s run: 1.769s total: 1.901s]
[       OK ] ( 6/10) HelloTest on aws-cluster:compute using aws_nvhpc_21.2 [compile: 0.154s run: 285.129s total: 285.300s]
[       OK ] ( 7/10) HelloTest on aws-cluster:compute using aws_gcc_11.2 [compile: 0.088s run: 285.545s total: 285.649s]
[       OK ] ( 8/10) HelloTest on aws-cluster:compute using builtin [compile: 0.037s run: 285.825s total: 285.878s]
[       OK ] ( 9/10) HelloTest on aws-cluster:compute using aws_gcc_10.3 [compile: 0.086s run: 285.963s total: 286.065s]
[       OK ] (10/10) HelloTest on aws-cluster:compute using aws_acfl_21.1 [compile: 0.116s run: 285.877s total: 286.010s]
[----------] all spawned checks have finished

[  PASSED  ] Ran 10/10 test case(s) from 1 check(s) (0 failure(s), 0 skipped)
[==========] Finished on Wed Nov 10 21:44:10 2021 
Run report saved in '/home/train25/.reframe/reports/run-report.json'
Log file(s) saved in '/tmp/rfm-00aj2xt6.log'

```


### Explanation

So lets look at whats actually happened.

Firstly, ReFrame has run 10 tests - for our simple `Hello World` example.

In our `setting.py` file, we have described the system we are running on (an AWS cluster in this case).
We have said that we can either run on the login node, or the compute nodes (login and compute).

We have also said that on both of these node types we have 5 compilers available:
* system GCC (7.3)
* GCC-10.3
* GCC-11.2
* ACfL-21.1
* NVHPC-21.2

Thus ReFrame generates the combinatorial set of these tests: 10 in total.

This was just a simple test, but for more complex tests we could include a performance comparison here, to compare the performance on different node types and compilers.
