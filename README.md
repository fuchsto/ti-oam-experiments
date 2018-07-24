
# Setup and Usage

Source the provided environment setup scripts. These are unaltered from the
original TI distribution.

Building and running the examples only requires the development environment
provided in the TI SDK.


# OAM Evaluation Scenarios

Directory `minimal` contains a minimal example containing just the OAM
boilerplate that can be copied to implement additional test applications.

An example use from command line is provided in every example subdirectory
in `<example name>.run`.  


## Utilities

### dsp\_freq

Calculates clock frequency deviations between host and target.
Provided to reveal suspected misconfiguration at platform-level of
the DSP system.


## Evaluation of Host/Target Communication

### comm\_async

Evaluates task cancelation latency from a series of target tasks, reports
min/max/mean/sdev.

Target tasks are launched and cancelled asynchronously using C++11 launch
policies. Interleaving output from host and target code demonstrate
parallel execution.

The `comm_async` application implements a non-trivial use
case and should yield the most realistic measurements in this application
collection for this purpose.

**Executable**: `comm_async.bin`

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion

Example usage:

  $ ./comm_async.bin -b 102400 -hr 200 -tr 10000 -pi 1 -c


### concurrent

Evaluates concurrent execution of two target tasks executed using C++11
asynchronous launch policies.

This test application is supposed to fail as only one target task is allowed
for execution on DSP at any time. It has been added to learn the effects and
behavior of the OMPACC subsystem in this defect scenario.

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion


### dsp\_img\_filter

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion


### poll\_cancel

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion


## Evaluation of Storage (Allocation and Declaration) and Linkage

### static\_vars



### globvars

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion


### declare

**Executable**: `declare.bin`

**Synopsis**

Demonstrates combination of declaration variants such as accessing static
symbols declared in host code from target regions, external linkage,
accessing symbols in nested declarations etc.

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion


### extlib\_linkage

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion

### sym\_heap\_reset

**Options**

      -b <buffer size> Bytes to allocate per repetition
      -hr <num rep>    Number of repetitions at host
      -tr <num rep>    Number of repetitions at target
      -to <ms>         Timeout duration of target task in ms
      -pi <ms>         Polling interval at target task in ms
      -c               Cancel request after host task completion



## Evaluation of OAM Builds


### hybrid\_lib


### task\_extlib




