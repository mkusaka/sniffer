# sniffer

## Build
### Dependencies
* gcc
* boost
* libcurl
* log4cpp

### Build sniffer
```
$ git clone git@github.com:mozzzzy/sniffer.git
$ cd src/
$ make
```

### Build sniffer plugin
```
$ cd src/sniffers/log_sniffer/
$ make
```

### Build notifier plugin
```
$ cd src/notifiers/slack_notifier/
$ make
```

## Usage
```
$ cd src/
$ ./sniffer -h
command line options:
  -h [ --help ]         produce help message
  -l [ --lconfig ] arg  specify logger's config file path
  -s [ --sconfig ] arg  specify sniffer's config file path
```
