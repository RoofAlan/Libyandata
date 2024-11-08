# Libyandata
# What is Libyandata?
Linyandata is a static library. And it's a simple library for WeiyanYun(https://yandata.cn/)

## Usage
Before use it, you need to install this package:
	cJSON
	libcurl (curl)
(Maybe you need more package.)
To make it:
```
# Clone this repo
cd libyandata
make
```
Then, include the "kami.h" or "notice.h" in your C code.

## Function
If you want to join the card password verification system (this version does not have security features such as rc4), you only need to use the kami_check() function.

```
kami_check("Your api here", app ID, card password);
```

In the same way, if you want to add an announcement, you can also do so, just use the notice() function.
```
notice("Your api here");
```
## Compile
You need to link some library. Such like this:
```
gcc -o filename filename.c -L/path/to/library -lnotice -lkami -lcurl -lcjson
```