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
[==========] Started on Wed Nov 10 21:09:00 2021 

[----------] started processing HelloTest (HelloTest)
[ RUN      ] HelloTest on aws-cluster:login using builtin
[ RUN      ] HelloTest on aws-cluster:login using aws_gcc_10.3
[ RUN      ] HelloTest on aws-cluster:login using aws_gcc_11.2
[ RUN      ] HelloTest on aws-cluster:login using aws_acfl_21.1
[ RUN      ] HelloTest on aws-cluster:login using aws_nvhpc_21.2
[----------] finished processing HelloTest (HelloTest)

[----------] waiting for spawned checks to finish
[       OK ] (1/5) HelloTest on aws-cluster:login using aws_gcc_11.2 [compile: 6.195s run: 0.474s total: 6.686s]
[       OK ] (2/5) HelloTest on aws-cluster:login using builtin [compile: 0.042s run: 6.852s total: 6.911s]
[       OK ] (3/5) HelloTest on aws-cluster:login using aws_nvhpc_21.2 [compile: 0.155s run: 0.236s total: 0.407s]
[       OK ] (4/5) HelloTest on aws-cluster:login using aws_gcc_10.3 [compile: 0.087s run: 6.896s total: 6.999s]
[       OK ] (5/5) HelloTest on aws-cluster:login using aws_acfl_21.1 [compile: 0.115s run: 0.609s total: 0.741s]
[----------] all spawned checks have finished

[  PASSED  ] Ran 5/5 test case(s) from 1 check(s) (0 failure(s), 0 skipped)
[==========] Finished on Wed Nov 10 21:09:07 2021 
Run report saved in '/home/train25/.reframe/reports/run-report.json'
Log file(s) saved in '/tmp/rfm-dapw41gm.log'

```
