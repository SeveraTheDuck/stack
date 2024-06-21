# Stack interface and dynamic array realization
### From [SeveraTheDuck](https://github.com/SeveraTheDuck)

## Contents
1. [Short description](#short-description)
2. [Install guide](#install-guide)
3. [Usage](#usage)

## Short description
The [repository](https://github.com/SeveraTheDuck/stack) contains stack data structure interface and implementation via void* dynamic array. You can use the interface for your own implementation, for example, using linked list.
The repository also contains old version with canary and hash protection, but it does not support different types in one program at the same time.

## Install guide
1. Download the [repository](https://github.com/SeveraTheDuck/stack) from github or use
`git clone https://github.com/SeveraTheDuck/stack`.
2. If you want to use the library, delete `source/example.c` file and copy include and source folders to your project.
3. If you want to run simple example test, open the terminal in stack folder and enter `make` command. Then run `stack_exe` file.

## Usage
The stack has following interface:
1. Constructor and destructor.
2. Push operation - copies value from the buffer into the stack and makes it the top element.
3. Pop operation - removes the top element from the stack.
4. Top operation - copies the top element from the stack into given buffer.

For details see the documentation in `include/stack.h` file.
