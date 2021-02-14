To compile & run mvote: 
	On command line, run `make`
	run `./mvote -f {filename} -m {bucketSize}`

To quit mvote:
	run `exit`
	please avoid adding spaces after the command. It does not get interpreted.

To test function `bulk vote`:
	run `bv file.txt`

Note:
	{filename} needs to exist in the same level of directory with the codes. Currently, voters50.csv, voters100000.csv are available. 
	For bulk vote function, it is the same. Currently, I have a test file named `file.txt`. 