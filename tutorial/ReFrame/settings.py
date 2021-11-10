site_configuration = {
    'systems': [
        {
            'name': 'aws-cluster',
            'descr': 'AWS Arm C6gn PCluster',
            'hostnames': ['ip'],
            'modules_system': 'tmod',
            'partitions': [
                {
                    'name': 'login',
                    'descr': 'Login nodes',
                    'scheduler': 'local',
                    'launcher': 'local',
                    'environs': ['builtin', 
			         'aws_gcc_10.3', 
			         'aws_gcc_11.2', 
				 'aws_acfl_21.1', 
				 'aws_nvhpc_21.2'
				 ],
                },
                {
                    'name': 'compute',
                    'descr': 'C6gn.16xlarge Nodes',
                    'scheduler': 'slurm',
                    'launcher': 'srun',
                    'access': ['-p compute'],
                    'environs': ['builtin', 
				'aws_gcc_10.3', 
				'aws_gcc_11.2', 
				'aws_acfl_21.1', 
				'aws_nvhpc_21.2'
				],
                }
            ]
        },
	{
            'name': 'oci-cluster',
            'descr': 'OCI A1 Compute Nodes',
            'hostnames': ['instance'],
            'modules_system': 'tmod',
            'partitions': [
                {
                    'name': 'login',
                    'descr': 'Login nodes',
                    'scheduler': 'local',
                    'launcher': 'mpirun',
                    'environs': ['builtin', 
				 'oci_gcc_10.2', 
				 'oci_gcc_11.2', 
				 'oci_acfl_21.0', 
				 'oci_nvhpc_21.9'
                                 ],
                }
            ]
        }
    ],
    'environments': [
        {
            'name': 'builtin',
            'cc': 'gcc',
            'cxx': 'g++',
            'ftn': 'gfortran'
        },
        {
            'name': 'aws_gcc_10.3',
            'modules': ['openmpi/gcc-10.3.0/4.1.1'],
            'cc': 'gcc',
            'cxx': 'g++',
            'ftn': 'gfortran'
        },
        {
            'name': 'aws_gcc_11.2',
            'modules': ['openmpi/gcc-11.2.0/4.1.1'],
            'cc': 'gcc',
            'cxx': 'g++',
            'ftn': 'gfortran'
        },
        {
            'name': 'aws_acfl_21.1',
            'modules': ['openmpi/acfl-21.1/4.1.1'],
            'cc': 'armclang',
            'cxx': 'armclang++',
            'ftn': 'armflang'
        },
        {
            'name': 'aws_nvhpc_21.2',
            'modules': ['openmpi/nvhpc-21.2/4.1.1'],
            'cc': 'nvc',
            'cxx': 'nvc++',
            'ftn': 'nvfortran'
        },
        {
            'name': 'oci_gcc_10.2',
            'modules': ['Generic-AArch64/RHEL/8/gcc-10.2.0/openmpi/4.1.1'],
            'cc': 'gcc',
            'cxx': 'g++',
            'ftn': 'gfortran'
        },
        {
            'name': 'oci_gcc_11.2',
            'modules': ['Generic-AArch64/RHEL/8/gcc-11.2.0/openmpi/4.1.1'],
            'cc': 'gcc',
            'cxx': 'g++',
            'ftn': 'gfortran'
        },
        {
            'name': 'oci_acfl_21.0',
            'modules': ['Generic-AArch64/RHEL/8/acfl-21.0/openmpi/4.1.1'],
            'cc': 'armclang',
            'cxx': 'armclang++',
            'ftn': 'armflang'
        },
        {
            'name': 'oci_nvhpc_21.9',
            'modules': ['nvhpc/21.9'],
            'cc': 'nvc',
            'cxx': 'nvc++',
            'ftn': 'nvfortran'
        },
    ],
    'logging': [
        {
            'level': 'debug',
            'handlers': [
                {
                    'type': 'stream',
                    'name': 'stdout',
                    'level': 'info',
                    'format': '%(message)s'
                },
                {
                    'type': 'file',
                    'level': 'debug',
                    'format': '[%(asctime)s] %(levelname)s: %(check_info)s: %(message)s',   # noqa: E501
                    'append': False
                }
            ],
            'handlers_perflog': [
                {
                    'type': 'filelog',
                    'prefix': '%(check_system)s/%(check_partition)s',
                    'level': 'info',
                    'format': (
                        '%(check_job_completion_time)s|reframe %(version)s|'
                        '%(check_info)s|jobid=%(check_jobid)s|'
                        '%(check_perf_var)s=%(check_perf_value)s|'
                        'ref=%(check_perf_ref)s '
                        '(l=%(check_perf_lower_thres)s, '
                        'u=%(check_perf_upper_thres)s)|'
                        '%(check_perf_unit)s'
                    ),
                    'append': True
                }
            ]
        }
    ],
}
