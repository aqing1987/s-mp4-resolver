
exe=mp4_resolver

srcs=mp4_resolver.c
objs=mp4_resolver.o

#srcs=mp4_resolver.c mpeg4.c Mux.c Mp4.c Audio.c Video.c Mybs.c FileIo.c \
	Ftyp_Box.c Moov_Box.c Mdat_Box.c Free_Box.c \
	Mvhd_Box.c Trak_Box.c Tkhd_Box.c Mdia_Box.c \
	Mdhd_Box.c Hdlr_Box.c Minf_Box.c Vmhd_Box.c Smhd_Box.c Dinf_Box.c \
	Stbl_Box.c Stsd_Box.c Stts_Box.c Ctts_Box.c Stsc_Box.c Stsz_Box.c \
	Stco_Box.c Stss_Box.c

#objs=mp4_resolver.o mpeg4.o Mux.o Mp4.o Audio.o Video.o Mybs.o FileIo.o \
	Ftyp_Box.o Moov_Box.o Mdat_Box.c Free_Box.o \
	Mvhd_Box.o Trak_Box.o Tkhd_Box.o Mdia_Box.o \
	Mdhd_Box.o Hdlr_Box.o Minf_Box.o Vmhd_Box.o Smhd_Box.o Dinf_Box.o \
	Stbl_Box.o Stsd_Box.o Stts_Box.o Ctts_Box.o Stsc_Box.o Stsz_Box.o \
	Stco_Box.o Stss_Box.o

all : $(exe)

$(objs) : $(srcs)
	gcc -c $(srcs) -g -Wall

$(exe) : $(objs)
	gcc -o $@ $(objs)

clean:
	rm *.o *.a mp4_resolver

