
exe=mp4_resolver

srcs=Mp4.c
objs=Mp4.o

$(objs) : $(srcs)
	gcc -c $(srcs) -g -Wall

$(exe) : $(objs)
	gcc -o $@ $(objs)

all : $(exe)
