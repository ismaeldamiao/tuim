# TODO

This is the todo file for the Tuim's ELF Interpreter.  
See: <https://github.com/todo-md/todo-md>

## Portable code

- [X] Parse ELF header
- [X] Parse program header table
- [X] Parse dynamic table
  - [X] Get dependency names
  - [X] Get relocation entries
  - [X] Get dynamic symbols
- [X] Load segments
- [ ] Execute initialization functions
- [ ] Execute termination functions

## OS dependent code

- [ ] Linux
  - [X] Execution as a child process
  - [X] Load segments on memory
  - [X] Start execution
- [ ] Windows-NT
  - [ ] Execution as a child process
  - [ ] Load segments on memory
  - [ ] Start execution
- [ ] XNU
  - [ ] Execution as a child process
  - [ ] Load segments on memory
  - [ ] Start execution
- [ ] Android
  - [ ] Write a native glue around the Linux code
- [ ] UEFI
  - [ ] Execution as a child process
  - [ ] Load segments on memory
  - [ ] Start execution

## Architecture dependent code

- [ ] riscv32
  - [ ] Parse ELF header
  - [ ] Implement relocation
  - [ ] Implement stack startup
- [ ] riscv64
  - [ ] Implement relocation
  - [ ] Implement stack startup
- [ ] arm
  - [X] Parse ELF header
  - [X] Implement relocation
  - [ ] Implement stack startup
- [ ] aarch64
  - [ ] Parse ELF header
  - [ ] Implement relocation
  - [ ] Implement stack startup
- [ ] i386
  - [ ] Parse ELF header
  - [ ] Implement relocation
  - [ ] Implement stack startup
- [ ] amd64
  - [ ] Parse ELF header
  - [ ] Implement relocation
  - [ ] Implement stack startup
