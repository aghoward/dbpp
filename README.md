# db++

`db++` is a c++ recreation of dirbuster (`dirb`). I made this a toy project after getting fed up with dirbuster being broken. This project will work in the happy path, but it is a toy so don't expect it to give nice failures yet. Feel free to submit PRs for anything you would like to see.

This project relies on [cpr](https://github.com/whoshuu/cpr) as a third-party dependency. Other third-party dependencies are submoduled under `include`, so don't forget to `git submodule update --init` after checking out.
