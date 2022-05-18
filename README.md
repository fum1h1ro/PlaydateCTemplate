# Simple Template for C_API

![sample](can_delete/record_sample.gif)


## rake tasks

```
rake build                       # Build
rake clean                       # Remove any temporary products
rake clobber                     # Remove any generated files
rake generate:device:debug       # Generate Makefile (device, Debug)
rake generate:device:release     # Generate Makefile (device, Release)
rake generate:simulator:debug    # Generate Makefile (simulator, Debug)
rake generate:simulator:release  # Generate Makefile (simulator, Release)
rake generate:xcode:simulator    # Generate Xcode project (simulator)
rake run                         # Run on Simulator
rake test                        # Test
```


## configure

- Please specify your game name.
    - https://github.com/fum1h1ro/PlaydateCTemplate/blob/master/Rakefile#L3


## usage

1. `rake (clean|clobber)`
2. `rake generate:simulator:(debug|release)`
3. `rake build`
4. `rake run`
5. `rake test`
    - only for `simulator:debug`
5. `rake (clean|clobber)`
6. `rake generate:device:(debug|release)`

