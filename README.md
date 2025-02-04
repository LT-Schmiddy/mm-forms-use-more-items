# Majora's Mask: Transformation Use More Items

This a mod for Zelda64Recomp that allows Link's transformation to use a few more items than they normally can.
This is a port of a ROM mod I made years ago. Currently a beta since I haven't fully tested this in Zelda64Recomp yet.

#### Deku Link

* Deku Sticks
* Magic Beans

#### Goron Link

* Bombs
* Bombchus
* Deku Nuts
* Magic Beans

#### Zora Link

* Bombs
* Bombchus
* Deku Sticks
* Deku Nuts
* Magic Beans

### Tools

You'll need to install `clang`, `make`, and the latest version of Python to build this template.

* On Windows, using [chocolatey](https://chocolatey.org/) to install both is recommended. The packages are `llvm` and `make` respectively.
  * The LLVM 19.1.0 [llvm-project](https://github.com/llvm/llvm-project) release binary, which is also what chocolatey provides, does not support MIPS correctly. The solution is to install 18.1.8 instead, which can be done in chocolatey by specifying `--version 18.1.8` or by downloading the 18.1.8 release directly.
* On Linux, these can both be installed using your distro's package manager.
* On MacOS, these can both be installed using Homebrew. Apple clang won't work, as you need a mips target for building the mod code.

On Linux and MacOS, you'll need to also ensure that you have the `zip` utility installed.

### Building

If you have Python, simply running the `build_mod.py` script should take care of everything.

If not, you can follow the instructions below:

* First, you need to build the [N64Recomp](https://github.com/N64Recomp/N64Recomp) library (included) for the `RecompModTool` utility.
* Second, run `make` (with an optional job count) to build the mod code itself.
* Third, run the `RecompModTool` utility with `mod.toml` as the first argument and the build dir (`build` in the case of this template) as the second argument.

 This will produce your mod's `.nrm` file in the build folder.
